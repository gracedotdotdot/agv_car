; Auto-generated. Do not edit!


(cl:in-package serial_itri_driver-srv)


;//! \htmlinclude command_key-request.msg.html

(cl:defclass <command_key-request> (roslisp-msg-protocol:ros-message)
  ((init
    :reader init
    :initarg :init
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass command_key-request (<command_key-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command_key-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command_key-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name serial_itri_driver-srv:<command_key-request> is deprecated: use serial_itri_driver-srv:command_key-request instead.")))

(cl:ensure-generic-function 'init-val :lambda-list '(m))
(cl:defmethod init-val ((m <command_key-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader serial_itri_driver-srv:init-val is deprecated.  Use serial_itri_driver-srv:init instead.")
  (init m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command_key-request>) ostream)
  "Serializes a message object of type '<command_key-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'init) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command_key-request>) istream)
  "Deserializes a message object of type '<command_key-request>"
    (cl:setf (cl:slot-value msg 'init) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command_key-request>)))
  "Returns string type for a service object of type '<command_key-request>"
  "serial_itri_driver/command_keyRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_key-request)))
  "Returns string type for a service object of type 'command_key-request"
  "serial_itri_driver/command_keyRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command_key-request>)))
  "Returns md5sum for a message object of type '<command_key-request>"
  "599c74f7b589131c348db47ed14be887")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command_key-request)))
  "Returns md5sum for a message object of type 'command_key-request"
  "599c74f7b589131c348db47ed14be887")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command_key-request>)))
  "Returns full string definition for message of type '<command_key-request>"
  (cl:format cl:nil "bool init~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command_key-request)))
  "Returns full string definition for message of type 'command_key-request"
  (cl:format cl:nil "bool init~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command_key-request>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command_key-request>))
  "Converts a ROS message object to a list"
  (cl:list 'command_key-request
    (cl:cons ':init (init msg))
))
;//! \htmlinclude command_key-response.msg.html

(cl:defclass <command_key-response> (roslisp-msg-protocol:ros-message)
  ((ans
    :reader ans
    :initarg :ans
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass command_key-response (<command_key-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command_key-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command_key-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name serial_itri_driver-srv:<command_key-response> is deprecated: use serial_itri_driver-srv:command_key-response instead.")))

(cl:ensure-generic-function 'ans-val :lambda-list '(m))
(cl:defmethod ans-val ((m <command_key-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader serial_itri_driver-srv:ans-val is deprecated.  Use serial_itri_driver-srv:ans instead.")
  (ans m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command_key-response>) ostream)
  "Serializes a message object of type '<command_key-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'ans) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command_key-response>) istream)
  "Deserializes a message object of type '<command_key-response>"
    (cl:setf (cl:slot-value msg 'ans) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command_key-response>)))
  "Returns string type for a service object of type '<command_key-response>"
  "serial_itri_driver/command_keyResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_key-response)))
  "Returns string type for a service object of type 'command_key-response"
  "serial_itri_driver/command_keyResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command_key-response>)))
  "Returns md5sum for a message object of type '<command_key-response>"
  "599c74f7b589131c348db47ed14be887")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command_key-response)))
  "Returns md5sum for a message object of type 'command_key-response"
  "599c74f7b589131c348db47ed14be887")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command_key-response>)))
  "Returns full string definition for message of type '<command_key-response>"
  (cl:format cl:nil "bool ans~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command_key-response)))
  "Returns full string definition for message of type 'command_key-response"
  (cl:format cl:nil "bool ans~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command_key-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command_key-response>))
  "Converts a ROS message object to a list"
  (cl:list 'command_key-response
    (cl:cons ':ans (ans msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'command_key)))
  'command_key-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'command_key)))
  'command_key-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_key)))
  "Returns string type for a service object of type '<command_key>"
  "serial_itri_driver/command_key")