;; Auto-generated. Do not edit!


(when (boundp 'navigation_controller::command_status)
  (if (not (find-package "NAVIGATION_CONTROLLER"))
    (make-package "NAVIGATION_CONTROLLER"))
  (shadow 'command_status (find-package "NAVIGATION_CONTROLLER")))
(unless (find-package "NAVIGATION_CONTROLLER::COMMAND_STATUS")
  (make-package "NAVIGATION_CONTROLLER::COMMAND_STATUS"))
(unless (find-package "NAVIGATION_CONTROLLER::COMMAND_STATUSREQUEST")
  (make-package "NAVIGATION_CONTROLLER::COMMAND_STATUSREQUEST"))
(unless (find-package "NAVIGATION_CONTROLLER::COMMAND_STATUSRESPONSE")
  (make-package "NAVIGATION_CONTROLLER::COMMAND_STATUSRESPONSE"))

(in-package "ROS")





(defclass navigation_controller::command_statusRequest
  :super ros::object
  :slots (_get_status _is_cancel _is_charging ))

(defmethod navigation_controller::command_statusRequest
  (:init
   (&key
    ((:get_status __get_status) nil)
    ((:is_cancel __is_cancel) nil)
    ((:is_charging __is_charging) nil)
    )
   (send-super :init)
   (setq _get_status __get_status)
   (setq _is_cancel __is_cancel)
   (setq _is_charging __is_charging)
   self)
  (:get_status
   (&optional __get_status)
   (if __get_status (setq _get_status __get_status)) _get_status)
  (:is_cancel
   (&optional __is_cancel)
   (if __is_cancel (setq _is_cancel __is_cancel)) _is_cancel)
  (:is_charging
   (&optional __is_charging)
   (if __is_charging (setq _is_charging __is_charging)) _is_charging)
  (:serialization-length
   ()
   (+
    ;; bool _get_status
    1
    ;; bool _is_cancel
    1
    ;; bool _is_charging
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _get_status
       (if _get_status (write-byte -1 s) (write-byte 0 s))
     ;; bool _is_cancel
       (if _is_cancel (write-byte -1 s) (write-byte 0 s))
     ;; bool _is_charging
       (if _is_charging (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _get_status
     (setq _get_status (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _is_cancel
     (setq _is_cancel (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _is_charging
     (setq _is_charging (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass navigation_controller::command_statusResponse
  :super ros::object
  :slots (_status_is ))

(defmethod navigation_controller::command_statusResponse
  (:init
   (&key
    ((:status_is __status_is) "")
    )
   (send-super :init)
   (setq _status_is (string __status_is))
   self)
  (:status_is
   (&optional __status_is)
   (if __status_is (setq _status_is __status_is)) _status_is)
  (:serialization-length
   ()
   (+
    ;; string _status_is
    4 (length _status_is)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _status_is
       (write-long (length _status_is) s) (princ _status_is s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _status_is
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _status_is (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass navigation_controller::command_status
  :super ros::object
  :slots ())

(setf (get navigation_controller::command_status :md5sum-) "33ed9e193eaee162a578945b5dc27255")
(setf (get navigation_controller::command_status :datatype-) "navigation_controller/command_status")
(setf (get navigation_controller::command_status :request) navigation_controller::command_statusRequest)
(setf (get navigation_controller::command_status :response) navigation_controller::command_statusResponse)

(defmethod navigation_controller::command_statusRequest
  (:response () (instance navigation_controller::command_statusResponse :init)))

(setf (get navigation_controller::command_statusRequest :md5sum-) "33ed9e193eaee162a578945b5dc27255")
(setf (get navigation_controller::command_statusRequest :datatype-) "navigation_controller/command_statusRequest")
(setf (get navigation_controller::command_statusRequest :definition-)
      "bool get_status
bool is_cancel
bool is_charging
---
string status_is

")

(setf (get navigation_controller::command_statusResponse :md5sum-) "33ed9e193eaee162a578945b5dc27255")
(setf (get navigation_controller::command_statusResponse :datatype-) "navigation_controller/command_statusResponse")
(setf (get navigation_controller::command_statusResponse :definition-)
      "bool get_status
bool is_cancel
bool is_charging
---
string status_is

")



(provide :navigation_controller/command_status "33ed9e193eaee162a578945b5dc27255")


