//
// Created by jongsik on 21. 1. 22..
//

#ifndef ROSBAG2SAVED_LOGGER_H
#define ROSBAG2SAVED_LOGGER_H

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <string>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <image_transport/image_transport.h>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <thread>
#include <mutex>
#include <chrono>
#include "Config.h"

class Logger
{
public:
  Logger(Config& config);
  void ImgCallBack(const sensor_msgs::ImageConstPtr& img);
  void PointCloudCallBack(const sensor_msgs::PointCloud2ConstPtr& pointcloud);
  void Img2Container();
  void pointcloud2Container();
  void WritePointcloudData();
  void WriteImgData();

private:
  Config& config_;

  ros::NodeHandle nh_;
  ros::Subscriber imgSub_;
  ros::Subscriber pointcloudSub_;

  cv::Mat inputImg_;
  pcl::PointCloud<pcl::PointXYZ> inputPointcloud_;
  ros::Time imgTime_;
  ros::Time pointcloudTime_;

  std::queue<cv::Mat> imgQueue_;
  std::queue<pcl::PointCloud<pcl::PointXYZ>> pointcloudQueue_;
  std::queue<std::string> imgNameQueue_;
  std::queue<std::string> pointcloudNameQueue_;

  cv::Mat savingImg;
  pcl::PointCloud<pcl::PointXYZ> savingPointcloud_;
  std::string imgTimeStr_;
  std::string pointcloudTimeStr_;

  bool GetImgDataFromContainer();
  bool GetPointCloudDataFromContainer();
  std::string GetCurrentTimeString();
  std::string GetTimeFromROSTIME(ros::Time input);

  std::string imgDir;
  std::string lidarDir;
};
#endif //ROSBAG2SAVED_LOGGER_H
