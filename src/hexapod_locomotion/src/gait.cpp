
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

#include <gait.h>

const static double PI = atan(1.0)*4.0;

//==============================================================================
//  Constructor: Initialize gait variables
//==============================================================================

#define    RR    0
#define    RM    1
#define    RF    2
#define    LR    3
#define    LM    4
#define    LF    5

Gait::Gait( void )
{
    cycle_leg_number_[RF] = 0;
    cycle_leg_number_[LM] = 0;
    cycle_leg_number_[RR] = 0;
    cycle_leg_number_[LF] = 1;
    cycle_leg_number_[RM] = 1;
    cycle_leg_number_[LR] = 1;

    cycle_length_ = 50;
    cycle_period_ = 1;
    leg_lift_height_ = 40;
    is_travelling_ = false;
    in_cycle_ == false;
    extra_gait_cycle_ = 1;
}

//=============================================================================
// step calculation
//=============================================================================

void Gait::cyclePeriod( const hexapod_msgs::RootJoint &base, hexapod_msgs::FeetPositions *feet )
{
    for( int leg_index = 0; leg_index <= 5; leg_index++ )
    {
        // Lifts the leg and move it forward
        if( cycle_leg_number_[leg_index] == 0 && is_travelling_ == true )
        {
            feet->foot[leg_index].x = -base.x * cos( cycle_period_ * PI / cycle_length_ );
            feet->foot[leg_index].y = -base.y * cos( cycle_period_ * PI / cycle_length_ );
            feet->foot[leg_index].z = leg_lift_height_ * sin( cycle_period_ * PI / cycle_length_ );
            feet->foot[leg_index].yaw = -base.yaw * cos( cycle_period_ * PI / cycle_length_ );
        }
        // Moves legs backward pushing the body forward
        if( cycle_leg_number_[leg_index] == 1 )
        {
            feet->foot[leg_index].x = base.x * cos( cycle_period_ * PI / cycle_length_ );
            feet->foot[leg_index].y = base.y * cos( cycle_period_ * PI / cycle_length_ );
            feet->foot[leg_index].z = 0.0;
            feet->foot[leg_index].yaw = base.yaw * cos( cycle_period_ * PI / cycle_length_ );
        }
    }
}

//=============================================================================
// Gait Sequencing
//=============================================================================

void Gait::gaitCycle( const hexapod_msgs::RootJoint &base, hexapod_msgs::FeetPositions *feet )
{
    // Check to see if we are actually travelling
    if( ( std::abs( base.y ) > 1 ) || // 1 mm
        ( std::abs( base.x ) > 1 ) || // 1 mm
        ( std::abs( base.yaw ) > 0.00436332313 ) ) // 0.25 degree
    {
        is_travelling_ = true;
    }
    else
    {
        is_travelling_ = false;

        for( int leg_index = 0; leg_index <= 5; leg_index++ )
        {
            if( ( std::abs( feet->foot[leg_index].x ) > 1 ) || // 1 mm
                ( std::abs( feet->foot[leg_index].y ) > 1 ) || // 1 mm
                ( std::abs( feet->foot[leg_index].yaw ) > 0.034906585 ) || // 2 degrees
                  std::abs( feet->foot[leg_index].z) > 1 ) // 1 mm
            {
                // This forces another cycle to allow all legs to set down after travel is stopped
                extra_gait_cycle_ = cycle_length_ - cycle_period_ + cycle_length_;
                break;
            }
            else
            {
                extra_gait_cycle_ = 1;
            }
        }

        // countdown for in_cycle state
        if( extra_gait_cycle_ > 1 )
        {
            extra_gait_cycle_--;
            in_cycle_ = !( extra_gait_cycle_ == 1 );
        }
    }

    // If either is true we consider the gait active
    if( is_travelling_ == true || in_cycle_ == true  )
    {
        cyclePeriod( base, feet );
        cycle_period_++;
    }
    else
    {
        // Reset period to start just to be sure. ( It should be here anyway )
        cycle_period_ = 1;
    }

    // Loop cycle and switch the leg groupings for cycle
    if( cycle_period_ == cycle_length_ )
    {
        cycle_period_ = 1;

        for( int leg_index = 0; leg_index <= 5; leg_index++ )
        {
            cycle_leg_number_[leg_index] = cycle_leg_number_[leg_index] * -1 + 1;
        }
    }
}

