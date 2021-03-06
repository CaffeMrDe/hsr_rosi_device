/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2019, Foshan Huashu Robotics Co.,Ltd
 * All rights reserved.
 * 
 * Author: Kunlin Xu <1125290220@qq.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *      * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *      * Neither the name of the Southwest Research Institute, nor the names
 *      of its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef JOINT_TRAJECTORY_FULL_DOWNLOADER_H
#define JOINT_TRAJECTORY_FULL_DOWNLOADER_H

#include "joint_trajectory_full_interface.h"

namespace industrial_robot_client{
namespace joint_trajectory_downloader{

using industrial_robot_client::joint_trajectory_interface::JointTrajectoryFullInterface;
using industrial::joint_traj_pt_full_message::JointTrajPtFullMessage;
using industrial::joint_traj_pt_message::JointTrajPtMessage;
/**
 * @brief The JointTrajectoryFullDownloader class 华数机器人的Ros轨迹点下发的继承 JointTrajectoryInterface 代理类
 */
class JointTrajectoryFullDownloader : public JointTrajectoryFullInterface{

public:

  bool send_to_robot(const std::vector<JointTrajPtFullMessage>& messages);
  virtual bool send_to_robot(const std::vector<JointTrajPtMessage>& messages){}

};


/**
 * @brief 带有有动态插补的类
 */
class JointTrajectoryDownloaderWithPt :public  JointTrajectoryFullDownloader{
public:
    virtual bool send_to_robot(const JointTrajPtMessage& messages);
    bool init(ros::NodeHandle &n, std::string default_ip= "", int default_port = joint_trajectory_interface::StandardSocketPorts::MOTION);

    void impedanceCB(const sensor_msgs::JointState::ConstPtr& msg) {
//        ROS_ERROR_STREAM("impedanceCB ....... "<<msg->position.size());
		if(mode != ONCE_PT){
			ROS_ERROR_STREAM("mode error ,please switch the motion mode ");
			return ;
		}
        for(int i = 0; i < 6; i++)
            impedanceErr[i] =  msg->position[i];

        JointTrajPtMessage ptm;
        if(msg->position.size() == 7){
            ptm = create_message(impedanceErr, -1);
            ROS_INFO("servo start pos message ok .......");
        }
        else
            ptm = create_message(impedanceErr, -3);
//        ROS_INFO("create_message ok .......");
        send_to_robot(ptm);
    }

private:
    // 回调函数队列 nodehandle
    // 偏移值回调函数队列
    ros::CallbackQueue *impeQueue;

};

}	//namespace joint_trajectory_downloader
}	//namespace industrial_robot_client

#endif
