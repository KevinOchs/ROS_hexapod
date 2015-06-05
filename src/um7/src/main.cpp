/**
 *
 *  \file
 *  \brief      Main entry point for UM7 driver. Handles serial connection
 *              details, as well as all ROS message stuffing, parameters,
 *              topics, etc.
 *  \author     Mike Purvis <mpurvis@clearpathrobotics.com> (original code for UM6)
 *  \copyright  Copyright (c) 2013, Clearpath Robotics, Inc.
 *  \author     Alex Brown <rbirac@cox.net>		    (adapted to UM7)
 *  \copyright  Copyright (c) 2015, Alex Brown.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Clearpath Robotics, Inc. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLEARPATH ROBOTICS, INC. OR ALEX BROWN BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Please send comments, questions, or patches to Alex Brown  rbirac@cox.net
 *
 */

#include "geometry_msgs/Vector3Stamped.h"
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "serial/serial.h"            // must install serial library from apt-get
#include "std_msgs/Float32.h"
#include "std_msgs/Header.h"
#include "um7/comms.h"
#include "um7/registers.h"
#include "um7/Reset.h"
#include <string>

float covar[9];     // orientation covariance values
const char VERSION[10] = "0.0.2";   // um7_driver version

// Don't try to be too clever. Arrival of this message triggers
// us to publish everything we have.
const uint8_t TRIGGER_PACKET = DREG_EULER_PHI_THETA;

/**
 * Function generalizes the process of writing an XYZ vector into consecutive
 * fields in UM7 registers.
 */
template<typename RegT>
void configureVector3(um7::Comms* sensor, const um7::Accessor<RegT>& reg,
    std::string param, std::string human_name)
{
  if (reg.length != 3)
  {
    throw std::logic_error("configureVector3 may only be used with 3-field registers!");
  }

  if (ros::param::has(param))
  {
    double x, y, z;
    ros::param::get(param + "/x", x);
    ros::param::get(param + "/y", y);
    ros::param::get(param + "/z", z);
    ROS_INFO_STREAM("Configuring " << human_name << " to ("
                    << x << ", " << y << ", " << z << ")");
    reg.set_scaled(0, x);
    reg.set_scaled(1, y);
    reg.set_scaled(2, z);
    if (sensor->sendWaitAck(reg))
    {
      throw std::runtime_error("Unable to configure vector.");
    }
  }
}

/**
 * Function generalizes the process of commanding the UM7 via one of its command
 * registers.
 */
template<typename RegT>
void sendCommand(um7::Comms* sensor, const um7::Accessor<RegT>& reg, std::string human_name)
{
  ROS_INFO_STREAM("Sending command: " << human_name);
  if (!sensor->sendWaitAck(reg))
  {
    throw std::runtime_error("Command to device failed.");
  }
}


/**
 * Send configuration messages to the UM7, critically, to turn on the value outputs
 * which we require, and inject necessary configuration parameters.
 */
