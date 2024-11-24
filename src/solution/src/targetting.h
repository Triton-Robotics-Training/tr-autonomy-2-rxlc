#ifndef TARGETTING_HPP
#define TARGETTING_HPP

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

#include <stdlib.h>
#include <array>
#include <random>

#include <std_msgs/msg/float32.hpp>

using ImageMsg = sensor_msgs::msg::Image; 
using Float32Msg = std_msgs::msg::Float32; 

class Targetting : public rclcpp::Node {
    public:
        Targetting();

    private:
        void imageCallback(const ImageMsg::SharedPtr msg);
        void currentAngleCallback(const Float32Msg::SharedPtr msg);

        rclcpp::Subscription<ImageMsg>::SharedPtr cam_sub;
        rclcpp::Subscription<Float32Msg>::SharedPtr current_angle_sub;

        rclcpp::Publisher<Float32Msg>::SharedPtr desired_angle_pub;

        double current_angle;
};

#endif // TARGETTING_HPP
