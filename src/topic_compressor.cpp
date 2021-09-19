#include <image_compressor/topic_compressor.h>

TopicCompressor::TopicCompressor (std::string const & topicName, ros::NodeHandle const & nodeHandle) : it(nodeHandle) {
    sub = it.subscribe(topicName, 1, &TopicCompressor::receiveImageCallback, this);
    compressPub = it.advertise(topicName + "_compressed", 1);
    encoder = nullptr;
}

void TopicCompressor::receiveImageCallback (sensor_msgs::ImageConstPtr const & msg) {
    if (!encoder) {
        encoder = new ros_h264_streamer::H264Encoder(msg->width, msg->height);
    }
    ros_h264_streamer::H264EncoderResult result = encoder->encode(msg);
    sensor_msgs::ImagePtr outMessage;

    compressPub.publish(outMessage);
}