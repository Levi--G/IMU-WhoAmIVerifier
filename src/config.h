//BOARD: uncomment or change pins for the right board
//D1
#define PIN_IMU_SDA D2
#define PIN_IMU_SCL D1

//ESP-01
//#define PIN_IMU_SDA 2
//#define PIN_IMU_SCL 0

//ESP32
//#define PIN_IMU_SDA 21
//#define PIN_IMU_SCL 22

//Enables reporting of the chip is supported by SlimeVR and what IMU to use in SlimeVR defines.h
#define SLIMEVR_SUPPORT true

//Enables extra logging on serial interface
#define INFO_LOGGING false

//Set boudrates, should be left alone
#define SERIAL_BAUDRATE 115200
#define I2C_SPEED 400000