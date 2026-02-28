#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// 使用占位符，这是 std::bind 的灵魂
using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber()
  : Node("minimal_subscriber") // 节点名称
  {
    // 步骤 1: 创建订阅者
    // <std_msgs::msg::String> 指定接收的消息类型
    // 参数分别是：话题名 "topic"、队列深度 10、以及收到消息后的回调函数
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
  }

private:
  // 步骤 2: 定义回调函数
  // 这里的参数必须是消息类型的 SharedPtr（或其变体）
  void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
  {
    // 打印接收到的消息内容
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
  }

  // 变量声明：订阅者的智能指针
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  // 步骤 3: 同样使用 spin 让节点保持存活并监听
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}