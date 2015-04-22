
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


#ifndef SERVO_DRIVER_H_
#define SERVO_DRIVER_H_

#include <cmath>
#include <ros/ros.h>
#include <dynamixel.h>
#include <ax.h>
#include <hexapod_msgs/HeadJoint.h>
#include <hexapod_msgs/LegsJoints.h>

//====================================================================
// Number servos in system
//====================================================================

#define SERVO_COUNT 20 //18 Legs, Pan, Tilt

//====================================================================
// Define Servo ID's that go with each joint
//====================================================================

#define RR_COXA_ID   8  // Rear Right leg Coxa
#define RR_FEMUR_ID  10  // Rear Right leg Femur
#define RR_TIBIA_ID  12  // Rear Right leg Tibia

#define RM_COXA_ID   14  // Middle Right leg Coxa
#define RM_FEMUR_ID  16  // Middle Right leg Femur
#define RM_TIBIA_ID  18  // Middle Right leg Tibia

#define RF_COXA_ID   2  // Front Right leg Coxa
#define RF_FEMUR_ID  4 // Front Right leg Femur
#define RF_TIBIA_ID  6 // Front Right leg Tibia

#define LR_COXA_ID   7 // Rear Left leg Coxa
#define LR_FEMUR_ID  9 // Rear Left leg Femur
#define LR_TIBIA_ID  11 // Rear Left leg Tibia

#define LM_COXA_ID   13 // Middle Left leg Coxa
#define LM_FEMUR_ID  15 // Middle Left leg Femur
#define LM_TIBIA_ID  17 // Middle Left leg Tibia

#define LF_COXA_ID   19 // Front Left leg Coxa
#define LF_FEMUR_ID  3 // Front Left leg Femur
#define LF_TIBIA_ID  5 // Front Left leg Tibia

#define HEAD_PAN_ID  21 // Head Pan

#define HEAD_TILT_ID 22 // Head Tilt

#define FIRST_COXA_ID     0
#define FIRST_FEMUR_ID    6
#define FIRST_TIBIA_ID    12

//==============================================================================
// Define the class(s) for Servo Drivers.
//==============================================================================

class ServoDriver
{
    public:
        ServoDriver( void );
        void transmitServoPositions( const hexapod_msgs::LegsJoints &legs, const hexapod_msgs::HeadJoint &head );
        void freeServos( void );
    private:
        void convertAngles( const hexapod_msgs::LegsJoints  &legs, const hexapod_msgs::HeadJoint &head );
        void makeSureServosAreOn( void );
        int cur_pos_[SERVO_COUNT];
        int goal_pos_[SERVO_COUNT];
        int pose_steps_[SERVO_COUNT];
        int write_pos_[SERVO_COUNT];
        bool servos_free_;
        double OFFSET_ANGLE;
};

#endif // SERVO_DRIVER_H_