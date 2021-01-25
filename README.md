# Sensor_msgs_Image-Pointcloud2-toPng-CSV
This package converts sensor_msgs::Image & sensor_msgs::Pointcloud2 -> saved .png and .csv format.
Both msgs can simultaneously converted and saved. Saving only one topic also supported.
Setting params can be easily done in yaml file.

#How to Build 
```
mkdir -p ~/catkin_ws/src/
cd ~/catkin_ws/src
git clone https://github.com/jongsik-moon/Sensor_msgs_Image-Pointcloud2-toPng-CSV.git
cd ..
catkin_make
```

#How to Use 

1. Build this package
2. Change the output directory for png & csv file where you want to save, and the sensor topic name you want to subscribe in /params/default.yaml
3. Run this package : png & csv files will be saved with the name of sensor msgs time

