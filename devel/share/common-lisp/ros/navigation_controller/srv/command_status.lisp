; Auto-generated. Do not edit!


(cl:in-package navigation_controller-srv)


;//! \htmlinclude command_status-request.msg.html

(cl:defclass <command_status-request> (roslisp-msg-protocol:ros-message)
  ((get_status
    :reader get_status
    :initarg :get_status
    :type cl:boolean
    :initform cl:nil)
   (is_cancel
    :reader is_cancel
    :initarg :is_cancel
    :type cl:boolean
    :initform cl:nil)
   (is_charging
    :reader is_charging
    :initarg :is_charging
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass command_status-request (<command_status-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command_status-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command_status-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name navigation_controller-srv:<command_status-request> is deprecated: use navigation_controller-srv:command_status-request instead.")))

(cl:ensure-generic-function 'get_status-val :lambda-list '(m))
(cl:defmethod get_status-val ((m <command_status-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:get_status-val is deprecated.  Use navigation_controller-srv:get_status instead.")
  (get_status m))

(cl:ensure-generic-function 'is_cancel-val :lambda-list '(m))
(cl:defmethod is_cancel-val ((m <command_status-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:is_cancel-val is deprecated.  Use navigation_controller-srv:is_cancel instead.")
  (is_cancel m))

(cl:ensure-generic-function 'is_charging-val :lambda-list '(m))
(cl:defmethod is_charging-val ((m <command_status-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:is_charging-val is deprecated.  Use navigation_controller-srv:is_charging instead.")
  (is_charging m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command_status-request>) ostream)
  "Serializes a message object of type '<command_status-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'get_status) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_cancel) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_charging) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command_status-request>) istream)
  "Deserializes a message object of type '<command_status-request>"
    (cl:setf (cl:slot-value msg 'get_status) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'is_cancel) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'is_charging) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command_status-request>)))
  "Returns string type for a service object of type '<command_status-request>"
  "navigation_controller/command_statusRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_status-request)))
  "Returns string type for a service object of type 'command_status-request"
  "navigation_controller/command_statusRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command_status-request>)))
  "Returns md5sum for a message object of type '<command_status-request>"
  "33ed9e193eaee162a578945b5dc27255")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command_status-request)))
  "Returns md5sum for a message object of type 'command_status-request"
  "33ed9e193eaee162a578945b5dc27255")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command_status-request>)))
  "Returns full string definition for message of type '<command_status-request>"
  (cl:format cl:nil "bool get_status~%bool is_cancel~%bool is_charging~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command_status-request)))
  "Returns full string definition for message of type 'command_status-request"
  (cl:format cl:nil "bool get_status~%bool is_cancel~%bool is_charging~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command_status-request>))
  (cl:+ 0
     1
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command_status-request>))
  "Converts a ROS message object to a list"
  (cl:list 'command_status-request
    (cl:cons ':get_status (get_status msg))
    (cl:cons ':is_cancel (is_cancel msg))
    (cl:cons ':is_charging (is_charging msg))
))
;//! \htmlinclude command_status-response.msg.html

(cl:defclass <command_status-response> (roslisp-msg-protocol:ros-message)
  ((status_is
    :reader status_is
    :initarg :status_is
    :type cl:string
    :initform ""))
)

(cl:defclass command_status-response (<command_status-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command_status-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command_status-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name navigation_controller-srv:<command_status-response> is deprecated: use navigation_controller-srv:command_status-response instead.")))

(cl:ensure-generic-function 'status_is-val :lambda-list '(m))
(cl:defmethod status_is-val ((m <command_status-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader navigation_controller-srv:status_is-val is deprecated.  Use navigation_controller-srv:status_is instead.")
  (status_is m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command_status-response>) ostream)
  "Serializes a message object of type '<command_status-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'status_is))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'status_is))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command_status-response>) istream)
  "Deserializes a message object of type '<command_status-response>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'status_is) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'status_is) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command_status-response>)))
  "Returns string type for a service object of type '<command_status-response>"
  "navigation_controller/command_statusResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_status-response)))
  "Returns string type for a service object of type 'command_status-response"
  "navigation_controller/command_statusResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command_status-response>)))
  "Returns md5sum for a message object of type '<command_status-response>"
  "33ed9e193eaee162a578945b5dc27255")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command_status-response)))
  "Returns md5sum for a message object of type 'command_status-response"
  "33ed9e193eaee162a578945b5dc27255")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command_status-response>)))
  "Returns full string definition for message of type '<command_status-response>"
  (cl:format cl:nil "string status_is~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command_status-response)))
  "Returns full string definition for message of type 'command_status-response"
  (cl:format cl:nil "string status_is~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command_status-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'status_is))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command_status-response>))
  "Converts a ROS message object to a list"
  (cl:list 'command_status-response
    (cl:cons ':status_is (status_is msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'command_status)))
  'command_status-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'command_status)))
  'command_status-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_status)))
  "Returns string type for a service object of type '<command_status>"
  "navigation_controller/command_status")