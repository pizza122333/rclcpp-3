#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <memory>
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

// 타이머가 호출하는 콜백함수
void callback(
    rclcpp::Node::SharedPtr node,
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mypub)
{
    auto message = std_msgs::msg::String();
    message.data = "Hello world!";

    RCLCPP_INFO(node->get_logger(),
                "Publish: %s", message.data.c_str());

    mypub->publish(message);
}

int main(int argc, char* argv[])
{
    // ROS2 초기화
    rclcpp::init(argc, argv);

    // Publisher 노드 생성
    auto node = std::make_shared<rclcpp::Node>("node_pub1_1");

    // QoS 설정
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

    // Publisher 생성
    auto pub = node->create_publisher<std_msgs::msg::String>(
        "topic_pub1_1", qos_profile);

    // 콜백함수 연결
    std::function<void()> fn = std::bind(callback, node, pub);

    // 100ms마다 콜백함수 실행
    auto timer = node->create_wall_timer(100ms, fn);

    // 타이머 이벤트 처리
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
