#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <memory>
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

// 타이머가 호출하는 콜백함수
void callback(
    rclcpp::Node::SharedPtr node,
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr mypub)
{
    static int count = 0;

    // Int32 메시지 생성
    auto message = std_msgs::msg::Int32();

    // 0부터 1씩 증가
    message.data = count++;

    // 발행 내용 출력
    RCLCPP_INFO(node->get_logger(),
                "Publish: %d", message.data);

    // 메시지 발행
    mypub->publish(message);
}

int main(int argc, char* argv[])
{
    // ROS2 초기화
    rclcpp::init(argc, argv);

    // Publisher 노드 생성
    auto node = std::make_shared<rclcpp::Node>("node_pub1_2");

    // QoS 설정
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

    // Int32 Publisher 생성
    auto pub = node->create_publisher<std_msgs::msg::Int32>(
        "topic_pub1_2", qos_profile);

    // 콜백함수 연결
    std::function<void()> fn = std::bind(callback, node, pub);

    // 50ms 주기로 타이머 실행
    auto timer = node->create_wall_timer(50ms, fn);

    // 타이머 이벤트 처리
    rclcpp::spin(node);

    // ROS2 종료
    rclcpp::shutdown();

    return 0;
}
