#include <image_compressor/topic_uncompressor.h>

TopicUncompressor::TopicUncompressor (std::string const & topicName, ros::NodeHandle & nodeHandle) : it(nodeHandle) {
    sub = nodeHandle.subscribe(topicName + "_compressed", 1, &TopicUncompressor::receiveImageCallback, this);
    originalImagePublisher = it.advertise(topicName + "1", 1);
    decoder = nullptr;
}

void TopicUncompressor::receiveImageCallback (image_compressor::EncodedImage const & msg) {
    if (!decoder) {
        ROS_INFO("%i", msg.image.width);
        ROS_INFO("%i", msg.image.height);
        decoder = new ros_h264_streamer::H264Decoder(msg.image.width, msg.image.height);
    }
    sensor_msgs::ImagePtr outMessage;
    decoder->decode(msg.frame_size, (uint8_t *)&msg.image.data[0], outMessage);
    originalImagePublisher.publish(outMessage);
}