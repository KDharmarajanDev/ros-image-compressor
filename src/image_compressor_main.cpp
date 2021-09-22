// System Includes
#include <string>
#include <vector>
#include "ros/ros.h"

// Project Includes
#include <image_compressor/topic_compressor.h>

void setupCompressor(std::vector<TopicCompressor> & topicCompressors, std::string const & topicName,
                     ros::NodeHandle & nodeHandle);

int main(int argc, char **argv) {
    ros::init(argc, argv, "image_compressor");
    ros::NodeHandle rosHandle;
    std::vector<std::string> compressedTopics;
    rosHandle.getParam("/image_encoder/compress_video_topics", compressedTopics);
    std::vector<TopicCompressor> topicCompressors;
    for (std::string const & topicName : compressedTopics) {
        ROS_INFO("added");
        setupCompressor(topicCompressors, topicName, rosHandle);
    }
    ros::spin();
    return 0;
}
void setupCompressor(std::vector<TopicCompressor> & topicCompressors,
                     std::string const & topicName,
                     ros::NodeHandle & nodeHandle) {
    topicCompressors.emplace_back(topicName, nodeHandle);
}
