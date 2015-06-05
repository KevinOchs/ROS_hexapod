 /**  
 *  \file
 *  \brief   Copied directly from the UM7 version of the UM6_config.h file, available online here:
 *           http://sourceforge.net/p/um6firmware/code/34/tree/trunk/UM6%20Firmware/UM6_config.h#l14
 *           Note: while while the source of this code is named "UM6_config.h", it is not the same
 *           as the file used for the UM6 and is part of the UM7 source code.
 *  \author  Alex Brown  rbirac@cox.net
 *  \maintainer  Alex Brown  rbirac@cox.net    
 */


// Define the firmware revision
#define	UM6_FIRMWARE_REVISION		(('U' << 24) | ('7' << 16) | ('1' << 8) | 'C')

// CONFIG_ARRAY_SIZE and DATA_ARRAY_SIZE specify the number of 32 bit configuration and data registers used by the firmware
// (Note: The term "register" is used loosely here.  These "registers" are not actually registers in the same sense of a 
// microcontroller register.  They are simply index locations into arrays stored in global memory.  Data and configuration
// parameters are stored in arrays because it allows a common communication protocol to be used to access all data and
// configuration.  The software communicating with the sensor needs only specify the register address, and the communication
// software running on the sensor knows exactly where to find it - it needn't know what the data is.  The software communicatin
// with the sensor, on the other hand, needs to know what it is asking for (naturally...)
// This setup makes it easy to make more data immediately available when needed - simply increase the array size, add code in
// the firmware that writes data to the new array location, and then make updates to the firmware definition on the PC side.
#define	CONFIG_ARRAY_SIZE		27
#define	DATA_ARRAY_SIZE			52
#define	COMMAND_COUNT			10

#define HIDDEN_ARRAY_SIZE		127

// Define address ranges for interpreting incoming packet data
#define	CONFIG_REG_START_ADDRESS	0
#define	DATA_REG_START_ADDRESS		85
#define	COMMAND_START_ADDRESS		170

// Definitions of configuration registers/address
#define CREG_COM_SETTINGS			0
#define	CREG_COM_RATES1				1
#define CREG_COM_RATES2				2
#define CREG_COM_RATES3				3
#define CREG_COM_RATES4				4
#define CREG_COM_RATES5				5
#define CREG_COM_RATES6				6
#define CREG_COM_RATES7				7
#define CREG_MISC_SETTINGS			8
#define CREG_HOME_NORTH				9
#define CREG_HOME_EAST				10
#define CREG_HOME_UP				11
#define CREG_GYRO_TRIM_X			12		// Floating point trim (actual angular rate)
#define CREG_GYRO_TRIM_Y			13
#define CREG_GYRO_TRIM_Z			14
#define CREG_MAG_CAL1_1				15
#define CREG_MAG_CAL1_2				16
#define CREG_MAG_CAL1_3				17
#define CREG_MAG_CAL2_1				18
#define CREG_MAG_CAL2_2				19
#define CREG_MAG_CAL2_3				20
#define CREG_MAG_CAL3_1				21
#define CREG_MAG_CAL3_2				22
#define CREG_MAG_CAL3_3				23
#define CREG_MAG_BIAS_X				24
#define CREG_MAG_BIAS_Y				25
#define CREG_MAG_BIAS_Z				26

// Bit definitions for COM_SETTINGS register
#define COM_BAUD_MASK				0x0F			// Uses 4 bits
#define COM_BAUD_START				28				// Lowest-order bit on bit 28

#define COM_GPS_BAUD_MASK           0x0F
#define COM_GPS_BAUD_START          24

#define COM_GPS_DATA_ENABLED		(1 << 8)
#define COM_GPS_SAT_DATA_ENABLED	(1 << 4)

// Definitions for controlling the baud rate of the USART
#define BAUD_9600					0
#define BAUD_14400					1
#define BAUD_19200					2
#define BAUD_38400					3
#define BAUD_57600					4
#define BAUD_115200					5
#define BAUD_128000					6
#define BAUD_153600					7
#define BAUD_230400					8
#define BAUD_256000					9
#define BAUD_460800					10
#define BAUD_921600					11

// Bit definitions for COM_RATE registers
#define RATE1_RAW_ACCEL_MASK		0x0FF
#define RATE1_RAW_ACCEL_START		24
#define RATE1_RAW_GYRO_MASK			0x0FF
#define RATE1_RAW_GYRO_START		16
#define RATE1_RAW_MAG_MASK			0x0FF
#define RATE1_RAW_MAG_START			8

#define RATE2_TEMPERATURE_MASK		0x0FF
#define RATE2_TEMPERATURE_START		24
#define RATE2_ALL_RAW_MASK			0x0FF
#define RATE2_ALL_RAW_START			0

