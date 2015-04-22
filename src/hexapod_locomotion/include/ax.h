
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


#ifndef _AX_H_
#define _AX_H_

#define AX_CENTER_VALUE  512

#define AX_MODEL_NUMBER_L          0
#define AX_MODEL_NUMBER_H          1
#define AX_VERSION                 2
#define AX_SERVO_ID                3
#define AX_BAUD_RATE               4
#define AX_RETURN_DELAY_TIME       5
#define AX_CW_ANGLE_LIMIT_L        6
#define AX_CW_ANGLE_LIMIT_H        7
#define AX_CCW_ANGLE_LIMIT_L       8
#define AX_CCW_ANGLE_LIMIT_H       9
#define AX_LIMIT_TEMPERATURE       11
#define AX_LOW_LIMIT_VOLTAGE       12
#define AX_HIGH_LIMIT_VOLTAGE      13
#define AX_MAX_TORQUE_L            14
#define AX_MAX_TORQUE_H            15
#define AX_RETURN_LEVEL            16
#define AX_ALARM_LED               17
#define AX_ALARM_SHUTDOWN          18
#define AX_TORQUE_ENABLE           24
#define AX_LED                     25
#define AX_CW_COMPLIANCE_MARGIN    26
#define AX_CCW_COMPLIANCE_MARGIN   27
#define AX_CW_COMPLIANCE_SLOPE     28
#define AX_CCW_COMPLIANCE_SLOPE    29
#define AX_GOAL_POSITION_L         30
#define AX_GOAL_POSITION_H         31
#define AX_MOVING_SPEED_L          32
#define AX_MOVING_SPEED_H          33
#define AX_TORQUE_LIMIT_L          34
#define AX_TORQUE_LIMIT_H          35
#define AX_PRESENT_POSITION_L      36
#define AX_PRESENT_POSITION_H      37
#define AX_PRESENT_SPEED_L         38
#define AX_PRESENT_SPEED_H         39
#define AX_PRESENT_LOAD_L          40
#define AX_PRESENT_LOAD_H          41
#define AX_PRESENT_VOLTAGE         42
#define AX_PRESENT_TEMPERATURE     43
#define AX_REGISTERED_INSTRUCTION  44
#define AX_MOVING                  46
#define AX_LOCK                    47
#define AX_PUNCH_L                 48
#define AX_PUNCH_H                 49

#endif