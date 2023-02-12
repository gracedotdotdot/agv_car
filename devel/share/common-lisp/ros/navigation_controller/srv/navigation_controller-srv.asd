
(cl:in-package :asdf)

(defsystem "navigation_controller-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "command" :depends-on ("_package_command"))
    (:file "_package_command" :depends-on ("_package"))
    (:file "command_check" :depends-on ("_package_command_check"))
    (:file "_package_command_check" :depends-on ("_package"))
    (:file "command_status" :depends-on ("_package_command_status"))
    (:file "_package_command_status" :depends-on ("_package"))
  ))