#pragma once
#include <string>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <ros_h264_streamer/h264_encoder.h>


class TopicCompressor {
private:
    image_transport::ImageTransport it;
    image_transport::Subscriber sub;
    image_transport::Publisher compressPub;
    ros_h264_streamer::H264Encoder *encoder;

public:
    TopicCompressor (std::string const & topicName, ros::NodeHandle const & nodeHandle);

    ~TopicCompressor() {
        if (encoder) {
            delete encoder;
        }
    }
    void receiveImageCallback(sensor_msgs::ImageConstPtr const & msg);
};