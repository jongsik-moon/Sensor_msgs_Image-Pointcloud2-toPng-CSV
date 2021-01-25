#include <ros/ros.h>
#include <thread>
#include "Config.h"
#include "Logger.h"

using namespace std;

int main (int argc, char** argv)
{
  ros::init(argc, argv, "pointcloudbag2csv");
  ros::NodeHandle nh;

  Config config;

  ros::Rate rate(config.datasetConfig.hz);
  Logger logger(config);

  std::vector<std::thread> workers;
  workers.push_back(std::thread(&Logger::WritePointcloudData, &logger));
  workers.push_back(std::thread(&Logger::WriteImgData, &logger));

  while (ros::ok()) {
    logger.Img2Container();
    logger.pointcloud2Container();
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}



