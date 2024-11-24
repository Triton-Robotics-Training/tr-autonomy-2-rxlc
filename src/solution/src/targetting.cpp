#include "targetting.h"

const double CAM_WIDTH = 640.0;
const double FOV = 90.0;
const double STILL_THRESHOLD = 55;
const std::array<double, 2> EXTREME_ANGLES = {-1.3, 1.3};

const cv::Scalar LOWER_RED(0, 120, 70);
const cv::Scalar UPPER_RED(10, 255, 255);

Targetting::Targetting() : Node("Targetting") {
    cam_sub = this->create_subscription<sensor_msgs::msg::Image>(
        "robotcam", 10, std::bind(&Targetting::imageCallback, this, std::placeholders::_1));
    
    current_angle_sub = this->create_subscription<Float32Msg>(
        "/current_angle", 10, std::bind(&Targetting::currentAngleCallback, this, std::placeholders::_1));

    desired_angle_pub = this->create_publisher<Float32Msg>(
        "/desired_angle", 10);
    
    current_angle = 0;
}

void Targetting::currentAngleCallback(const Float32Msg::SharedPtr msg) {
    current_angle = msg -> data;
}

void Targetting::imageCallback(const ImageMsg::SharedPtr msg) {

    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat current_image;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, msg -> encoding);
        current_image = cv_ptr->image;
    }
    catch (cv_bridge::Exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat hsv_image;
    cv::cvtColor(current_image, hsv_image, cv::COLOR_BGR2HSV);

    cv::Mat red_mask;
    cv::inRange(hsv_image, LOWER_RED, UPPER_RED, red_mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (!contours.empty()) {
        auto main_countour = std::max_element(contours.begin(), contours.end(),
            [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2) {
                return cv::contourArea(c1) < cv::contourArea(c2);
            });

        cv::Rect boundingRect = cv::boundingRect(*main_countour);
        cv::rectangle(current_image, boundingRect, cv::Scalar(0, 255, 0), 2);

        cv::Point center(boundingRect.x + boundingRect.width / 2, 
                         boundingRect.y + boundingRect.height / 2);
        
        if (abs(center.x - CAM_WIDTH/2) >= STILL_THRESHOLD) {
            double offset_angle_deg = -1.0f * (center.x - CAM_WIDTH/2) * (FOV/CAM_WIDTH);
            double desired_angle = current_angle + (offset_angle_deg * M_PI/180);

            std_msgs::msg::Float32 desired_angle_msg;
            desired_angle_msg.data = desired_angle;
            desired_angle_pub->publish(desired_angle_msg);
        }
    } else {
        std::mt19937 gen(std::time(0));
        std::uniform_int_distribution<> dis(0, 1);

        int randomIndex = dis(gen);

        std_msgs::msg::Float32 desired_angle_msg;
        desired_angle_msg.data = EXTREME_ANGLES[randomIndex];
        desired_angle_pub->publish(desired_angle_msg);
    }

}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Targetting>());
    rclcpp::shutdown();
    return 0;
}