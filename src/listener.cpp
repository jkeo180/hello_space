#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "hello_space/gap_checker.hpp"

using namespace std::chrono_literals;

class Listener : public rclcpp::Node
{
public:
  Listener() : Node("listener")
  {
    sub_ = create_subscription<std_msgs::msg::String>(
      "chatter", 10,
      [this](const std_msgs::msg::String::SharedPtr msg) {
        double now = steady_now();
        if (silent_) {
          RCLCPP_INFO(get_logger(), "Talker is back after %.3f s", now - last_rx_);
          silent_ = false;
        } else if (have_msg_ && hello_space::is_gap_too_long(last_rx_, now, max_gap_s_)) {
          RCLCPP_WARN(get_logger(), "Late message: %.3f s since previous", now - last_rx_);
        }
        last_rx_ = now;
        have_msg_ = true;
        RCLCPP_INFO(get_logger(), "I heard: '%s'", msg->data.c_str());
      });

    watchdog_ = create_wall_timer(250ms, [this]() {
      if (have_msg_ && !silent_ &&
          hello_space::is_gap_too_long(last_rx_, steady_now(), max_gap_s_)) {
        RCLCPP_ERROR(get_logger(), "WATCHDOG: no message for over %.1f s", max_gap_s_);
        silent_ = true;
      }
    });
  }

private:
  static double steady_now()
  {
    return std::chrono::duration<double>(
      std::chrono::steady_clock::now().time_since_epoch()).count();
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr watchdog_;
  double last_rx_ = 0.0;
  double max_gap_s_ = 1.5;
  bool have_msg_ = false;
  bool silent_ = false;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}
