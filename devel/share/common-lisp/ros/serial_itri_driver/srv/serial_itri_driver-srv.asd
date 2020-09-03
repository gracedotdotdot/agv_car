
(cl:in-package :asdf)

(defsystem "serial_itri_driver-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "command_key" :depends-on ("_package_command_key"))
    (:file "_package_command_key" :depends-on ("_package"))
  ))