void configureSensor(um7::Comms* sensor)
{
  um7::Registers r;

    uint32_t comm_reg = (BAUD_115200 << COM_BAUD_START);
    r.communication.set(0, comm_reg);
    if (!sensor->sendWaitAck(r.comrate2))
    {
      throw std::runtime_error("Unable to set CREG_COM_SETTINGS.");
    }

    uint32_t raw_rate = (20 << RATE2_ALL_RAW_START);
    r.comrate2.set(0, raw_rate);
    if (!sensor->sendWaitAck(r.comrate2))
    {
      throw std::runtime_error("Unable to set CREG_COM_RATES2.");
    }

    uint32_t proc_rate = (20 << RATE4_ALL_PROC_START);
    r.comrate4.set(0, proc_rate);
    if (!sensor->sendWaitAck(r.comrate4))
    {
      throw std::runtime_error("Unable to set CREG_COM_RATES4.");
    }

    uint32_t misc_rate = (20 << RATE5_EULER_START) | (20 << RATE5_QUAT_START);
    r.comrate5.set(0, misc_rate);
    if (!sensor->sendWaitAck(r.comrate5))
    {
      throw std::runtime_error("Unable to set CREG_COM_RATES5.");
    }

    uint32_t health_rate = (5 << RATE6_HEALTH_START);  // note:  5 gives 2 hz rate
    r.comrate6.set(0, health_rate);
    if (!sensor->sendWaitAck(r.comrate6))
    {
      throw std::runtime_error("Unable to set CREG_COM_RATES6.");
    }


  // Options available using parameters)
  uint32_t misc_config_reg = 0;  // initialize all options off

  // Optionally disable mag updates in the sensor's EKF.
  bool mag_updates;
  ros::param::param<bool>("~mag_updates", mag_updates, true);
  if (mag_updates)
  {
    misc_config_reg |= MAG_UPDATES_ENABLED;
  }
  else
  {
    ROS_WARN("Excluding magnetometer updates from EKF.");
  }

  // Optionally enable quaternion mode .
  bool quat_mode;
  ros::param::param<bool>("~quat_mode", quat_mode, true);
  if (quat_mode)
  {
    misc_config_reg |= QUATERNION_MODE_ENABLED;
  }
  else
  {
    ROS_WARN("Excluding quaternion mode.");
  }

  r.misc_config.set(0, misc_config_reg);
  if (!sensor->sendWaitAck(r.misc_config))
  {
    throw std::runtime_error("Unable to set CREG_MISC_SETTINGS.");
  }

  // Optionally disable performing a zero gyros command on driver startup.
  bool zero_gyros;
  ros::param::param<bool>("~zero_gyros", zero_gyros, true);
  if (zero_gyros) sendCommand(sensor, r.cmd_zero_gyros, "zero gyroscopes");
}


bool handleResetService(um7::Comms* sensor,
    const um7::Reset::Request& req, const um7::Reset::Response& resp)
{
  um7::Registers r;
  if (req.zero_gyros) sendCommand(sensor, r.cmd_zero_gyros, "zero gyroscopes");
  if (req.reset_ekf) sendCommand(sensor, r.cmd_reset_ekf, "reset EKF");
  if (req.set_mag_ref) sendCommand(sensor, r.cmd_set_mag_ref, "set magnetometer reference");
  return true;
}

/**
 * Uses the register accessors to grab data from the IMU, and populate
 * the ROS messages which are output.
 */
void publishMsgs(um7::Registers& r, ros::NodeHandle* n, const std_msgs::Header& header)
{
  static ros::Publisher imu_pub = n->advertise<sensor_msgs::Imu>("imu/data", 1, false);
  static ros::Publisher mag_pub = n->advertise<geometry_msgs::Vector3Stamped>("imu/mag", 1, false);
  static ros::Publisher rpy_pub = n->advertise<geometry_msgs::Vector3Stamped>("imu/rpy", 1, false);
  static ros::Publisher temp_pub = n->advertise<std_msgs::Float32>("imu/temperature", 1, false);

  if (imu_pub.getNumSubscribers() > 0)
  {
    sensor_msgs::Imu imu_msg;
    imu_msg.header = header;

    // IMU outputs [w,x,y,z], convert to [x,y,z,w] & transform to ROS axes
    imu_msg.orientation.x =  r.quat.get_scaled(1);
    imu_msg.orientation.y = -r.quat.get_scaled(2);
    imu_msg.orientation.z = -r.quat.get_scaled(3);
    imu_msg.orientation.w = r.quat.get_scaled(0);

    // Covariance of attitude.  set to constant default or parameter values
    imu_msg.orientation_covariance[0] = covar[0];
    imu_msg.orientation_covariance[1] = covar[1];
    imu_msg.orientation_covariance[2] = covar[2];
    imu_msg.orientation_covariance[3] = covar[3];
    imu_msg.orientation_covariance[4] = covar[4];
    imu_msg.orientation_covariance[5] = covar[5];
    imu_msg.orientation_covariance[6] = covar[6];
    imu_msg.orientation_covariance[7] = covar[7];
    imu_msg.orientation_covariance[8] = covar[8];

    // Angular velocity.  transform to ROS axes
    imu_msg.angular_velocity.x =  r.gyro.get_scaled(0);
    imu_msg.angular_velocity.y = -r.gyro.get_scaled(1);
    imu_msg.angular_velocity.z = -r.gyro.get_scaled(2);

    // Linear accel.  transform to ROS axes
    imu_msg.linear_acceleration.x =  r.accel.get_scaled(0);
    imu_msg.linear_acceleration.y = -r.accel.get_scaled(1);
    imu_msg.linear_acceleration.z = -r.accel.get_scaled(2);

    imu_pub.publish(imu_msg);
  }

  // Magnetometer.  transform to ROS axes
  if (mag_pub.getNumSubscribers() > 0)
  {
    geometry_msgs::Vector3Stamped mag_msg;
    mag_msg.header = header;
    mag_msg.vector.x =  r.mag.get_scaled(0);
    mag_msg.vector.y = -r.mag.get_scaled(1);
    mag_msg.vector.z = -r.mag.get_scaled(2);
    mag_pub.publish(mag_msg);
  }

  // Euler attitudes.  transform to ROS axes
  if (rpy_pub.getNumSubscribers() > 0)
  {
    geometry_msgs::Vector3Stamped rpy_msg;
    rpy_msg.header = header;
    rpy_msg.vector.x =  r.euler.get_scaled(0);
    rpy_msg.vector.y = -r.euler.get_scaled(1);
    rpy_msg.vector.z = -r.euler.get_scaled(2);
    rpy_pub.publish(rpy_msg);
  }

  // Temperature
  if (temp_pub.getNumSubscribers() > 0)
  {
    std_msgs::Float32 temp_msg;
    temp_msg.data = r.temperature.get_scaled(0);
    temp_pub.publish(temp_msg);
  }
}


