
// ROS Hexapod Teleop Joystick Node
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


#include <hexapod_teleop_joystick.h>


//==============================================================================
// Define some enums to use in the code
//==============================================================================
namespace JOYSTICK_AXES
{
  enum { LX=0, LY, RX, RY };
}

namespace JOYSTICK_BUTTONS
{
    enum {START=0, STOP, SHIFT, FIXED_COUNT=3};  // these are the fixed ones that  are not table driven
}

//==============================================================================
// Define some static default settings for menu...
//==============================================================================
// Default axes
static const int g_axes_index_default[] = {0, 1, 2, 3};   // DS3
//static const int g_axes_index_default[] = {0, 1, 2, 5};   // DS4

static const int g_button_index_default[] = {3, 0, 10, 13}; // DS3 Start, Select, L1, CIRCLE
// static const int g_button_index_default[] = {3, 0, 4, 2}; // DS4 TRI, SQUARE, L1, CIRCLE

static const std::string g_extra_button_topic_names[] = {"high_step_active"}; // DS3 Start, Select, L1, CIRCLE, X?


//==============================================================================
// Constructor
//==============================================================================

HexapodTeleopJoystick::HexapodTeleopJoystick( void )
{
    state_.data = false;
    imu_override_.data = false;

    // Try to read in button and axis mappings.
    // If not found will initialize to Kevin's DS3 settings (or commented out DS4)
    if (! ros::param::get( "JOYSTICK_AXES_MAPPING", axes_index_map_))
    {
        axes_index_map_.assign(g_axes_index_default, g_axes_index_default + ((sizeof(g_axes_index_default)/sizeof(g_axes_index_default[0]))));
        ROS_INFO("Joystick: Use default axes mapping");
    }

    if (! ros::param::get( "JOYSTICK_BUTTON_MAPPING", button_index_map_) )
    {
        button_index_map_.assign(g_button_index_default, g_button_index_default + ((sizeof(g_button_index_default)/sizeof(g_button_index_default[0]))));
        ROS_INFO("Joystick: Use default button mapping");
    }

    // Also allow us to append mapping to defaults...
    std::vector<int> button_index_append;
    if ( ros::param::get( "JOYSTICK_BUTTON_MAPPING_APPEND", button_index_append) )
    {
        button_index_map_.reserve( button_index_map_.size() + button_index_append.size() );
        button_index_map_.insert ( button_index_map_.end(), button_index_append.begin(), button_index_append.end() );  // add new ones on the end...
    }

    // See if there are any extra topics defined.
    std::vector<std::string> button_topic_names;
    if ( ros::param::get( "JOYSTICK_EXTRA_BUTTON_TOPICS", button_topic_names))
    {
        button_topic_names_ = button_topic_names;   // User overroode the list.
    }

    else
    {
        button_topic_names_ .assign(g_extra_button_topic_names, g_extra_button_topic_names + ((sizeof(g_extra_button_topic_names)/sizeof(g_extra_button_topic_names[0]))));

        if ( ros::param::get( "JOYSTICK_EXTRA_BUTTON_TOPICS_APPEND", button_topic_names))
        {
            button_topic_names_.reserve( button_topic_names_.size()+button_topic_names.size() );
            button_topic_names_.insert ( button_topic_names_.end(), button_topic_names.begin(), button_topic_names.end() );  // add new ones on the end...
        }
    }
    buttons_prev_.assign(button_index_map_.size(), 0);	// zero this one out...

    // Initialize some of the default publish
    ros::param::get( "MAX_METERS_PER_SEC", MAX_METERS_PER_SEC );
    ros::param::get( "MAX_RADIANS_PER_SEC", MAX_RADIANS_PER_SEC );
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 5, &HexapodTeleopJoystick::joyCallback, this);
    base_scalar_pub_ = nh_.advertise<geometry_msgs::AccelStamped>("base_scalar", 100);
    body_scalar_pub_ = nh_.advertise<geometry_msgs::AccelStamped>("body_scalar", 100);
    head_scalar_pub_ = nh_.advertise<geometry_msgs::AccelStamped>("head_scalar", 100);
    state_pub_ = nh_.advertise<std_msgs::Bool>("state", 100);
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 100);
    imu_override_pub_ = nh_.advertise<std_msgs::Bool>("imu_override", 100);
    // And also publish any button topics that we may have.
    button_pubs_.reserve( button_topic_names_.size() );      // reserve space for the generated topics.
    button_states_.reserve( button_topic_names_.size() ) ; // Set all of our button states to false...

    if ( (JOYSTICK_BUTTONS::FIXED_COUNT + button_topic_names_.size()) != button_index_map_.size() )
    {
        ROS_ERROR("Joystick Button Mapping sizes mismatch %d  %d",  (int)(JOYSTICK_BUTTONS::FIXED_COUNT + button_topic_names_.size()),
                (int)button_index_map_.size() );
    }

    std_msgs::Bool stdb;
    stdb.data = false;
    for (int i = 0; i < button_topic_names_.size(); i ++ )
    {
        button_pubs_.push_back( nh_.advertise<std_msgs::Bool>( button_topic_names_[i], 10 ) );
        button_states_.push_back( stdb );
        button_pubs_[i] .publish( button_states_[i] ); // Send out an initial value...
    }

}

