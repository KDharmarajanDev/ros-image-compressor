#include <image_compressor/topic_compressor.h>

TopicCompressor::TopicCompressor (std::string const & topicName, ros::NodeHandle const & nodeHandle) : it(nodeHandle) {
    sub = it.subscribe(topicName, 1, &TopicCompressor::receiveImageCallback, this);
    compressPub = it.advertise(topicName + "_compressed", 1);
    encoder = nullptr;
}

void TopicCompressor::receiveImageCallback (sensor_msgs::ImageConstPtr const & msg) {
    if (!encoder) {
        encoder = new ros_h264_streamer::H264Encoder(msg->width, msg->height, 5, 30, 30, msg->encoding);
    }
    ros_h264_streamer::H264EncoderResult result = encoder->encode(msg);
    sensor_msgs::ImagePtr outMessage;
    outMessage->header = msg->header;

    // IMPORTANT: Height field is set as the frame_size
    outMessage->height = result.frame_size;

    outMessage->encoding = msg->encoding;
    outMessage->is_bigendian = msg->is_bigendian;
    outMessage->step = msg->step;
    outMessage->data = std::vector<uint8_t>(result.frame_size);
    memcpy(&outMessage->data[0], result.frame_data, result.frame_size * sizeof(uint8_t));
    compressPub.publish(outMessage);
}