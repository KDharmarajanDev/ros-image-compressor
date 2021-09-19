#include <image_compressor/topic_compressor.h>
#include <image_compressor/EncodedImage.h>

TopicCompressor::TopicCompressor (std::string const & topicName, ros::NodeHandle & nodeHandle) : it(nodeHandle) {
    sub = it.subscribe(topicName, 1, &TopicCompressor::receiveImageCallback, this);
    compressPub = nodeHandle.advertise<image_compressor::EncodedImage>(topicName + "_compressed", 1);
    encoder = nullptr;
}

void TopicCompressor::receiveImageCallback (sensor_msgs::ImageConstPtr const & msg) {
    if (!encoder) {
        encoder = new ros_h264_streamer::H264Encoder(msg->width, msg->height, 5, 30, 30, msg->encoding);
    }
    ros_h264_streamer::H264EncoderResult result = encoder->encode(msg);
    image_compressor::EncodedImage outMessage;
    outMessage.image.header = msg->header;
    // IMPORTANT: Height field is set as the frame_size
    outMessage.image.height = msg->height;

    outMessage.image.encoding = msg->encoding;
    outMessage.image.is_bigendian = msg->is_bigendian;
    outMessage.image.step = msg->step;
    outMessage.image.data = std::vector<uint8_t>(result.frame_size);
    memcpy(&outMessage.image.data[0], result.frame_data, result.frame_size * sizeof(uint8_t));
    outMessage.frame_size = result.frame_size;
    compressPub.publish(outMessage);
}