//==============================================================================
// Joystick call reading joystick topics
//==============================================================================

void HexapodTeleopJoystick::joyCallback( const sensor_msgs::Joy::ConstPtr &joy )
{
    ros::Time current_time = ros::Time::now();
    // Activate locomotion with Logical start button (PS3=Start, PS4=Triangle)
    // Also have the ability here to define for joysticks with fewer buttons to use just one button to toggle on and off...
    if ( buttonPressed( joy, JOYSTICK_BUTTONS::START ) )
    {
        if (button_index_map_[JOYSTICK_BUTTONS::START] == button_index_map_[JOYSTICK_BUTTONS::STOP])
        {
            state_.data = state_.data ? false : true;
        }
        else
        {
            state_.data = true;
        }
    }

    // Deactivate locomotion with logical Stop button (PS3=Select, PS4=Square)
    else if (  buttonPressed( joy, JOYSTICK_BUTTONS::STOP ) )
    {
        state_.data = false;
    }

    // Now lets walk through our list of button topics and see if we have a match...
    for (int i = 0; i < button_topic_names_.size(); i ++ )
    {
        if ( buttonPressed( joy, JOYSTICK_BUTTONS::FIXED_COUNT + i ) )
        {
            button_states_[i].data = button_states_[i].data ? false : true;
            button_pubs_[i] .publish( button_states_[i] );
        }
    }

     // Only process some of the other conditions if we are active
    if ( state_.data )
    {
        // Body shift L1 Trigger
        if ( buttonDown( joy, JOYSTICK_BUTTONS::SHIFT ))
        {
            imu_override_.data = true;
            body_scalar_.header.stamp = current_time;
            body_scalar_.accel.angular.x = -joy->axes[axes_index_map_[JOYSTICK_AXES::LX]];
            body_scalar_.accel.angular.y = -joy->axes[axes_index_map_[JOYSTICK_AXES::LY]];
            head_scalar_.header.stamp = current_time;
            head_scalar_.accel.angular.z = joy->axes[axes_index_map_[JOYSTICK_AXES::RX]];
            head_scalar_.accel.angular.y = joy->axes[axes_index_map_[JOYSTICK_AXES::RY]];
        }
        else
        {
            imu_override_.data = false;
            base_scalar_.header.stamp = current_time;
            base_scalar_.accel.linear.x = -joy->axes[axes_index_map_[JOYSTICK_AXES::LY]];
            base_scalar_.accel.linear.y = joy->axes[axes_index_map_[JOYSTICK_AXES::LX]];
            base_scalar_.accel.angular.z = -joy->axes[axes_index_map_[JOYSTICK_AXES::RX]];
            cmd_vel_.linear.x = ( joy->axes[axes_index_map_[JOYSTICK_AXES::LY]] * MAX_METERS_PER_SEC );
            cmd_vel_.linear.y = ( joy->axes[axes_index_map_[JOYSTICK_AXES::LX]] * MAX_METERS_PER_SEC );
            cmd_vel_.angular.z = ( joy->axes[axes_index_map_[JOYSTICK_AXES::RX]] * MAX_RADIANS_PER_SEC );
        }
    }

    // Now publish the states.  Currently all of them.
    base_scalar_pub_.publish( base_scalar_ );
    body_scalar_pub_.publish( body_scalar_ );
    head_scalar_pub_.publish( head_scalar_ );
    state_pub_.publish( state_ );
    cmd_vel_pub_.publish( cmd_vel_ );
    imu_override_pub_.publish( imu_override_ );

    // Lets remember the state of the buttons...
    for (int i = 0; i < buttons_prev_.size(); i++)
    {
        buttons_prev_[i] = joy->buttons[button_index_map_[i]];
    }

}

bool HexapodTeleopJoystick::buttonDown(  const sensor_msgs::Joy::ConstPtr &joy, int button_index )
{
    return ((button_index < button_index_map_.size()) && (joy->buttons[button_index_map_[ button_index ]] == 1));
}

bool HexapodTeleopJoystick::buttonPressed ( const sensor_msgs::Joy::ConstPtr &joy, int button_index )
{
    return ((button_index < button_index_map_.size()) && (joy->buttons[button_index_map_[ button_index ]] == 1)
            && (buttons_prev_[button_index] == 0));
}

bool HexapodTeleopJoystick::buttonReleased ( const sensor_msgs::Joy::ConstPtr &joy, int button_index )
{
    return ((button_index < button_index_map_.size()) && (joy->buttons[button_index_map_[ button_index ]] == 0)
            && (buttons_prev_[button_index] == 1));
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "hexapod_teleop_joystick");
    HexapodTeleopJoystick hexapodTeleopJoystick;

    ros::spin();
}
