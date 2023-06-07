#include <stdio.h>
#include <unistd.h>
#include "coroutine.h"
#include "cube.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <softTone.h>

#include "rpi_ws281x/ws2811.h"

#define LED_COUNT 9
#define LED_PIN 18

static ws2811_t ledstring = {
    .freq = WS2811_TARGET_FREQ,
    .dmanum = 5,
    .channel = {
        [0] = {
            .gpionum = LED_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 255,
        },
        [1] = {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};

#define CHANNEL     0       // SPI 채널 번호

/* Device Address/Identifier for MPU6050 */
#define Device_Address 0x68	   

#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

#define NUMBER_OF_SHIFT_CHIPS   2
/* Width of data (how many ext lines). */
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8
/* Width of pulse to trigger the shift register to read and latch. */
#define PULSE_WIDTH_USEC   5
/* Optional delay between shift register reads. */
#define POLL_DELAY_MSEC   1
/* You will need to change the "int" to "long" If the
 * NUMBER_OF_SHIFT_CHIPS is higher than 2. */
#define BYTES_VAL_T unsigned long long

static int fd;

static int ploadPin        =  8;  // Connects to Parallel load pin the 165
static int dataPin         =  9;  // Connects to the Q7 pin the 165
static int clockPin        = 11;  // Connects to the Clock pin the 165

static const int pinPiezo = 6;

static BYTES_VAL_T pinValues;
static BYTES_VAL_T oldPinValues;

/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long). */
BYTES_VAL_T read_shift_regs() {
    long bitVal;
    BYTES_VAL_T bytesVal = 0;

    /* Trigger a parallel Load to latch the state of the data lines, */
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);

    /* Loop to read each bit value from the serial out line
     * of the SN74HC165N. */
    for(int i = 0; i < DATA_WIDTH; i++) {
        bitVal = digitalRead(dataPin);

        /* Set the corresponding bit in bytesVal. */
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

        /* Pulse the Clock (rising edge shifts the next bit). */
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(clockPin, LOW);
    }

    return(bytesVal);
}

// Implement Terminal Output
void TerminalOutput() {
    system("clear");
    printf("\n");
    int face = 4;
    for(int lednum=0; lednum<9; lednum++) {
        if(lednum % 3 == 0) printf("    ");
        Color ledcolor = GetCurrColor(face, lednum);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            ledcolor.r, ledcolor.g, ledcolor.b, "■");
        if(lednum % 3 == 2) printf("\n");
    }
    face = 3;
    int lednum = 0;
    while(lednum < 9) {
        Color ledcolor = GetCurrColor(face, lednum);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            ledcolor.r, ledcolor.g, ledcolor.b, "■");
        if(lednum % 3 == 2) {
            if(face == 2) {
                printf("\n");
                lednum++;
                face = 3;
            }
            else {
                printf(" ");
                lednum -= 2;
                face = (face + 1) % 4;
            }
        }
        else lednum++;
    }
    face = 5;
    for(int lednum=0; lednum<9; lednum++) {
        if(lednum % 3 == 0) printf("    ");
        Color ledcolor = GetCurrColor(face, lednum);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            ledcolor.r, ledcolor.g, ledcolor.b, "■");
        if(lednum % 3 == 2) printf("\n");
    }
}

void MPU6050_Init() {
	wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0x07);	/* Write to sample rate register */
	wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x01);	/* Write to power management register */
	wiringPiI2CWriteReg8 (fd, CONFIG, 0);		/* Write to Configuration register */
	wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, 24);	/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8 (fd, INT_ENABLE, 0x01);	/*Write to interrupt enable register */
}

short read_raw_data(int addr) {
	short high_byte,low_byte,value;
	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr+1);
	value = (high_byte << 8) | low_byte;
	return value;
}

static const int aMelody[8] = {131,147,165,175,196,220,247,262};

void tone(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);

	softToneWrite(pinPiezo, aMelody[coroutine->level]);
    WAIT_FOR_MILISEC(coroutine, coroutine->waitms);
    softToneWrite(pinPiezo,0);

    END_COROUTINE(coroutine);
}

void UpdateLED() {
    int face = 0;

    for(int lednum=0; lednum<9; lednum++) {
        Color ledcolor = GetCurrColor(face, lednum);
        ledstring.channel[0].leds[lednum] = (ledcolor.r << 8) | (ledcolor.g << 16) | ledcolor.b;
    }

    // Neopixel 업데이트
    if (ws2811_render(&ledstring) < 0) {
        fprintf(stderr, "ws2811_render failed\n");
    }
}

void StartCubeRoutine() {
    InitCoroutines();
    CubeInit();
    srand(time(NULL));

    wiringPiSetup();
    wiringPiSetupGpio();
    wiringPiSPISetup(CHANNEL, 500000);

    if (ws2811_init(&ledstring) < 0) {
        fprintf(stderr, "ws2811_init failed\n");
    }

    // float Acc_x,Acc_y,Acc_z;
	// float Gyro_x,Gyro_y,Gyro_z;
	// float Ax=0, Ay=0, Az=0;
	// float Gx=0, Gy=0, Gz=0;
	// fd = wiringPiI2CSetup(Device_Address);   /*Initializes I2C with device Address*/
	// MPU6050_Init();     /* Initializes MPU6050 */

    pinMode(ploadPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);

    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);
	softToneCreate(pinPiezo);

    pinValues = read_shift_regs();
    oldPinValues = pinValues;

    Start();
    // tone();

    int order[] = {2, 1, 0, 5, 4, 3, 8, 7, 6};

    while (true) {
        CallCoroutines();

        pinValues = read_shift_regs();
        pinValues = pinValues;
        if(pinValues != oldPinValues) {
            // if(pinValues & 0xFF) ButtonDown(0, 8);
            for(int i = 8; i < DATA_WIDTH; i++) {
                int face = i / 8 - 1;
                int lednum = order[i % 8];
                if((pinValues >> i) & 1)
                    ButtonDown(face, lednum);
                else
                    ButtonUp(face, lednum);
            }
            oldPinValues = pinValues;
        }
        printf("%llx", pinValues);
        
        /*Read raw value of Accelerometer and gyroscope from MPU6050*/
		// Acc_x = read_raw_data(ACCEL_XOUT_H);
		// Acc_y = read_raw_data(ACCEL_YOUT_H);
		// Acc_z = read_raw_data(ACCEL_ZOUT_H);
		
		// Gyro_x = read_raw_data(GYRO_XOUT_H);
		// Gyro_y = read_raw_data(GYRO_YOUT_H);
		// Gyro_z = read_raw_data(GYRO_ZOUT_H);
		
		// /* Divide raw value by sensitivity scale factor */
		// Ax = Acc_x/16384.0;
		// Ay = Acc_y/16384.0;
		// Az = Acc_z/16384.0;
		
		// Gx = Gyro_x/131;
		// Gy = Gyro_y/131;
		// Gz = Gyro_z/131;
		
		// printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\n",Gx,Gy,Gz,Ax,Ay,Az);

        // Terminal Version
        TerminalOutput();

        // Real LED Version
        UpdateLED();

        /* ==================================================== */

        Update();
        delay(deltaTime);
    }

    // 종료 시 정리
    ws2811_fini(&ledstring);
}