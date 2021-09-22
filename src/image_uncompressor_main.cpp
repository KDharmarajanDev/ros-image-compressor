// System Includes
#include <string>
#include <vector>
#include "ros/ros.h"

// Project Includes
#include <image_compressor/topic_uncompressor.h>

void setupUncompressor(std::vector<TopicUncompressor> & topicUncompressors, std::string const & topicName,
                     ros::NodeHandle & nodeHandle);

int main(int argc, char **argv) {
    ros::init(argc, argv, "image_uncompressor");
    ros::NodeHandle rosHandle;
    std::vector<std::string> uncompressedTopics;
    rosHandle.getParam("/image_decoder/uncompress_video_topics", uncompressedTopics);
    std::vector<TopicUncompressor> topicUncompressors;
    for (std::string const & topicName : uncompressedTopics) {
        ROS_INFO("POOF!!!!!");
        setupUncompressor(topicUncompressors, topicName, rosHandle);
    }
    ros::spin();
    return 0;
}
void setupUncompressor(std::vector<TopicUncompressor> & topicUncompressors,
                     std::string const & topicName,
                     ros::NodeHandle & nodeHandle) {
    topicUncompressors.emplace_back(topicName, nodeHandle);
}
