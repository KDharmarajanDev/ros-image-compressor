cmake_minimum_required(VERSION 2.8.3)
project(image_compressor)

if(NOT WIN32)
find_package(catkin REQUIRED COMPONENTS
        roscpp
        image_transport
        message_generation
        sensor_msgs)
include_directories(${catkin_INCLUDE_DIRS})
else()
add_definitions("-D_WIN32_WINNT=0x0501")
include_directories(${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
set(catkin_LIBRARIES "")
set(CATKIN_PACKAGE_INCLUDE_DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
set(CATKIN_PACKAGE_BIN_DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
set(CATKIN_PACKAGE_LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
set(catkin_FOUND 0)
endif()

find_package(Boost REQUIRED COMPONENTS program_options)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_message_files(
        FILES
        EncodedImage.msg
)

generate_messages(
        DEPENDENCIES
        sensor_msgs
)

  catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
    DEPENDS roscpp
  )

  find_package(dynamic_graph_bridge_msgs)
    if(${dynamic_graph_bridge_msgs_FOUND})
      include_directories(${dynamic_graph_bridge_msgs_INCLUDE_DIRS})
      add_definitions("-DHAS_DGBRIDGE_MSGS")
    endif()

if(NOT WIN32)
set(ros_h264_streamer_src
  src/h264_encoder.cpp
  src/h264_decoder.cpp
        )
else()
set(ros_h264_streamer_src
  src/h264_encoder.cpp
  src/h264_decoder.cpp
        )
endif()

add_library(ros_h264_streamer SHARED ${ros_h264_streamer_src})
target_link_libraries(ros_h264_streamer ${catkin_LIBRARIES} x264 avcodec avutil swscale)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)

if(NOT WIN32)
set(SHOULD_INSTALL ros_h264_streamer)
else()
set(SHOULD_INSTALL ros_h264_streamer)
endif()

install(TARGETS ${SHOULD_INSTALL}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

add_executable(image_compressor_main src/image_compressor_main.cpp src/topic_compressor.cpp)
target_link_libraries(image_compressor_main ${catkin_LIBRARIES} ros_h264_streamer)
add_dependencies(image_compressor_main image_compressor_generate_messages_cpp)

add_executable(image_uncompressor_main src/image_uncompressor_main.cpp src/topic_uncompressor.cpp)
target_link_libraries(image_uncompressor_main ${catkin_LIBRARIES} ros_h264_streamer)
add_dependencies(image_uncompressor_main image_uncompressor_generate_messages_cpp)

if (CATKIN_ENABLE_TESTING)
  find_package(roslaunch REQUIRED)
  roslaunch_add_file_check(launch)
endif()
