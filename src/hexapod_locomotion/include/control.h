
// ROS Hexapod Locomotion Node
// Copyright (c) 2014, Kevin M. Ochs
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the <organization> nor the
//     names of its contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: Kevin M. Ochs


#ifndef CONTROL_H_
#define CONTROL_H_

#include <math.h>
#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/JointState.h>
#include <hexapod_msgs/RootJoint.h>
#include <hexapod_msgs/BodyJoint.h>
#include <hexapod_msgs/HeadJoint.h>
#include <hexapod_msgs/LegJoints.h>
#include <hexapod_msgs/LegsJoints.h>
#include <hexapod_msgs/FootPosition.h>
#include <hexapod_msgs/FeetPositions.h>
#include <hexapod_msgs/State.h>
#include <hexapod_msgs/Sounds.h>

//==============================================================================
// Define class Control: This is the main structure of data that manipulates
// the hexapod.
//==============================================================================

class Control
{
    public:
        Control( void );
        void setHexActiveState( bool state );
        bool getHexActiveState( void );
        void setPrevHexActiveState( bool state );
        bool getPrevHexActiveState( void );
        void publishJointStates( const hexapod_msgs::LegsJoints &legs );
		sensor_msgs::JointState joint_state_;
        hexapod_msgs::RootJoint base_;
        hexapod_msgs::BodyJoint body_;
        hexapod_msgs::HeadJoint head_;
        hexapod_msgs::LegsJoints legs_;
        hexapod_msgs::FeetPositions feet_;
        hexapod_msgs::State state_;
        hexapod_msgs::Sounds sounds_;
        ros::Publisher sounds_pub_;
        ros::Publisher joint_state_pub_;
    private:
        bool imu_init_stored_;
        double imu_roll_lowpass_;
        double imu_pitch_lowpass_;
        double imu_yaw_lowpass_;
        double imu_roll_init_;
        double imu_pitch_init_;
        hexapod_msgs::State imu_override_;
        sensor_msgs::Imu imu_;
        bool hex_state_;      // Current loop state
        bool prev_hex_state_; // Previous loop state
        void baseCallback( const hexapod_msgs::RootJointConstPtr &base_msg );
        void bodyCallback( const hexapod_msgs::BodyJointConstPtr &body_msg );
        void headCallback( const hexapod_msgs::HeadJointConstPtr &head_msg );
        void stateCallback( const hexapod_msgs::StateConstPtr &state_msg );
        void imuOverrideCallback( const hexapod_msgs::StateConstPtr &imuOverride_msg );
        void imuCallback( const sensor_msgs::ImuConstPtr &imu_msg );
        ros::NodeHandle nh_;
        ros::Subscriber base_sub_;
        ros::Subscriber body_sub_;
        ros::Subscriber head_sub_;
        ros::Subscriber state_sub_;
        ros::Subscriber imu_sub_;
        ros::Subscriber imu_override_sub_;
};

#endif // CONTROL_H_

