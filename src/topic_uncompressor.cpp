#include <image_compressor/topic_uncompressor.h>

TopicUncompressor::TopicUncompressor (std::string const & topicName, ros::NodeHandle const & nodeHandle) : it(nodeHandle) {
    sub = it.subscribe(topicName + "_compressed", 1, &TopicUncompressor::receiveImageCallback, this);
    originalImagePublisher = it.advertise(topicName + "1", 1);
    decoder = nullptr;
}

void TopicUncompressor::receiveImageCallback (sensor_msgs::ImageConstPtr const & msg) {
    if (!decoder) {
        decoder = new ros_h264_streamer::H264Decoder(msg->width, msg->height);
    }
    ros_h264_streamer::H264DecoderResult result = encoder->encode(msg);
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