/**
 * Node entry-point. Handles ROS setup, and serial port connection/reconnection.
 */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "um7_driver");

  // Load parameters from private node handle.
  std::string port;
  int32_t baud;
  ros::param::param<std::string>("~port", port, "/dev/ttyUSB0");
  ros::param::param<int32_t>("~baud", baud, 115200);

  serial::Serial ser;
  ser.setPort(port);
  ser.setBaudrate(baud);
  serial::Timeout to = serial::Timeout(50, 50, 0, 50, 0);
  ser.setTimeout(to);

  ros::NodeHandle n;

  std_msgs::Header header;
  ros::param::param<std::string>("~frame_id", header.frame_id, "imu_link");

  // Initialize covariance. The UM7 sensor does not provide covariance values so,
  //   by default, this driver provides a covariance array of all zeros indicating
  //   "covariance unknown" as advised in sensor_msgs/Imu.h.
  // This param allows the user to specify alternate covariance values if needed.

  std::string covariance;
  char cov[200];
  char *ptr1;

  ros::param::param<std::string>("~covariance", covariance, "0 0 0 0 0 0 0 0 0");
  snprintf(cov, sizeof(cov), "%s", covariance.c_str());

  char* p = strtok_r(cov, " ", &ptr1);           // point to first value
  for (int iter = 0; iter < 9; iter++)
  {
    if (p) covar[iter] = atof(p);                // covar[] is global var
    else  covar[iter] = 0.0;
    p = strtok_r(NULL, " ", &ptr1);              // point to next value (nil if none)
  }

  // Real Time Loop
  bool first_failure = true;
  while (ros::ok())
  {
    try
    {
      ser.open();
    }
    catch(const serial::IOException& e)
    {
        ROS_DEBUG("um7_driver ver %s unable to connect to port.", VERSION);
    }
    if (ser.isOpen())
    {
      ROS_INFO("um7_driver ver %s connected to serial port.", VERSION);
      first_failure = true;
      try
      {
        um7::Comms sensor(&ser);
        configureSensor(&sensor);
        um7::Registers registers;
        ros::ServiceServer srv = n.advertiseService<um7::Reset::Request, um7::Reset::Response>(
            "reset", boost::bind(handleResetService, &sensor, _1, _2));

        while (ros::ok())
        {
          // triggered by arrival of last message packet
          if (sensor.receive(&registers) == TRIGGER_PACKET)
          {
            header.stamp = ros::Time::now();
            publishMsgs(registers, &n, header);
            ros::spinOnce();
          }
        }
      }
      catch(const std::exception& e)
      {
        if (ser.isOpen()) ser.close();
        ROS_ERROR_STREAM(e.what());
        ROS_INFO("Attempting reconnection after error.");
        ros::Duration(1.0).sleep();
      }
    }
    else
    {
      ROS_WARN_STREAM_COND(first_failure, "Could not connect to serial device "
          << port << ". Trying again every 1 second.");
      first_failure = false;
      ros::Duration(1.0).sleep();
    }
  }
}
