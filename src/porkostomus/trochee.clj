(ns porkostomus.trochee
  (:require [scad-clj.scad :as scad :refer [write-scad]]
            [scad-clj.model :as model :refer [with-fn translate mirror difference sphere cube cylinder polyhedron union hull color rotate extrude-linear polygon project]]))

(defn view [block]
  (spit "resources/view.scad"
        (write-scad
         block)))

;;;;;;;;;;;;;;;;;
;; Switch Hole ;;
;;;;;;;;;;;;;;;;;

(def keyswitch-height 14.25) ;; Was 14.1, then 14.25
(def keyswitch-width 14.25)

(def sa-profile-key-height 12.7)

(def plate-thickness 4)
(def mount-width (+ keyswitch-width 3))
(def mount-height (+ keyswitch-height 3))

(def alps-notch-width 15.5)
(def alps-notch-height 1)

(def single-plate
  (let [top-wall (->> (cube (+ keyswitch-width 3) 2.2 plate-thickness)
                      (translate [0
                                  (+ (/ 2.2 2) (/ keyswitch-height 2))
                                  (/ plate-thickness 2)]))
        left-wall (union (->> (cube 1.5 (+ keyswitch-height 3) plate-thickness)
                              (translate [(+ (/ 1.5 2) (/ keyswitch-width 2))
                                          0
                                          (/ plate-thickness 2)]))
                         (->> (cube 1.5 (+ keyswitch-height 3) 1.0)
                              (translate [(+ (/ 1.5 2) (/ alps-notch-width 2))
                                          0
                                          (- plate-thickness
                                             (/ alps-notch-height 2))]))
                         )
        plate-half (union top-wall left-wall)]
    (union plate-half
           (->> plate-half
                (mirror [1 0 0])
                (mirror [0 1 0])))))

;;;;;;;;;;;;;;;;;;;;;;;;;
;; Placement Functions ;;
;;;;;;;;;;;;;;;;;;;;;;;;;

(def columns (range 0 11))
(def rows (range 0 8))
(def π Math/PI)
(def α (/ π 12))
(def β (/ π 36))
(def cap-top-height (+ plate-thickness sa-profile-key-height))
(def row-radius (+ (/ (/ (+ mount-height 1/2) 2)
                      (Math/sin (/ α 2)))
                   cap-top-height))
(def column-radius (+ (/ (/ (+ mount-width 2.0) 2)
                         (Math/sin (/ β 2)))
                      cap-top-height))

(defn key-place [column row shape]
  (let [row-placed-shape (->> shape
                              (translate [0 0 (- row-radius)])
                              (translate [0 (* 18.5 row) 0])
                              (translate [0 0 row-radius]))
        placed-shape (->> row-placed-shape
                          (translate [0 0 (- column-radius)])
                          (translate [(* 18.5 column) 0 0])
                          (translate [0 0 column-radius]))]
    (->> placed-shape
         (rotate (/ π 24) [1 0 0])
         (translate [0 0 21.5]))))

(defn case-place [column row shape]
  (let [row-placed-shape (->> shape
                              (translate [0 0 (- row-radius)])
                              (translate [0 (* 18.5 row) 0])
                              (translate [0 0 row-radius]))
        placed-shape (->> row-placed-shape
                          (translate [0 0 (- column-radius)])
                          (translate [(* 18.5 column) 0 0])
                          (translate [0 0 column-radius]))]
    (->> placed-shape
         (rotate (/ π 24) [1 0 0])
         (translate [0 0 13]))))

(def key-holes
  (apply union
         (for [column columns
               row rows]
           (->> single-plate
                (key-place column row)))))

(def caps
  (apply union
         (for [column columns
               row rows]
           (->> (sa-cap (if (= column 5) 1 1))
                (key-place column row)))))

;;;;;;;;;;;;;;;;;;;;
;; Web Connectors ;;
;;;;;;;;;;;;;;;;;;;;

(def web-thickness 3.5)
(def post-size 0.1)
(def web-post (->> (cube post-size post-size web-thickness)
                   (translate [0 0 (+ (/ web-thickness -2)
                                      plate-thickness)])))

(def post-adj (/ post-size 2))
(def web-post-tr (translate [(- (/ mount-width 2) post-adj) (- (/ mount-height 2) post-adj) 0] web-post))
(def web-post-tl (translate [(+ (/ mount-width -2) post-adj) (- (/ mount-height 2) post-adj) 0] web-post))
(def web-post-bl (translate [(+ (/ mount-width -2) post-adj) (+ (/ mount-height -2) post-adj) 0] web-post))
(def web-post-br (translate [(- (/ mount-width 2) post-adj) (+ (/ mount-height -2) post-adj) 0] web-post))

(defn triangle-hulls [& shapes]
  (apply union
         (map (partial apply hull)
              (partition 3 1 shapes))))

