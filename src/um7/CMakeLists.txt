cmake_minimum_required(VERSION 2.8.3)
project(um7)

find_package(catkin REQUIRED COMPONENTS roscpp serial roslint sensor_msgs 
             message_generation)

add_service_files(
  FILES
  Reset.srv
)

generate_messages()

catkin_package(
   INCLUDE_DIRS include
   CATKIN_DEPENDS message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp executable
add_executable(um7_driver src/main.cpp src/registers.cpp src/comms.cpp)
target_link_libraries(um7_driver ${catkin_LIBRARIES} )
add_dependencies(um7_driver um7_generate_messages_cpp)


#############
## Install ##
#############

install(TARGETS um7_driver
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

##########
## Docs ##
##########

file(GLOB LINT_SRCS
  src/*.cpp
  include/um6/registers.h
  include/um6/comms.h)
roslint_cpp(${LINT_SRCS})
roslint_add_test()

#############
## Testing ##
#############

catkin_add_gtest(${PROJECT_NAME}_test_registers test/test_registers.cpp src/registers.cpp)
catkin_add_gtest(${PROJECT_NAME}_test_comms test/test_comms.cpp src/comms.cpp src/registers.cpp)
if(TARGET ${PROJECT_NAME}_test_comms)
  target_link_libraries(${PROJECT_NAME}_test_comms util ${catkin_LIBRARIES})
endif()
if(TARGET ${PROJECT_NAME}_test_registers)
  target_link_libraries(${PROJECT_NAME}_test_registers util ${catkin_LIBRARIES})
endif()


