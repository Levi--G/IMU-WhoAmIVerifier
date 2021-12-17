# IMU-WhoAmIVerifier

## Supported

Supported IMUs by this tool:
MPU6050
MPU6500
MPU9250 + AK8963
MPU9255 + AK8963
ICM-20600
ICM-20601
ICM-20602
ICM-20608-G
ICM-20609
ICM-20648
ICM-20649
ICM-20689
ICM-20690
ICM-20948 + AK09916
ICM-40627
ICM-42605
ICM-42670-P
ICM-42688-V
ICM-42688-P
ICG-20660/L
IAM-20380
IAM-20680
IAM-20680HP
IIM-42652
BNO055
BNO08x (WIP)

Of these the following are supported by SlimeVR:
MPU6050
MPU6500
MPU9250 without mag
BNO055
BNO08x

These are being worked on by SlimeVR:
MPU9250 + AK8963
MPU9255 + AK8963
ICM-20689
ICM-20948 + AK09916

When in trouble:
[Submit an issue](https://github.com/Levi--G/IMU-WhoAmIVerifier/issues)
or contact AnimeDev on [Discord](https://discord.gg/vs9CdTD)

## Usage

### TLDR

Upload the sketch to a D1 or similar ESP8266 board. Set the correct I2C lines in config.h and upload and monitor using the correct board using PlatformIO.
The device will scan the i2c bus and report findings every 5 seconds.

### Extended guide

- Download VS-code: [here](https://code.visualstudio.com/download)
- Install PlatformIO extension in VS-code: [like this](https://docs.platformio.org/en/latest//integration/ide/vscode.html)
- Download the [code](https://github.com/Levi--G/IMU-WhoAmIVerifier/archive/refs/heads/main.zip) somewhere, unzip and open with VS-code
- Open src/config.h and check the correct board is uncommented, (default is D1)
- Open the PlatformIO tasks and open the corerct board menu
- Make sure the device is connected and press the "upload and monitor" button
- Every 5 seconds the device will scan and report any found sensord in the terminal that opened

Example:



This is an example of connecting a MPU6050 and then a MPU6050A, which actually has a ICM-20689 instead of the advertised 6050
This is the same with SLIMEVR_SUPPORT enabled:

