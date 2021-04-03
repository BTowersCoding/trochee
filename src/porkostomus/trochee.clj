(ns porkostomus.trochee
  (:require [scad-clj.scad :as scad :refer [write-scad]]
            [scad-clj.model :as model :refer [with-fn translate mirror difference sphere cube cylinder union hull color rotate extrude-linear polygon project]]))

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
;;;;;;;;;;;;;;;;;;;;;;;;;

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
                   -2]))

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

(defn top-case-cover [place-fn sphere
                      x-start x-end
                      y-start y-end
                      step]
  (apply union
         (for [x (range-inclusive x-start (- x-end step) step)
               y (range-inclusive y-start (- y-end step) step)]
           (hull (place-fn x y sphere)
                 (place-fn (+ x step) y sphere)
                 (place-fn x (+ y step) sphere)
                 (place-fn (+ x step) (+ y step) sphere)))))

(def front-wall
  (let [step wall-step
        place case-place]
    (apply union
           (for [x (range-inclusive 0 (- right-wall-column step) step)]
             (hull (place (- x 1/2) 8.3 wall-sphere-top-front)
                   (place (+ x step) 8.3 wall-sphere-top-front)
                   (place (- x 1/2) 8.3 (wall-sphere-at [0 -25 -24]))
                   (place (+ x step) 8.3 (wall-sphere-at [0 -25 -24])))))))

(def back-wall
  (let [step wall-step
        place case-place]
    (translate [0 -1 -3]
               (rotate (/ π 10) [-1 0 0]
                       (apply union
                              (for [x (range-inclusive left-wall-column (- right-wall-column step) step)]
                                (hull (place x (dec back-y) wall-sphere-top-back)
                                      (place (+ x step) (dec back-y) wall-sphere-top-back)
                                      (place x (dec back-y) wall-sphere-bottom-back)
                                      (place (+ x step) (dec back-y) wall-sphere-bottom-back))))))))

(def left-wall
  (let [place case-place]
    (union
     (apply union
            (map (partial apply hull)
                 (partition 2 1
                            (for [scale (range-inclusive 0 0.75 0.2)]
                              (let [x (scale-to-range 8.3 -3.9 scale)]
                                (hull (place left-wall-column x (wall-sphere-top scale))
                                      (place left-wall-column (* 0.83 x) (wall-sphere-at [0 1 (- -3 (* 2.4 x))])))))))))))

(def right-wall
  (let [place case-place]
    (union
     (apply union
            (map (partial apply hull)
                 (partition 2 1
                            (for [scale (range-inclusive 0 0.75 0.2)]
                              (let [x (scale-to-range 8.3 -3.9 scale)]
                                (hull (place right-wall-column x (wall-sphere-top scale))
                                      (place right-wall-column (* 0.83 x) (wall-sphere-at [0 1 (- -3 (* 2.4 x))])))))))))))

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

(view (union
       key-holes
       ;caps
       front-wall
       back-wall
       right-wall
       left-wall
       bottom-wall))

(view bottom-wall)





;;;;;;;;;;;;
;; Bottom ;;
;;;;;;;;;;;;


(def bottom-key-guard (->> (cube mount-width mount-height web-thickness)
                           (translate [0 0 (+ (- (/ web-thickness 2)) -4.5)])))
(def bottom-front-key-guard (->> (cube mount-width (/ mount-height 2) web-thickness)
                                 (translate [0 (/ mount-height 4) (+ (- (/ web-thickness 2)) -4.5)])))
(defn bottom [height p]
  (->> (project p)
       (extrude-linear {:height height :twist 0 :convexity 0})
       (translate [0 0 (/ height 2)])))

(defn bottom-hull [p]
  (hull p (bottom 1 p)))