#define RATE3_PROC_ACCEL_MASK		0x0FF
#define RATE3_PROC_ACCEL_START		24
#define RATE3_PROC_GYRO_MASK		0x0FF
#define RATE3_PROC_GYRO_START		16
#define RATE3_PROC_MAG_MASK			0x0FF
#define RATE3_PROC_MAG_START		8

#define RATE4_ALL_PROC_MASK			0x0FF
#define RATE4_ALL_PROC_START		0

#define RATE5_QUAT_MASK				0x0FF
#define RATE5_QUAT_START			24
#define RATE5_EULER_MASK			0x0FF
#define RATE5_EULER_START			16
#define RATE5_POSITION_MASK			0x0FF
#define RATE5_POSITION_START		8
#define RATE5_VELOCITY_MASK			0x0FF
#define RATE5_VELOCITY_START		0

#define RATE6_POSE_MASK				0x0FF
#define RATE6_POSE_START			24
#define RATE6_HEALTH_MASK			0x0F
#define RATE6_HEALTH_START			16

#define RATE7_NMEA_HEALTH_MASK		0x0F
#define RATE7_NMEA_HEALTH_START		28
#define RATE7_NMEA_POSE_MASK		0x0F
#define RATE7_NMEA_POSE_START		24
#define RATE7_NMEA_ATTITUDE_MASK	0x0F
#define RATE7_NMEA_ATTITUDE_START	20
#define RATE7_NMEA_SENSOR_MASK		0x0F
#define RATE7_NMEA_SENSOR_START		16
#define RATE7_NMEA_RATE_MASK		0x0F
#define RATE7_NMEA_RATE_START		12
#define RATE7_NMEA_GPS_POSE_MASK	0x0F
#define RATE7_NMEA_GPS_POSE_START	8
#define RATE7_NMEA_QUATERNION_MASK	0x0F
#define RATE7_NMEA_QUATERNION_START	4

// Bit definitions for filter settings register.
#define MAG_UPDATES_ENABLED			(1 << 0)
#define QUATERNION_MODE_ENABLED     (1 << 1)
#define ZERO_GYROS_ON_STARTUP       (1 << 2)
#define USE_TX2_AS_PPS_INPUT		(1 << 8)

// Definitions of data registers/addresses
#define DREG_HEALTH					85
#define DREG_GYRO_RAW_XY			86
#define DREG_GYRO_RAW_Z				87
#define DREG_GYRO_TIME				88
#define DREG_ACCEL_RAW_XY			89
#define DREG_ACCEL_RAW_Z			90
#define DREG_ACCEL_TIME				91
#define DREG_MAG_RAW_XY				92
#define DREG_MAG_RAW_Z				93
#define DREG_MAG_RAW_TIME			94
#define DREG_TEMPERATURE    		95
#define DREG_TEMPERATURE_TIME		96
#define DREG_GYRO_PROC_X			97
#define DREG_GYRO_PROC_Y			98
#define DREG_GYRO_PROC_Z			99
#define DREG_GYRO_PROC_TIME			100
#define DREG_ACCEL_PROC_X			101
#define DREG_ACCEL_PROC_Y			102
#define DREG_ACCEL_PROC_Z			103
#define DREG_ACCEL_PROC_TIME		104
#define DREG_MAG_PROC_X				105
#define DREG_MAG_PROC_Y				106
#define DREG_MAG_PROC_Z				107
#define DREG_MAG_PROC_TIME			108
#define DREG_QUAT_AB				109
#define DREG_QUAT_CD				110
#define DREG_QUAT_TIME				111
#define DREG_EULER_PHI_THETA		112
#define DREG_EULER_PSI				113
#define DREG_EULER_PHI_THETA_DOT	114
#define DREG_EULER_PSI_DOT			115
#define DREG_EULER_TIME				116
#define DREG_POSITION_NORTH			117
#define DREG_POSITION_EAST			118
#define DREG_POSITION_UP			119
#define DREG_POSITION_TIME			120
#define DREG_VELOCITY_NORTH			121
#define DREG_VELOCITY_EAST			122
#define DREG_VELOCITY_UP			123
#define DREG_VELOCITY_TIME			124
#define DREG_GPS_LATITUDE			125
#define DREG_GPS_LONGITUDE			126
#define DREG_GPS_ALTITUDE			127
#define DREG_GPS_COURSE				128
#define DREG_GPS_SPEED				129
#define DREG_GPS_TIME				130
#define DREG_GPS_SAT_1_2			131
#define DREG_GPS_SAT_3_4			132
#define DREG_GPS_SAT_5_6			133
#define DREG_GPS_SAT_7_8			134
#define DREG_GPS_SAT_9_10			135
#define DREG_GPS_SAT_11_12			136