(def connectors
  (apply union
         (concat
          ;; Row connections
          (for [column (drop-last columns)
                row rows]
            (triangle-hulls
             (key-place (inc column) row web-post-tl)
             (key-place column row web-post-tr)
             (key-place (inc column) row web-post-bl)
             (key-place column row web-post-br)))

          ;; Column connections
          (for [column columns
                row (drop-last rows)]
            (triangle-hulls
             (key-place column row web-post-bl)
             (key-place column row web-post-br)
             (key-place column (inc row) web-post-tl)
             (key-place column (inc row) web-post-tr)))

          ;; Diagonal connections
          (for [column (drop-last columns)
                row (drop-last rows)]
            (triangle-hulls
             (key-place column row web-post-br)
             (key-place column (inc row) web-post-tr)
             (key-place (inc column) row web-post-bl)
             (key-place (inc column) (inc row) web-post-tl))))))

(comment
  (view
   (union
    key-holes
    ;connectors
    )))

;;;;;;;;;;
;; Case ;;
;;;;;;;;;;

;; In column units
(def right-wall-column (+ (last columns) 0.55))
(def left-wall-column (- (first columns) 1/2))
(def back-y -0.5)

(defn range-inclusive [start end step]
  (concat (range start end step) [end]))

(def wall-step 0.2)
(def wall-sphere-n 20) ;;Sphere resolution, lower for faster renders

(defn wall-sphere-at [coords]
  (->> (sphere 2)
       (translate coords)
       (with-fn wall-sphere-n)))

(defn scale-to-range [start end x]
  (+ start (* (- end start) x)))

(defn wall-sphere-bottom [front-to-back-scale]
  (wall-sphere-at [0
                   (scale-to-range
                    (+ (/ mount-height -2) -3.5)
                    (+ (/ mount-height 2) 5.0)
                    front-to-back-scale)
                   -1.5]))

(defn wall-sphere-top [front-to-back-scale]
  (wall-sphere-at [0
                   (scale-to-range
                    (+ (/ mount-height -2) -3.5)
                    (+ (/ mount-height 2) 3.5)
                    front-to-back-scale)
                   10]))

(def wall-sphere-top-back (wall-sphere-top 1))
(def wall-sphere-bottom-back (wall-sphere-bottom 1))
(def wall-sphere-bottom-front (wall-sphere-bottom 0))
(def wall-sphere-top-front (wall-sphere-top 0))

(def front-wall
  (let [step wall-step
        place case-place]
    (apply union
           (for [x (range-inclusive 0 (- right-wall-column step) step)]
             (hull (place (- x 1/2) 8.3 wall-sphere-top-front)
                   (place (+ x step) 8.3 wall-sphere-top-front)
                   (place (- x 1/2) 8.77 (wall-sphere-at [0 -25 -20.5]))
                   (place (+ x step) 8.77 (wall-sphere-at [0 -25 -20.5])))))))

(def back-wall
  (let [step wall-step
        place case-place]
    (translate [0 -1 -3]
               (rotate (/ π 10) [-1 0 0]
                       (apply union
                              (for [x (range-inclusive left-wall-column (- right-wall-column step) step)]
                                (hull (place x (dec back-y) wall-sphere-top-back)
                                      (place (+ x step) (dec back-y) wall-sphere-top-back)
                                      (place x (- back-y 0.95) wall-sphere-bottom-back)
                                      (place (+ x step) (- back-y 0.95) wall-sphere-bottom-back))))))))

(def left-wall
  (let [place case-place]
    (union
     (apply union
            (map (partial apply hull)
                 (partition 2 1
                            (for [scale (range-inclusive 0 0.75 0.2)]
                              (let [x (scale-to-range 8.3 -3.8 scale)]
                                (hull (place left-wall-column x (wall-sphere-top scale))
                                      (place left-wall-column (* 0.882 x) (wall-sphere-at [0 2 (- -3 (* 2.1 x))])))))))))))

(for [scale (range-inclusive 0 0.75 0.2)]
 (let [x (scale-to-range 8.7 -3.9 scale)]
  x))

(comment
  (view (union
         (difference
          left-wall
          (translate [-10 60 10]
                     (cube 10 150 10)))
         (rotate 0 [0 1 0]
                 (translate [0 0 3]
                            (difference  (rotate 45 [0 1 0]
                                                 (translate [-14 60 0] (cube 5 150 5)))
                                         (union
                                          (translate [-8 60 17] (cube 10 150 5))
                                          (translate [-8 60 10] (cube 10 150 5)))))))))

(def right-wall
  (let [place case-place]
    (union
     (apply union
            (map (partial apply hull)
                 (partition 2 1
                            (for [scale (range-inclusive 0 0.75 0.2)]
                              (let [x (scale-to-range 8.3 -3.8 scale)]
                                (hull (place right-wall-column x (wall-sphere-top scale))
                                      (place right-wall-column (* 0.882 x) (wall-sphere-at [0 2 (- -3 (* 2.1 x))])))))))))))

(def bottom-wall
  (let [step wall-step
        place case-place]
    (rotate -0.15 [1 0 0]
            (translate [0 0 -3]
                       (apply union
                              (for [x (range-inclusive left-wall-column (- right-wall-column step) step)]
                                (hull (place x 7.6 wall-sphere-bottom-front)
                                      (place (+ x step) 7.6  wall-sphere-bottom-front)
                                      (place x (- back-y 0.9) wall-sphere-bottom-back)
                                      (place (+ x step) (- back-y 0.9) wall-sphere-bottom-back))))))))
