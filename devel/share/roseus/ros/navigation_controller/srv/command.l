;; Auto-generated. Do not edit!


(when (boundp 'navigation_controller::command)
  (if (not (find-package "NAVIGATION_CONTROLLER"))
    (make-package "NAVIGATION_CONTROLLER"))
  (shadow 'command (find-package "NAVIGATION_CONTROLLER")))
(unless (find-package "NAVIGATION_CONTROLLER::COMMAND")
  (make-package "NAVIGATION_CONTROLLER::COMMAND"))
(unless (find-package "NAVIGATION_CONTROLLER::COMMANDREQUEST")
  (make-package "NAVIGATION_CONTROLLER::COMMANDREQUEST"))
(unless (find-package "NAVIGATION_CONTROLLER::COMMANDRESPONSE")
  (make-package "NAVIGATION_CONTROLLER::COMMANDRESPONSE"))

(in-package "ROS")





(defclass navigation_controller::commandRequest
  :super ros::object
  :slots (_type _x _y _theta _is_nav ))

(defmethod navigation_controller::commandRequest
  (:init
   (&key
    ((:type __type) 0)
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:theta __theta) 0.0)
    ((:is_nav __is_nav) nil)
    )
   (send-super :init)
   (setq _type (round __type))
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _theta (float __theta))
   (setq _is_nav __is_nav)
   self)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:theta
   (&optional __theta)
   (if __theta (setq _theta __theta)) _theta)
  (:is_nav
   (&optional __is_nav)
   (if __is_nav (setq _is_nav __is_nav)) _is_nav)
  (:serialization-length
   ()
   (+
    ;; int64 _type
    8
    ;; float64 _x
    8
    ;; float64 _y
    8
    ;; float64 _theta
    8
    ;; bool _is_nav
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int64 _type
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _type (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _type) (= (length (_type . bv)) 2)) ;; bignum
              (write-long (ash (elt (_type . bv) 0) 0) s)
              (write-long (ash (elt (_type . bv) 1) -1) s))
             ((and (class _type) (= (length (_type . bv)) 1)) ;; big1
              (write-long (elt (_type . bv) 0) s)
              (write-long (if (>= _type 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _type s)(write-long (if (>= _type 0) 0 #xffffffff) s)))
     ;; float64 _x
       (sys::poke _x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _y
       (sys::poke _y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _theta
       (sys::poke _theta (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; bool _is_nav
       (if _is_nav (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int64 _type
#+(or :alpha :irix6 :x86_64)
      (setf _type (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _type (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; float64 _x
     (setq _x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _y
     (setq _y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _theta
     (setq _theta (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; bool _is_nav
     (setq _is_nav (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass navigation_controller::commandResponse
  :super ros::object
  :slots (_is_running _run_completed ))

(defmethod navigation_controller::commandResponse
  (:init
   (&key
    ((:is_running __is_running) nil)
    ((:run_completed __run_completed) nil)
    )
   (send-super :init)
   (setq _is_running __is_running)
   (setq _run_completed __run_completed)
   self)
  (:is_running
   (&optional __is_running)
   (if __is_running (setq _is_running __is_running)) _is_running)
  (:run_completed
   (&optional __run_completed)
   (if __run_completed (setq _run_completed __run_completed)) _run_completed)
  (:serialization-length
   ()
   (+
    ;; bool _is_running
    1
    ;; bool _run_completed
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _is_running
       (if _is_running (write-byte -1 s) (write-byte 0 s))
     ;; bool _run_completed
       (if _run_completed (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _is_running
     (setq _is_running (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _run_completed
     (setq _run_completed (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass navigation_controller::command
  :super ros::object
  :slots ())

(setf (get navigation_controller::command :md5sum-) "f75d6c7ceee1c8023f1b356191a18d49")
(setf (get navigation_controller::command :datatype-) "navigation_controller/command")
(setf (get navigation_controller::command :request) navigation_controller::commandRequest)
(setf (get navigation_controller::command :response) navigation_controller::commandResponse)

(defmethod navigation_controller::commandRequest
  (:response () (instance navigation_controller::commandResponse :init)))

(setf (get navigation_controller::commandRequest :md5sum-) "f75d6c7ceee1c8023f1b356191a18d49")
(setf (get navigation_controller::commandRequest :datatype-) "navigation_controller/commandRequest")
(setf (get navigation_controller::commandRequest :definition-)
      "int64 type
float64 x
float64 y
float64 theta
bool is_nav
---
bool is_running
bool run_completed


")

(setf (get navigation_controller::commandResponse :md5sum-) "f75d6c7ceee1c8023f1b356191a18d49")
(setf (get navigation_controller::commandResponse :datatype-) "navigation_controller/commandResponse")
(setf (get navigation_controller::commandResponse :definition-)
      "int64 type
float64 x
float64 y
float64 theta
bool is_nav
---
bool is_running
bool run_completed


")



(provide :navigation_controller/command "f75d6c7ceee1c8023f1b356191a18d49")


