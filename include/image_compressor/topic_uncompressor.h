#pragma once
#include <string>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <ros_h264_streamer/h264_decoder.h>


class TopicUncompressor {
private:
    image_transport::ImageTransport it;
    image_transport::Subscriber sub;
    image_transport::Publisher originalImagePublisher;
    ros_h264_streamer::H264Decoder *decoder;

public:
    TopicUncompressor (std::string const & topicName, ros::NodeHandle const & nodeHandle);

    ~TopicUncompressor() {
        if (decoder) {
            delete decoder;
        }
    }
    void receiveImageCallback(sensor_msgs::ImageConstPtr const & msg);
};