(def bottom-plate
  (union
   (apply union
          (for [column columns
                row (drop-last rows) ;;
                :when (or (not= column 0)
                          (not= row 4))]
            (->> bottom-key-guard
                 (key-place column row))))
   (apply union
          (for [column columns
                row [(last rows)] ;;
                :when (or (not= column 0)
                          (not= row 4))]
            (->> bottom-front-key-guard
                 (key-place column row))))
   (let [shift #(translate [0 0 (+ (- web-thickness) -5)] %)
         web-post-tl (shift web-post-tl)
         web-post-tr (shift web-post-tr)
         web-post-br (shift web-post-br)
         web-post-bl (shift web-post-bl)
         half-shift-correction #(translate [0 (/ mount-height 2) 0] %)
         half-post-br (half-shift-correction web-post-br)
         half-post-bl (half-shift-correction web-post-bl)
         row-connections (concat
                          (for [column (drop-last columns)
                                row (drop-last rows)
                                :when (or (not= column 0)
                                          (not= row 4))]
                            (triangle-hulls
                             (key-place (inc column) row web-post-tl)
                             (key-place column row web-post-tr)
                             (key-place (inc column) row web-post-bl)
                             (key-place column row web-post-br)))
                          (for [column (drop-last columns)
                                row [(last rows)]
                                :when (or (not= column 0)
                                          (not= row 4))]
                            (triangle-hulls
                             (key-place (inc column) row web-post-tl)
                             (key-place column row web-post-tr)
                             (key-place (inc column) row half-post-bl)
                             (key-place column row half-post-br))))
         column-connections (for [column columns
                                  row (drop-last rows)
                                  :when (or (not= column 0)
                                            (not= row 3))]
                              (triangle-hulls
                               (key-place column row web-post-bl)
                               (key-place column row web-post-br)
                               (key-place column (inc row) web-post-tl)
                               (key-place column (inc row) web-post-tr)))
         diagonal-connections (for [column (drop-last columns)
                                    row (drop-last rows)
                                    :when (or (not= column 0)
                                              (not= row 3))]
                                (triangle-hulls
                                 (key-place column row web-post-br)
                                 (key-place column (inc row) web-post-tr)
                                 (key-place (inc column) row web-post-bl)
                                 (key-place (inc column) (inc row) web-post-tl)))
         main-keys-bottom (concat row-connections
                                  column-connections
                                  diagonal-connections)
         front-wall (concat
                     (for [x (range 2 5)]
                       (union
                        (hull (case-place (- x 1/2) 4 (translate [0 1 1] wall-sphere-bottom-front))
                              (case-place (+ x 1/2) 4 (translate [0 1 1] wall-sphere-bottom-front))
                              (key-place x 4 half-post-bl)
                              (key-place x 4 half-post-br))
                        (hull (case-place (- x 1/2) 4 (translate [0 1 1] wall-sphere-bottom-front))
                              (key-place x 4 half-post-bl)
                              (key-place (- x 1) 4 half-post-br))))
                     [(hull (case-place right-wall-column 4 (translate [0 1 1] wall-sphere-bottom-front))
                            (case-place (- right-wall-column 1) 4 (translate [0 1 1] wall-sphere-bottom-front))
                            (key-place 5 4 half-post-bl)
                            (key-place 5 4 half-post-br))
                      (hull (case-place (+ 4 1/2) 4 (translate [0 1 1] wall-sphere-bottom-front))
                            (case-place (- right-wall-column 1) 4 (translate [0 1 1] wall-sphere-bottom-front))
                            (key-place 4 4 half-post-br)
                            (key-place 5 4 half-post-bl))])
         right-wall (concat
                     (for [x (range 0 4)]
                       (hull (case-place right-wall-column x (translate [-1 0 1] (wall-sphere-bottom 1/2)))
                             (key-place 5 x web-post-br)
                             (key-place 5 x web-post-tr)))
                     (for [x (range 0 4)]
                       (hull (case-place right-wall-column x (translate [-1 0 1] (wall-sphere-bottom 1/2)))
                             (case-place right-wall-column (inc x) (translate [-1 0 1] (wall-sphere-bottom 1/2)))
                             (key-place 5 x web-post-br)
                             (key-place 5 (inc x) web-post-tr)))
                     [(union
                       (hull (case-place right-wall-column 0 (translate [-1 0 1] (wall-sphere-bottom 1/2)))
                             (case-place right-wall-column 0.02 (translate [-1 -1 1] (wall-sphere-bottom 1)))
                             (key-place 5 0 web-post-tr))
                       (hull (case-place right-wall-column 4 (translate [-1 0 1] (wall-sphere-bottom 1/2)))
                             (case-place right-wall-column 4 (translate [0 1 1] (wall-sphere-bottom 0)))
                             (key-place 5 4 half-post-br))
                       (hull (case-place right-wall-column 4 (translate [-1 0 1] (wall-sphere-bottom 1/2)))
                             (key-place 5 4 half-post-br)
                             (key-place 5 4 web-post-tr)))])
         back-wall (concat
                    (for [x (range 1 6)]
                      (union
                       (hull (case-place (- x 1/2) 0 (translate [0 -1 1] wall-sphere-bottom-back))
                             (case-place (+ x 1/2) 0 (translate [0 -1 1] wall-sphere-bottom-back))
                             (key-place x 0 web-post-tl)
                             (key-place x 0 web-post-tr))
                       (hull (case-place (- x 1/2) 0 (translate [0 -1 1] wall-sphere-bottom-back))
                             (key-place x 0 web-post-tl)
                             (key-place (- x 1) 0 web-post-tr))))
                    [(hull (case-place left-wall-column 0 (translate [1 -1 1] wall-sphere-bottom-back))
                           (case-place (+ left-wall-column 1) 0  (translate [0 -1 1] wall-sphere-bottom-back))
                           (key-place 0 0 web-post-tl)
                           (key-place 0 0 web-post-tr))])
         left-wall (let [place case-place]
                     [(hull (place left-wall-column 0 (translate [1 -1 1] wall-sphere-bottom-back))
                            (place left-wall-column 1 (translate [1 0 1] wall-sphere-bottom-back))
                            (key-place 0 0 web-post-tl)
                            (key-place 0 0 web-post-bl))
                      (hull (place left-wall-column 1 (translate [1 0 1] wall-sphere-bottom-back))
                            (place left-wall-column 2 (translate [1 0 1] wall-sphere-bottom-back))
                            (key-place 0 0 web-post-bl)
                            (key-place 0 1 web-post-bl))
                      (hull (place left-wall-column 2 (translate [1 0 1] wall-sphere-bottom-back))
                            (place left-wall-column 1.6666  (translate [1 0 1] wall-sphere-bottom-front))
                            (key-place 0 1 web-post-bl)
                            (key-place 0 2 web-post-bl))
                      (hull (place left-wall-column 1.6666  (translate [1 0 1] wall-sphere-bottom-front))
                            (key-place 0 2 web-post-bl)
                            (key-place 0 3 web-post-tl))])



         stands (let [bumper-diameter 9.6
                      bumper-radius (/ bumper-diameter 2)
                      stand-diameter (+ bumper-diameter 2)
                      stand-radius (/ stand-diameter 2)
                      stand-at #(difference (->> (sphere stand-radius)
                                                 (translate [0 0 (+ (/ stand-radius -2) -4.5)])
                                                 %
                                                 (bottom-hull))
                                            (->> (cube stand-diameter stand-diameter stand-radius)
                                                 (translate [0 0 (/ stand-radius -2)])
                                                 %)
                                            (->> (sphere bumper-radius)
                                                 (translate [0 0 (+ (/ stand-radius -2) -4.5)])
                                                 %
                                                 (bottom 1.5)))]
                  [(stand-at #(key-place 0 1 %))
                   (stand-at #(key-place 5 0 %))
                   (stand-at #(key-place 5 3 %))])]
     (apply union
            (concat
             main-keys-bottom
             front-wall
             right-wall
             back-wall
             left-wall
             stands)))))

(def screw-hole (->> (cylinder 1.5 60)
                     (translate [0 0 3/2])
                     (with-fn wall-sphere-n)))

(def screw-holes
  (union
   (key-place (+ 4 1/2) 1/2 screw-hole)
   (key-place (+ 4 1/2) (+ 3 1/2) screw-hole)))

(defn circuit-cover [width length height]
  (let [cover-sphere-radius 1
        cover-sphere (->> (sphere cover-sphere-radius)
                          (with-fn 20))
        cover-sphere-z (+ (- height) (- cover-sphere-radius))
        cover-sphere-x (+ (/ width 2) cover-sphere-radius)
        cover-sphere-y (+ (/ length 2) (+ cover-sphere-radius))
        cover-sphere-tl (->> cover-sphere
                             (translate [(- cover-sphere-x) (- cover-sphere-y) cover-sphere-z])
                             (key-place 1/2 3/2))
        cover-sphere-tr (->> cover-sphere
                             (translate [cover-sphere-x (- cover-sphere-y) cover-sphere-z])
                             (key-place 1/2 3/2))
        cover-sphere-br (->> cover-sphere
                             (translate [cover-sphere-x cover-sphere-y cover-sphere-z])
                             (key-place 1/2 3/2))
        cover-sphere-bl (->> cover-sphere
                             (translate [(- cover-sphere-x) cover-sphere-y cover-sphere-z])
                             (key-place 1/2 3/2))

        lower-to-bottom #(translate [0 0 (+ (- cover-sphere-radius) -5.5)] %)
        bl (->> cover-sphere lower-to-bottom (key-place 0 1/2))
        br (->> cover-sphere lower-to-bottom (key-place 1 1/2))
        tl (->> cover-sphere lower-to-bottom (key-place 0 5/2))
        tr (->> cover-sphere lower-to-bottom (key-place 1 5/2))

        mlb (->> cover-sphere
                 (translate [(- cover-sphere-x) 0 (+ (- height) -1)])
                 (key-place 1/2 3/2))
        mrb (->> cover-sphere
                 (translate [cover-sphere-x 0 (+ (- height) -1)])
                 (key-place 1/2 3/2))

        mlt (->> cover-sphere
                 (translate [(+ (- cover-sphere-x) -4) 0 -6])
                 (key-place 1/2 3/2))
        mrt (->> cover-sphere
                 (translate [(+ cover-sphere-x 4) 0 -6])
                 (key-place 1/2 3/2))]
    (union
     (hull cover-sphere-bl cover-sphere-br cover-sphere-tl cover-sphere-tr)
     (hull cover-sphere-br cover-sphere-bl bl br)
     (hull cover-sphere-tr cover-sphere-tl tl tr)
     (hull cover-sphere-tl tl mlb mlt)
     (hull cover-sphere-bl bl mlb mlt)
     (hull cover-sphere-tr tr mrb mrt)
     (hull cover-sphere-br br mrb mrt))))

(def io-exp-width 10)
(def io-exp-height 8)
(def io-exp-length 36)

(def teensy-width 20)
(def teensy-height 12)
(def teensy-length 33)

(def io-exp-cover (circuit-cover io-exp-width io-exp-length io-exp-height))
(def teensy-cover (circuit-cover teensy-width teensy-length teensy-height))

(def trrs-diameter 6.6)
(def trrs-radius (/ trrs-diameter 2))
(def trrs-hole-depth 10)

(def trrs-hole (->> (union (cylinder trrs-radius trrs-hole-depth)
                           (->> (cube trrs-diameter (+ trrs-radius 5) trrs-hole-depth)
                                (translate [0 (/ (+ trrs-radius 5) 2) 0])))
                    (rotate (/ π 2) [1 0 0])
                    (translate [0 (+ (/ mount-height 2) 4) (- trrs-radius)])
                    (with-fn 50)))

(def trrs-hole-just-circle
  (->> (cylinder trrs-radius trrs-hole-depth)
       (rotate (/ π 2) [1 0 0])
       (translate [0 (+ (/ mount-height 2) 4) (- trrs-radius)])
       (with-fn 50)
       (key-place 1/2 0)))

(def trrs-box-hole (->> (cube 14 14 7)
                        (translate [0 1 -3.5])))


(def trrs-cutout
  (->> (union trrs-hole
              trrs-box-hole)
       (key-place 1/2 0)))

(def teensy-pcb-thickness 1.6)
(def teensy-offset-height 5)

(def teensy-pcb (->> (cube 18 30.5 teensy-pcb-thickness)
                     (translate [0 0 (+ (/ teensy-pcb-thickness -2) (- teensy-offset-height))])
                     (key-place 1/2 3/2)
                     (color [1 0 0])))

(def teensy-support
  (difference
   (union
    (->> (cube 3 3 9)
         (translate [0 0 -2])
         (key-place 1/2 3/2)
         (color [0 1 0]))
    (hull (->> (cube 3 6 9)
               (translate [0 0 -2])
               (key-place 1/2 2)
               (color [0 0 1]))
          (->> (cube 3 3 (+ teensy-pcb-thickness 3))
               (translate [0 (/ 30.5 -2) (+ (- teensy-offset-height)
                                            #_(/ (+ teensy-pcb-thickness 3) -2))])
               (key-place 1/2 3/2)
               (color [0 0 1]))))
   teensy-pcb
   (->> (cube 18 30.5 teensy-pcb-thickness)
        (translate [0 1.5 (+ (/ teensy-pcb-thickness -2) (- teensy-offset-height) -1)])
        (key-place 1/2 3/2)
        (color [1 0 0]))))

(def usb-cutout
  (let [hole-height 6.2
        side-radius (/ hole-height 2)
        hole-width 10.75
        side-cylinder (->> (cylinder side-radius teensy-length)
                           (with-fn 20)
                           (translate [(/ (- hole-width hole-height) 2) 0 0]))]
    (->> (hull side-cylinder
               (mirror [-1 0 0] side-cylinder))
         (rotate (/ π 2) [1 0 0])
         (translate [0 (/ teensy-length 2) (- side-radius)])
         (translate [0 0 (- 1)])
         (translate [0 0 (- teensy-offset-height)])
         (key-place 1/2 3/2))))

;;;;;;;;;;;;;;;;;;
;; Final Export ;;
;;;;;;;;;;;;;;;;;;

(def dactyl-bottom-right
  (difference
   (union
    teensy-cover
    (difference
     bottom-plate
     (hull teensy-cover)
     teensy-cover
     trrs-cutout
     (->> (cube 1000 1000 10) (translate [0 0 -5]))
     screw-holes))
   usb-cutout))

(def dactyl-bottom-left
  (mirror [-1 0 0]
          (union
           io-exp-cover
           (difference
            bottom-plate
            (hull io-exp-cover)
            io-exp-cover
            trrs-cutout
            (->> (cube 1000 1000 10) (translate [0 0 -5]))
            screw-holes))))

(def dactyl-top-right
  (difference
   (union key-holes
          connectors
          teensy-support)
   trrs-hole-just-circle
   screw-holes))

(def dactyl-top-left
  (mirror [-1 0 0]
          (difference
           (union key-holes
                  connectors)
           trrs-hole-just-circle
           screw-holes)))
