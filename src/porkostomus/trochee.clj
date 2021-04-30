(ns porkostomus.trochee
  (:require [scad-clj.scad :as scad :refer [write-scad]]
            [scad-clj.model :as model :refer [with-fn translate mirror difference sphere cube cylinder polyhedron union intersection hull color rotate scale extrude-linear polygon project]]))

(defn view [block]
  (spit "resources/view.scad"
        (scad/write-scad
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


;;;;;;;;;;;;;;;;
;; SA Keycaps ;;
;;;;;;;;;;;;;;;;

(def sa-length 18.25)
(def sa-double-length 37.5)
(def sa-cap {1 (let [bl2 (/ 18.5 2)
                     m (/ 17 2)
                     key-cap (hull (->> (polygon [[bl2 bl2] [bl2 (- bl2)] [(- bl2) (- bl2)] [(- bl2) bl2]])
                                        (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                        (translate [0 0 0.05]))
                                   (->> (polygon [[m m] [m (- m)] [(- m) (- m)] [(- m) m]])
                                        (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                        (translate [0 0 6]))
                                   (->> (polygon [[6 6] [6 -6] [-6 -6] [-6 6]])
                                        (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                        (translate [0 0 12])))]
                 (->> key-cap
                      (translate [0 0 (+ 5 plate-thickness)])
                      (color [220/255 163/255 163/255 1])))
             2 (let [bl2 (/ sa-double-length 2)
                     bw2 (/ 18.25 2)
                     key-cap (hull (->> (polygon [[bw2 bl2] [bw2 (- bl2)] [(- bw2) (- bl2)] [(- bw2) bl2]])
                                        (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                        (translate [0 0 0.05]))
                                   (->> (polygon [[6 16] [6 -16] [-6 -16] [-6 16]])
                                        (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                        (translate [0 0 12])))]
                 (->> key-cap
                      (translate [0 0 (+ 5 plate-thickness)])
                      (color [127/255 159/255 127/255 1])))
             1.5 (let [bl2 (/ 18.25 2)
                       bw2 (/ 28 2)
                       key-cap (hull (->> (polygon [[bw2 bl2] [bw2 (- bl2)] [(- bw2) (- bl2)] [(- bw2) bl2]])
                                          (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                          (translate [0 0 0.05]))
                                     (->> (polygon [[11 6] [-11 6] [-11 -6] [11 -6]])
                                          (extrude-linear {:height 0.1 :twist 0 :convexity 0})
                                          (translate [0 0 12])))]
                   (->> key-cap
                        (translate [0 0 (+ 5 plate-thickness)])
                        (color [240/255 223/255 175/255 1])))})

;;;;;;;;;;;;;;;;;;;;;;;;;
;; Placement Functions ;;
;;;;;;;;;;;;;;;;;;;;;;;;

(def columns (range 0 8))
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
  (->> (sphere 2.5)
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
                   9.5]))

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
                                      (place x (- back-y 0.95) (wall-sphere-at 
                                                                [0 (scale-to-range (+ (/ mount-height -2) -3.5)
                                                                     (+ (/ mount-height 2) 5.0)
                                                                                 1)
                                                                                0]))
                                      (place (+ x step) (- back-y 0.95) (wall-sphere-at
                                                                         [0
                                                                          (scale-to-range
                                                                           (+ (/ mount-height -2) -3.5)
                                                                           (+ (/ mount-height 2) 5.0)
                                                                           1)
                                                                          0])))))))))

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

(defn bottom-at [x y z]
  (let [bevel (union
               (translate [(- x 82) (+ y 27) (- z 2)]
                          (rotate -45 [0 1 0] (cube 10 100 20)))
               (translate [(+ x 82) (+ y 27) (- z 2)]
                          (rotate 45 [0 1 0] (cube 10 100 20))))]
    (difference
     (translate [x y z]
                (cube 151 153 4))
     bevel)))

(def pico
 (union 
  (cube 9 9 4)
   (translate [0 -24 -1.5]
  (cube 21 53 2))))

(def pico-cavity
  (union
   (translate [0 -24 -1.5]
              (cube 22 53 3))))

(def wrist-rest
  (let [clips (union (translate [-11 0 16.5] (cube 2 20 13))
                     (translate [141.5 0 16.5] (cube 2 20 13))
                     (translate [-10 5 16.5] (rotate 1.56 [0 1 0] (cylinder 4 3)))
                     (translate [140.5 5 16.5] (rotate 1.56 [0 1 0] (cylinder 4 3))))]
    (union
     (difference

      (scale [1 0.5 0.15]
             (translate [65 -50 100]
                        (sphere 100)))

      (translate [65 -10 -5]
                 (cube 170 130 30))
      (translate [65 14 20]
                 (cube 170 40 30))
      (translate [-37 -22 10]
                 (cube 50 70 40))
      (translate [167.5 -22 10]
                 (cube 50 70 40)))
     clips)))

(def trochee-bottom
  (union 
   (difference (bottom-at 65.2 64 12.0)
               (translate [66 137.5 15] pico-cavity)
               (translate [0 -7 0] wrist-rest))
   (difference
   (translate [66 89 15] (cube 24 5 7))
    (translate [66 98 10] (rotate 45 [1 0 0] (cube 22 22 22))))))

(def trochee-top
  (difference
   (difference (union
                key-holes
                left-wall
                right-wall
                front-wall
                back-wall)
               (translate [93 63 5] (cube 220 170 10)))
   (translate [66 137.5 17] pico)
   trochee-bottom
   (translate [0 -7 0] wrist-rest)))

(comment
  (view trochee-top)
  )
