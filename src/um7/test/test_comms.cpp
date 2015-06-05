#include "um7/comms.h"
#include "um7/registers.h"
#include "serial/serial.h"
#include <gtest/gtest.h>
#include <fcntl.h>

class FakeSerial : public ::testing::Test {
  protected:
    /**
     * What's going on here? The posix_openpt() call establishes a pseudo terminal
     * and gives us a fd for the other end of it. So we can connect up a Serial
     * instance to the pty, and have full control over reading-from and writing-to
     * the driver.
     */
    virtual void SetUp() {
      ASSERT_NE(-1, master_fd = posix_openpt( O_RDWR | O_NOCTTY | O_NDELAY ));
      ASSERT_NE(-1, grantpt(master_fd));
      ASSERT_NE(-1, unlockpt(master_fd));
      ASSERT_TRUE((ser_name = ptsname(master_fd)) != NULL);
      ser.setPort(ser_name);
      ser.open();
      ASSERT_TRUE(ser.isOpen()) << "Couldn't open Serial connection to pseudoterminal.";
    }

    void write_serial(const std::string& msg) {
      write(master_fd, msg.c_str(), msg.length());
    }

    virtual void TearDown() {
      ser.close();
      close(master_fd);
    }

    serial::Serial ser;

  private:
    int master_fd;
    char* ser_name;
};

TEST_F(FakeSerial, basic_message_rx) {
  // Send message from device which should write four bytes to the raw magnetometer's first register.
  std::string msg(um7::Comms::message(DREG_MAG_RAW_XY, std::string("\x1\x2\x3\x4")));
  write_serial(msg);

  um7::Comms sensor(&ser);
  um7::Registers registers;
  ASSERT_EQ(DREG_MAG_RAW_XY, sensor.receive(&registers)) << "Didn't return ID of arriving message.";
  EXPECT_EQ(0x0102, registers.mag_raw.get(0));
}

TEST_F(FakeSerial, batch_message_rx) {
  // Send message from device which should write four bytes to the raw accelerometer's registers.
  std::string msg(um7::Comms::message(DREG_ACCEL_RAW_XY, std::string("\x5\x6\x7\x8\x9\xa\0\0", 8)));
  write_serial(msg);

  um7::Comms sensor(&ser);
  um7::Registers registers;
  ASSERT_EQ(DREG_ACCEL_RAW_XY, sensor.receive(&registers)) << "Didn't return ID of arriving message.";
  EXPECT_EQ(0x0506, registers.accel_raw.get(0));
  EXPECT_EQ(0x0708, registers.accel_raw.get(1));
  EXPECT_EQ(0x090a, registers.accel_raw.get(2));
}

TEST_F(FakeSerial, bad_checksum_message_rx) {
  // Generate message, then twiddle final byte.
  std::string msg(um7::Comms::message(DREG_MAG_RAW_XY, std::string("\x1\x2\x3\x4")));
  msg[msg.length() - 1]++;
  write_serial(msg);

  um7::Comms sensor(&ser);
  um7::Registers registers;
  EXPECT_EQ(-1, sensor.receive(&registers)) << "Didn't properly ignore bad checksum message.";
}

TEST_F(FakeSerial, garbage_bytes_preceeding_message_rx) {
  // Generate message, then prepend junk.
  std::string msg(um7::Comms::message(CONFIG_REG_START_ADDRESS, std::string()));
  msg = "ssssssnsnsns" + msg;
  write_serial(msg);

  um7::Comms sensor(&ser);
  EXPECT_EQ(CONFIG_REG_START_ADDRESS, sensor.receive(NULL)) << "Didn't handle garbage prepended to message.";
}

TEST_F(FakeSerial, timeout_message_rx) {
  std::string msg("snp\x12\x45");
  write_serial(msg);
  um7::Comms sensor(&ser);
  EXPECT_EQ(-1, sensor.receive(NULL)) << "Didn't properly time out in the face of a partial message.";
}

int main(int argc, char **argv){
testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
}
