//
// Created by jongsik on 21. 1. 22..
//

#ifndef ROSBAG2SAVED_CONFIG_H
#define ROSBAG2SAVED_CONFIG_H

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>

struct DatasetConfig
{
  std::string outputDir;
  std::string imgTopicName;
  std::string lidarTopicName;
  float hz;
};

class Config{
public:
  Config();
  ~Config();
  DatasetConfig datasetConfig;

private:
  void ReadEveryParameter(YAML::Node yamlFile);
};
#endif //ROSBAG2SAVED_CONFIG_H
