;; Auto-generated. Do not edit!


(when (boundp 'serial_itri_driver::command_key)
  (if (not (find-package "SERIAL_ITRI_DRIVER"))
    (make-package "SERIAL_ITRI_DRIVER"))
  (shadow 'command_key (find-package "SERIAL_ITRI_DRIVER")))
(unless (find-package "SERIAL_ITRI_DRIVER::COMMAND_KEY")
  (make-package "SERIAL_ITRI_DRIVER::COMMAND_KEY"))
(unless (find-package "SERIAL_ITRI_DRIVER::COMMAND_KEYREQUEST")
  (make-package "SERIAL_ITRI_DRIVER::COMMAND_KEYREQUEST"))
(unless (find-package "SERIAL_ITRI_DRIVER::COMMAND_KEYRESPONSE")
  (make-package "SERIAL_ITRI_DRIVER::COMMAND_KEYRESPONSE"))

(in-package "ROS")





(defclass serial_itri_driver::command_keyRequest
  :super ros::object
  :slots (_init ))

(defmethod serial_itri_driver::command_keyRequest
  (:init
   (&key
    ((:init __init) nil)
    )
   (send-super :init)
   (setq _init __init)
   self)
  (:init
   (&optional __init)
   (if __init (setq _init __init)) _init)
  (:serialization-length
   ()
   (+
    ;; bool _init
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _init
       (if _init (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _init
     (setq _init (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass serial_itri_driver::command_keyResponse
  :super ros::object
  :slots (_ans ))

(defmethod serial_itri_driver::command_keyResponse
  (:init
   (&key
    ((:ans __ans) nil)
    )
   (send-super :init)
   (setq _ans __ans)
   self)
  (:ans
   (&optional __ans)
   (if __ans (setq _ans __ans)) _ans)
  (:serialization-length
   ()
   (+
    ;; bool _ans
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _ans
       (if _ans (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _ans
     (setq _ans (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass serial_itri_driver::command_key
  :super ros::object
  :slots ())

(setf (get serial_itri_driver::command_key :md5sum-) "599c74f7b589131c348db47ed14be887")
(setf (get serial_itri_driver::command_key :datatype-) "serial_itri_driver/command_key")
(setf (get serial_itri_driver::command_key :request) serial_itri_driver::command_keyRequest)
(setf (get serial_itri_driver::command_key :response) serial_itri_driver::command_keyResponse)

(defmethod serial_itri_driver::command_keyRequest
  (:response () (instance serial_itri_driver::command_keyResponse :init)))

(setf (get serial_itri_driver::command_keyRequest :md5sum-) "599c74f7b589131c348db47ed14be887")
(setf (get serial_itri_driver::command_keyRequest :datatype-) "serial_itri_driver/command_keyRequest")
(setf (get serial_itri_driver::command_keyRequest :definition-)
      "bool init
---
bool ans
")

(setf (get serial_itri_driver::command_keyResponse :md5sum-) "599c74f7b589131c348db47ed14be887")
(setf (get serial_itri_driver::command_keyResponse :datatype-) "serial_itri_driver/command_keyResponse")
(setf (get serial_itri_driver::command_keyResponse :definition-)
      "bool init
---
bool ans
")



(provide :serial_itri_driver/command_key "599c74f7b589131c348db47ed14be887")


