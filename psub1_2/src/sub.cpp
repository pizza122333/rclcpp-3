#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <memory>
#include <functional>

// 메시지를 수신하면 실행되는 콜백함수
void mysub_callback(
    rclcpp::Node::SharedPtr node,
    const std_msgs::msg::Int32::SharedPtr msg)
{
    RCLCPP_INFO(node->get_logger(),
                "Received message: %d",
                msg->data);
}

int main(int argc, char* argv[])
{
    // ROS2 초기화
    rclcpp::init(argc, argv);

    // Subscriber 노드 생성
    auto node = std::make_shared<rclcpp::Node>("node_sub1_2");

    // QoS 설정
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

    // 콜백함수 연결
    std::function<void(const std_msgs::msg::Int32::SharedPtr)> fn =
        std::bind(mysub_callback, node, std::placeholders::_1);

    // Int32 Subscriber 생성
    auto mysub = node->create_subscription<std_msgs::msg::Int32>(
        "topic_pub1_2", qos_profile, fn);

    // 메시지 수신 이벤트 처리
    rclcpp::spin(node);

    // ROS2 종료
    rclcpp::shutdown();

    return 0;
}