// Bit definitions for sensor health register
#define HEALTH_SATS_USED_MASK		0x3F		// Uses 6 bits
#define HEALTH_SATS_USED_START		26			// Lowest-order bit starts at 26
#define HEALTH_HDOP_MASK			0x3FF		// Uses 10 bits
#define HEALTH_HDOP_START			16			// Lowest-order bit starts at 16
#define HEALTH_SATS_IN_VIEW_MASK	0x3F		
#define HEALTH_SATS_IN_VIEW_START	10
#define HEALTH_COM_OVERFLOW			(1 << 8)	// Set when the sensor was unable to transmit all the requested data
#define HEALTH_MAG_NORM				(1 << 5)
#define HEALTH_ACCEL_NORM			(1 << 4)
#define HEALTH_ACCEL				(1 << 3)
#define HEALTH_GYRO					(1 << 2)
#define HEALTH_MAG					(1 << 1)
#define HEALTH_GPS					(1 << 0)

// Definition of packet address for COM error codes
#define	CHR_BAD_CHECKSUM			253								// Sent if the module receives a packet with a bad checksum
#define	CHR_UNKNOWN_ADDRESS			254								// Sent if the module receives a packet with an unknown address
#define	CHR_INVALID_BATCH_SIZE		255								// Sent if a requested batch read or write operation would go beyond the bounds of the config or data array

// Command addresses
#define	CHR_GET_FW_VERSION	     COMMAND_START_ADDRESS			// Causes the device to report the firmware revision
#define	CHR_FLASH_COMMIT		(COMMAND_START_ADDRESS + 1)		// Causes the device to write all configuration values to FLASH
#define	CHR_RESET_TO_FACTORY	(COMMAND_START_ADDRESS + 2)		// Causes the UM6 to load default factory settings
#define CHR_ZERO_GYROS			(COMMAND_START_ADDRESS + 3)
#define CHR_SET_HOME_POSITION	(COMMAND_START_ADDRESS + 4)
#define CHR_FACTORY_COMMIT		(COMMAND_START_ADDRESS + 5)
#define CHR_SET_MAG_REFERENCE	(COMMAND_START_ADDRESS + 6)

#define CHR_RESET_EKF			(COMMAND_START_ADDRESS + 9)

// Some definitions for working with COM events
#define NMEA_HEALTH_PACKET		0
#define NMEA_POSE_PACKET		1
#define NMEA_ATTITUDE_PACKET	2
#define NMEA_SENSOR_PACKET		3
#define NMEA_RATE_PACKET		4
#define NMEA_GPS_POSE_PACKET	5
#define NMEA_QUATERNION_PACKET	6

// Definition for zeroing rate gyros and pressure sensor
#define GYRO_ZERO_SAMPLES		500
#define GYRO_X_INDEX			0
#define GYRO_Y_INDEX			1
#define GYRO_Z_INDEX			2
#define ANGLE_ZERO_SAMPLES		1000
#define ACCEL_X_INDEX			0
#define ACCEL_Y_INDEX			1
#define ACCEL_Z_INDEX			2
#define PRESSURE_ZERO_SAMPLES	250
#define AIRSPEED_ZERO_SAMPLES	500

#define PI						3.14159265f
#define PIx2                    6.28318530f
#define PId2                    1.57079633f
#define GRAVITY					9.80665f
#define GRAVITYx2				19.6133f
#define GRAVITYd2               4.903325f
#define RAW_TO_MBAR             (1.0f/4096.0f)
#define KPA_PER_VOLT            1.0f

#define	CHR_USE_CONFIG_ADDRESS		0
#define	CHR_USE_FACTORY_ADDRESS		1

// Start address for writing configuration to FLASH
#define	FLASH_START_ADDRESS	(uint32_t)0x0800F000
// Start address for factory FLASH configuration
#define	FACTORY_FLASH_ADDRESS	(uint32_t)0x0800E000

// Macro for determining whether FLASH has been initialized
#define		FGET_FLASH_UNINITIALIZED()		((uint32_t)( *(__IO uint32_t*)(FLASH_START_ADDRESS) ) == 0xFFFFFFFF)
#define		FGET_FACTORY_UNINITIALIZED()	((uint32_t)( *(__IO uint32_t*)(FACTORY_FLASH_ADDRESS) ) == 0xFFFFFFFF)

#define	GYRO_ZERO_SAMPLE_SIZE	500

typedef struct __CHR_config 
{
    union
    {
	    uint32_t r[CONFIG_ARRAY_SIZE];
        float    f[CONFIG_ARRAY_SIZE];
    };
} CHR_config;

typedef struct __CHR_data
{
	 union
     {
        uint32_t r[DATA_ARRAY_SIZE];
        float    f[DATA_ARRAY_SIZE];
    };
} CHR_data;


