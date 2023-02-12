; Auto-generated. Do not edit!


(cl:in-package navigation_controller-srv)


;//! \htmlinclude command-request.msg.html

(cl:defclass <command-request> (roslisp-msg-protocol:ros-message)
  ((type
    :reader type
    :initarg :type
    :type cl:integer
    :initform 0)
   (x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (theta
    :reader theta
    :initarg :theta
    :type cl:float
    :initform 0.0)
   (is_nav
    :reader is_nav
    :initarg :is_nav
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass command-request (<command-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name navigation_controller-srv:<command-request> is deprecated: use navigation_controller-srv:command-request instead.")))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <command-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:type-val is deprecated.  Use navigation_controller-srv:type instead.")
  (type m))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <command-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:x-val is deprecated.  Use navigation_controller-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <command-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:y-val is deprecated.  Use navigation_controller-srv:y instead.")
  (y m))

(cl:ensure-generic-function 'theta-val :lambda-list '(m))
(cl:defmethod theta-val ((m <command-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:theta-val is deprecated.  Use navigation_controller-srv:theta instead.")
  (theta m))

(cl:ensure-generic-function 'is_nav-val :lambda-list '(m))
(cl:defmethod is_nav-val ((m <command-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:is_nav-val is deprecated.  Use navigation_controller-srv:is_nav instead.")
  (is_nav m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command-request>) ostream)
  "Serializes a message object of type '<command-request>"
  (cl:let* ((signed (cl:slot-value msg 'type)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'theta))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_nav) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command-request>) istream)
  "Deserializes a message object of type '<command-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'type) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'theta) (roslisp-utils:decode-double-float-bits bits)))
    (cl:setf (cl:slot-value msg 'is_nav) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command-request>)))
  "Returns string type for a service object of type '<command-request>"
  "navigation_controller/commandRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command-request)))
  "Returns string type for a service object of type 'command-request"
  "navigation_controller/commandRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command-request>)))
  "Returns md5sum for a message object of type '<command-request>"
  "f75d6c7ceee1c8023f1b356191a18d49")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command-request)))
  "Returns md5sum for a message object of type 'command-request"
  "f75d6c7ceee1c8023f1b356191a18d49")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command-request>)))
  "Returns full string definition for message of type '<command-request>"
  (cl:format cl:nil "int64 type~%float64 x~%float64 y~%float64 theta~%bool is_nav~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command-request)))
  "Returns full string definition for message of type 'command-request"
  (cl:format cl:nil "int64 type~%float64 x~%float64 y~%float64 theta~%bool is_nav~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command-request>))
  (cl:+ 0
     8
     8
     8
     8
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command-request>))
  "Converts a ROS message object to a list"
  (cl:list 'command-request
    (cl:cons ':type (type msg))
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':theta (theta msg))
    (cl:cons ':is_nav (is_nav msg))
))
;//! \htmlinclude command-response.msg.html

(cl:defclass <command-response> (roslisp-msg-protocol:ros-message)
  ((is_running
    :reader is_running
    :initarg :is_running
    :type cl:boolean
    :initform cl:nil)
   (run_completed
    :reader run_completed
    :initarg :run_completed
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass command-response (<command-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name navigation_controller-srv:<command-response> is deprecated: use navigation_controller-srv:command-response instead.")))

(cl:ensure-generic-function 'is_running-val :lambda-list '(m))
(cl:defmethod is_running-val ((m <command-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:is_running-val is deprecated.  Use navigation_controller-srv:is_running instead.")
  (is_running m))

(cl:ensure-generic-function 'run_completed-val :lambda-list '(m))
(cl:defmethod run_completed-val ((m <command-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:run_completed-val is deprecated.  Use navigation_controller-srv:run_completed instead.")
  (run_completed m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command-response>) ostream)
  "Serializes a message object of type '<command-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_running) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'run_completed) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command-response>) istream)
  "Deserializes a message object of type '<command-response>"
    (cl:setf (cl:slot-value msg 'is_running) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'run_completed) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command-response>)))
  "Returns string type for a service object of type '<command-response>"
  "navigation_controller/commandResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command-response)))
  "Returns string type for a service object of type 'command-response"
  "navigation_controller/commandResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command-response>)))
  "Returns md5sum for a message object of type '<command-response>"
  "f75d6c7ceee1c8023f1b356191a18d49")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command-response)))
  "Returns md5sum for a message object of type 'command-response"
  "f75d6c7ceee1c8023f1b356191a18d49")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command-response>)))
  "Returns full string definition for message of type '<command-response>"
  (cl:format cl:nil "bool is_running~%bool run_completed~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command-response)))
  "Returns full string definition for message of type 'command-response"
  (cl:format cl:nil "bool is_running~%bool run_completed~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command-response>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command-response>))
  "Converts a ROS message object to a list"
  (cl:list 'command-response
    (cl:cons ':is_running (is_running msg))
    (cl:cons ':run_completed (run_completed msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'command)))
  'command-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'command)))
  'command-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command)))
  "Returns string type for a service object of type '<command>"
  "navigation_controller/command")