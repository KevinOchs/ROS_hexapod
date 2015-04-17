
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


#ifndef _AXMX_H_
#define _AXMX_H_

#define AX_CENTER_VALUE  512
#define MX_CENTER_VALUE  2048

#define AXMX_MODEL_NUMBER_L          0
#define AXMX_MODEL_NUMBER_H          1
#define AXMX_VERSION                 2
#define AXMX_SERVO_ID                3
#define AXMX_BAUD_RATE               4
#define AXMX_RETURN_DELAY_TIME       5
#define AXMX_CW_ANGLE_LIMIT_L        6
#define AXMX_CW_ANGLE_LIMIT_H        7
#define AXMX_CCW_ANGLE_LIMIT_L       8
#define AXMX_CCW_ANGLE_LIMIT_H       9
#define AXMX_LIMIT_TEMPERATURE       11
#define AXMX_LOW_LIMIT_VOLTAGE       12
#define AXMX_HIGH_LIMIT_VOLTAGE      13
#define AXMX_MAX_TORQUE_L            14
#define AXMX_MAX_TORQUE_H            15
#define AXMX_RETURN_LEVEL            16
#define AXMX_ALARM_LED               17
#define AXMX_ALARM_SHUTDOWN          18
#define AXMX_MULTI_TURN_OFFSET       20
#define AXMX_RESOLUTION_DIVIDER      22
#define AXMX_TORQUE_ENABLE           24
#define AXMX_LED                     25
#define AXMX_P_GAIN                  26
#define AXMX_I_GAIN                  27
#define AXMX_D_GAIN                  28
#define AXMX_GOAL_POSITION_L         30
#define AXMX_GOAL_POSITION_H         31
#define AXMX_GOAL_SPEED_L            32
#define AXMX_GOAL_SPEED_H            33
#define AXMX_TORQUE_LIMIT_L          34
#define AXMX_TORQUE_LIMIT_H          35
#define AXMX_PRESENT_POSITION_L      36
#define AXMX_PRESENT_POSITION_H      37
#define AXMX_PRESENT_SPEED_L         38
#define AXMX_PRESENT_SPEED_H         39
#define AXMX_PRESENT_LOAD_L          40
#define AXMX_PRESENT_LOAD_H          41
#define AXMX_PRESENT_VOLTAGE         42
#define AXMX_PRESENT_TEMPERATURE     43
#define AXMX_REGISTERED_INSTRUCTION  44
#define AXMX_MOVING                  46
#define AXMX_LOCK                    47
#define AXMX_PUNCH_L                 48
#define AXMX_PUNCH_H                 49
#define AXMX_CURRENT                 68
#define AXMX_TORQUE_CONTROL_MODE     70
#define AXMX_GOAL_TORQUE             71
#define AXMX_GOAL_ACCELERATION       73

#endif