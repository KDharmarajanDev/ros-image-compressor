#pragma once
#include <string>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <ros_h264_streamer/h264_decoder.h>
#include <image_compressor/EncodedImage.h>

class TopicUncompressor {
private:
    image_transport::ImageTransport it;
    ros::Subscriber sub;
    image_transport::Publisher originalImagePublisher;
    ros_h264_streamer::H264Decoder *decoder;

public:
    TopicUncompressor (std::string const & topicName, ros::NodeHandle & nodeHandle);

    ~TopicUncompressor() {
        if (decoder) {
            delete decoder;
        }
    }
    void receiveImageCallback(image_compressor::EncodedImage const & msg);
};