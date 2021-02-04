//
// Created by jongsik on 21. 1. 22..
//

#include "Logger.h"

std::mutex imgLock_;
std::mutex pointcloudLock_;

Logger::Logger(Config& config)
  : config_(config)
  , imgLen(1000000)
  , lidarLen(1000000)
{
  std::string imgTopic(config_.datasetConfig.imgTopicName);
  std::string pcTopic(config_.datasetConfig.lidarTopicName);

  imgSub_ = nh_.subscribe(imgTopic, 1, &Logger::ImgCallBack, this);
  pointcloudSub_ = nh_.subscribe(pcTopic, 1, &Logger::PointCloudCallBack, this);

  std::string saveDir = config_.datasetConfig.outputDir + GetCurrentTimeString();
  imgDir = saveDir + "/img";
  lidarDir = saveDir + "/lidar";
  std::cout << imgDir << std::endl;
  std::cout << lidarDir << std::endl;

  boost::filesystem::create_directory(saveDir);
  boost::filesystem::create_directory(imgDir);
  boost::filesystem::create_directory(lidarDir);
}

void Logger::ImgCallBack(const sensor_msgs::ImageConstPtr &img)
{
  cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(img, "bgr8");
  inputImg_ = cv_ptr->image.clone();
  imgTime_ = ros::Time::now();
}

void Logger::PointCloudCallBack(const sensor_msgs::PointCloud2ConstPtr &pointcloud)
{
  pcl::PCLPointCloud2 pcl_pc2;
  pcl_conversions::toPCL(*pointcloud, pcl_pc2);
  pcl::fromPCLPointCloud2(pcl_pc2, inputPointcloud_);
  pointcloudTime_ = ros::Time::now();
}

void Logger::Img2Container()
{
  std::lock_guard<std::mutex> flagLockGuard(imgLock_);
  if(!inputImg_.empty())
  {
    imgQueue_.push(inputImg_);
    imgNameQueue_.push(std::to_string(imgLen));
    imgLen++;
  }
}

void Logger::pointcloud2Container()
{
  std::lock_guard<std::mutex> flagLockGuard(pointcloudLock_);
  if(inputPointcloud_.points.size() != 0)
  {
    pointcloudQueue_.push(inputPointcloud_);
    pointcloudNameQueue_.push(std::to_string(lidarLen));
    lidarLen++;
  }
}

void Logger::WritePointcloudData()
{
  while(true){
    while(!GetPointCloudDataFromContainer()){
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::string fileName;
    fileName = lidarDir + "/" + pointcloudTimeStr_ + ".csv";

    FILE* logFp_;
    logFp_ = fopen(fileName.c_str(), "wb");

    ros::Time t = ros::Time::now();
    for (int i = 0; i < savingPointcloud_.points.size() -1; i++) {
      fprintf(logFp_, "%f, %f, %f, ", savingPointcloud_.points[i].x, savingPointcloud_.points[i].y, savingPointcloud_.points[i].z);
    }
    fprintf(logFp_, "%f, %f, %f", savingPointcloud_.points[savingPointcloud_.points.size()-1].x, savingPointcloud_.points[savingPointcloud_.points.size()-1].y, savingPointcloud_.points[savingPointcloud_.points.size()-1].z);
    fclose(logFp_);
  }
}

void Logger::WriteImgData()
{
  while(true){
    while(!GetImgDataFromContainer()){
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::string currTime = GetCurrentTimeString();
    cv::imwrite(imgDir + "/" + imgTimeStr_ + ".png", savingImg);
  }
}

bool Logger::GetImgDataFromContainer()
{
  std::lock_guard<std::mutex> flagLockGuard(imgLock_);
  if(imgQueue_.size() > 0){
    savingImg = imgQueue_.front();
    imgQueue_.pop();
    imgTimeStr_ = imgNameQueue_.front();
    std::cout << imgTimeStr_ << std::endl;
    imgNameQueue_.pop();
    return true;
  }
  else return false;
}
bool Logger::GetPointCloudDataFromContainer()
{
  std::lock_guard<std::mutex> flagLockGuard(pointcloudLock_);
  if(pointcloudQueue_.size() > 0){
    savingPointcloud_ = pointcloudQueue_.front();
    pointcloudQueue_.pop();
    pointcloudTimeStr_ = pointcloudNameQueue_.front();
    pointcloudNameQueue_.pop();
    return true;
  }
  else return false;
}

std::string Logger::GetCurrentTimeString()
{
  time_t timer = time(NULL);
  struct tm* ltime = localtime(&timer);
  char fn_date[13];
  sprintf(fn_date, "%02d%02d%02d_%02d%02d%02d", (ltime->tm_year) - 100, (ltime->tm_mon) + 1, ltime->tm_mday,
          ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
  std::string dirName(fn_date);
  return dirName;
}

std::string Logger::GetTimeFromROSTIME(ros::Time input)
{
  boost::posix_time::ptime thistime = input.toBoost();
  std::string isoTimeStr = boost::posix_time::to_iso_extended_string(thistime);
  return isoTimeStr;
}
