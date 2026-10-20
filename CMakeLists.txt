cmake_minimum_required(VERSION 3.8)
project(hello_space)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(hello_node src/hello_node.cpp)
target_link_libraries(hello_node rclcpp::rclcpp)

add_executable(talker src/talker.cpp)
target_link_libraries(talker rclcpp::rclcpp ${std_msgs_TARGETS})

add_executable(listener src/listener.cpp)
target_link_libraries(listener rclcpp::rclcpp ${std_msgs_TARGETS})
target_include_directories(listener PRIVATE include)

install(TARGETS hello_node talker listener DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_gap_checker test/test_gap_checker.cpp)
  target_include_directories(test_gap_checker PRIVATE include)
endif()

ament_package()
