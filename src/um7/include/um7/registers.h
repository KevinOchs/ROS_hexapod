/**
 *
 *  \file
 *  \brief      Provides the Registers class, which initializes with a suite
 *              of accessors suitable for reading and writing the UM7 registers,
 *              including byte-order conversion and scaling handled.
 *  \author     Mike Purvis <mpurvis@clearpathrobotics.com> wrote original code for UM6
 *  \copyright  Copyright (c) 2013, Clearpath Robotics, Inc.
 *  \author     Alex Brown <rbirac@cox.net>                 adapted code for UM7
 *  \copyright  Copyright (c) 2015, Alex Brown
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
 * DISCLAIMED. IN NO EVENT SHALL CLEARPATH ROBOTICS, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Please send comments, questions, or patches to Alex Brown  rbirac@cox.net
 *
 */

#ifndef INCLUDE_UM7_REGISTERS_H_
#define INCLUDE_UM7_REGISTERS_H_

#if __APPLE__
#include <machine/endian.h>
#else
#include <endian.h>
#endif

#include <endian.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include <stdexcept>
#include <string>

#include "um7/firmware_registers.h"

#define TO_RADIANS (M_PI / 180.0)
#define TO_DEGREES (180.0 / M_PI)

// This excludes the command registers, which are always sent
// and received with no data.
#define NUM_REGISTERS (DATA_REG_START_ADDRESS + DATA_ARRAY_SIZE)


namespace um7 {

inline void memcpy_network(void* dest, void* src, size_t count) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  uint8_t* d = reinterpret_cast<uint8_t*>(dest);
  uint8_t* s = reinterpret_cast<uint8_t*>(src);
  for (uint8_t i = 0; i < count; i++) {
    d[i] = s[count - (i+1)];
  }
#else
  // Copy bytes without reversing.
  #warning Big-endian implementation is untested.
  memcpy(dest, src, count);
#endif
}

class Registers;

/**
 * This class provides an accessor of fields contained in one or more
 * consecutive UM7 registers. Each register is nominally a uint32_t, 
 * but XYZ vectors are sometimes stored as a pair of int16_t values in 
 * one register and one in the following register. Other values are 
 * stored as int32_t representation or float32s.
 *
 * This class takes care of the necessary transformations to simplify
 * the actual "business logic" of the driver.
 */
class Accessor_ {
  public:
    Accessor_(Registers* registers, uint8_t register_index,
        uint8_t register_width, uint8_t array_length)
      : index(register_index), width(register_width),
        length(array_length), registers_(registers)
    {}

    void* raw() const;

    /** 
     * Number/address of the register in the array of uint32s which is
     * shared with the UM7 firmware. */ 
    const uint8_t index;

    /** 
     * Width of the sub-register field, in bytes, either 2 or 4. */
    const uint8_t width;

    /** 
     * Length of how many sub-register fields comprise this accessor. Not 
     * required to stay within the bounds of a single register. */
    const uint16_t length;

  private:
    Registers* registers_;
};

template<typename RegT>
class Accessor : public Accessor_ {
  public:
    Accessor(Registers* registers, uint8_t register_index, uint8_t array_length = 0, double scale_factor = 1.0)
      : Accessor_(registers, register_index, sizeof(RegT), array_length), scale_(scale_factor)
    {}

    RegT get(uint8_t field) const {
      RegT* raw_ptr = reinterpret_cast<RegT*>(raw());
      RegT value;
      memcpy_network(&value, raw_ptr + field, sizeof(value));
      return value;
    }

    double get_scaled(uint16_t field) const {
      return get(field) * scale_;
    }

    void set(uint8_t field, RegT value) const {
      RegT* raw_ptr = reinterpret_cast<RegT*>(raw());
      memcpy_network(raw_ptr + field, &value, sizeof(value));
    }

    void set_scaled(uint16_t field, double value) const {
      set(field, value / scale_);
    }

  private:
    const double scale_;
};

class Registers {
  public:
    Registers() :
      gyro_raw(this, DREG_GYRO_RAW_XY, 3),
      accel_raw(this, DREG_ACCEL_RAW_XY, 3),
      mag_raw(this, DREG_MAG_RAW_XY, 3),
      gyro(this, DREG_GYRO_PROC_X, 3, 1.0 * TO_RADIANS),
      accel(this, DREG_ACCEL_PROC_X, 3, 9.80665),              
      mag(this, DREG_MAG_PROC_X, 3,1.0),
      euler(this, DREG_EULER_PHI_THETA, 3, 0.0109863 * TO_RADIANS),
      quat(this, DREG_QUAT_AB, 4, 0.0000335693),
      temperature(this, DREG_TEMPERATURE, 1),
      communication(this, CREG_COM_SETTINGS, 1),
      comrate2(this, CREG_COM_RATES2, 1),
      comrate4(this, CREG_COM_RATES4, 1),
      comrate5(this, CREG_COM_RATES5, 1), 
      comrate6(this, CREG_COM_RATES6, 1),
      misc_config(this, CREG_MISC_SETTINGS, 1),
      status(this, CREG_COM_RATES6, 1),
      mag_bias(this, CREG_MAG_BIAS_X, 3),
      cmd_zero_gyros(this, CHR_ZERO_GYROS),
      cmd_reset_ekf(this, CHR_RESET_EKF),
      cmd_set_mag_ref(this, CHR_SET_MAG_REFERENCE)
    {
      memset(raw_, 0, sizeof(raw_));
    }

    // Data
    const Accessor<int16_t> gyro_raw, accel_raw, euler, mag_raw, quat;
                             
    const Accessor<float> gyro, accel, mag, temperature;

    // Configs
    const Accessor<uint32_t> communication, misc_config, status, comrate2,
                            comrate4, comrate5, comrate6;

    const Accessor<float>  mag_bias;  

    // Commands
    const Accessor<uint32_t> cmd_zero_gyros, cmd_reset_ekf, cmd_set_mag_ref;

    void write_raw(uint8_t register_index, std::string data) {
      if ((register_index - 1) + (data.length()/4 - 1) >= NUM_REGISTERS) {
        throw std::range_error("Index and length write beyond boundaries of register array.");
      }
      memcpy(&raw_[register_index], data.c_str(), data.length());
    }

  private:
    uint32_t raw_[NUM_REGISTERS];

  friend class Accessor_;
};
}

#endif  // INCLUDE_UM7_REGISTERS_H_
