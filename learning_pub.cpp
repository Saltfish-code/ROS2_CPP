#include <chrono>    // 处理时间
#include <functional>// 处理 std::bind
#include <memory>    // 处理智能指针
#include <string>    // 处理字符串

#include "rclcpp/rclcpp.hpp"        // ROS 2 核心库
#include "std_msgs/msg/string.hpp"  // 标准字符串消息类型

using namespace std::chrono_literals;

/* 创建一个继承自 rclcpp::Node 的类 */
class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0) // 初始化节点名称为 "minimal_publisher"
  {
    // 步骤 1: 创建发布者
    // 泛型参数是消息类型，参数分别是：话题名 "topic" 和 队列深度 10
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);

    // 步骤 2: 创建定时器
    // 每 500ms 执行一次 timer_callback 函数
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  // 步骤 3: 定义定时器回调函数
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++);
    
    // 打印日志到终端
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    
    // 发布消息
    publisher_->publish(message);
  }

  // 变量声明
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  // 步骤 4: 初始化 ROS 2 并在退出前保持运行
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}