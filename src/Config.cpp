//
// Created by jongsik on 21. 1. 22..
//

#include "Config.h"

Config::Config()
{
  YAML::Node yamlFile = YAML::LoadFile("/home/jongsik/modu_ws/src/rosbag2saved/params/default.yaml");
  ReadEveryParameter(yamlFile);
}

Config::~Config()
{
}

void Config::ReadEveryParameter(const YAML::Node yamlFile)
{
  YAML::Node datasetYaml = yamlFile["Dataset"];
  datasetConfig.inputImgDir = datasetYaml["inputImgDir"].as<std::string>();
  datasetConfig.inputLidarDir = datasetYaml["inputLidarDir"].as<std::string>();
  datasetConfig.outputDir = datasetYaml["outputDir"].as<std::string>();
  datasetConfig.imgTopicName = datasetYaml["imgTopicName"].as<std::string>();
  datasetConfig.lidarTopicName = datasetYaml["lidarTopicName"].as<std::string>();
  datasetConfig.hz = datasetYaml["hz"].as<float>();
}