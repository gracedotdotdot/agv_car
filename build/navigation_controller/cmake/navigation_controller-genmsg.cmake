# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "navigation_controller: 0 messages, 2 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(navigation_controller_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" NAME_WE)
add_custom_target(_navigation_controller_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "navigation_controller" "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" ""
)

get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" NAME_WE)
add_custom_target(_navigation_controller_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "navigation_controller" "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/navigation_controller
)
_generate_srv_cpp(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/navigation_controller
)

### Generating Module File
_generate_module_cpp(navigation_controller
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/navigation_controller
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(navigation_controller_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(navigation_controller_generate_messages navigation_controller_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_cpp _navigation_controller_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_cpp _navigation_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(navigation_controller_gencpp)
add_dependencies(navigation_controller_gencpp navigation_controller_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS navigation_controller_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages

### Generating Services
_generate_srv_eus(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/navigation_controller
)
_generate_srv_eus(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/navigation_controller
)

### Generating Module File
_generate_module_eus(navigation_controller
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/navigation_controller
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(navigation_controller_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(navigation_controller_generate_messages navigation_controller_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_eus _navigation_controller_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_eus _navigation_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(navigation_controller_geneus)
add_dependencies(navigation_controller_geneus navigation_controller_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS navigation_controller_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/navigation_controller
)
_generate_srv_lisp(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/navigation_controller
)

### Generating Module File
_generate_module_lisp(navigation_controller
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/navigation_controller
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(navigation_controller_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(navigation_controller_generate_messages navigation_controller_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_lisp _navigation_controller_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_lisp _navigation_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(navigation_controller_genlisp)
add_dependencies(navigation_controller_genlisp navigation_controller_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS navigation_controller_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages

### Generating Services
_generate_srv_nodejs(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/navigation_controller
)
_generate_srv_nodejs(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/navigation_controller
)

### Generating Module File
_generate_module_nodejs(navigation_controller
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/navigation_controller
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(navigation_controller_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(navigation_controller_generate_messages navigation_controller_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_nodejs _navigation_controller_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_nodejs _navigation_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(navigation_controller_gennodejs)
add_dependencies(navigation_controller_gennodejs navigation_controller_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS navigation_controller_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/navigation_controller
)
_generate_srv_py(navigation_controller
  "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/navigation_controller
)

### Generating Module File
_generate_module_py(navigation_controller
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/navigation_controller
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(navigation_controller_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(navigation_controller_generate_messages navigation_controller_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_py _navigation_controller_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/an/Desktop/catkin_ws/src/navigation_controller/srv/command_status.srv" NAME_WE)
add_dependencies(navigation_controller_generate_messages_py _navigation_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(navigation_controller_genpy)
add_dependencies(navigation_controller_genpy navigation_controller_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS navigation_controller_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/navigation_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/navigation_controller
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(navigation_controller_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/navigation_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/navigation_controller
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(navigation_controller_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/navigation_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/navigation_controller
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(navigation_controller_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/navigation_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/navigation_controller
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(navigation_controller_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/navigation_controller)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/navigation_controller\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/navigation_controller
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(navigation_controller_generate_messages_py std_msgs_generate_messages_py)
endif()
