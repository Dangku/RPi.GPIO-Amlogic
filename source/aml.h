/*
*  Bananapi specific code borrowed from Bananapi's wiringPi port
*/

// License and info from Hardkernel's original file:
/*
 * wiringPi:
 *	Arduino compatable (ish) Wiring library for the Raspberry Pi
 *	Copyright (c) 2012 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifndef AML_H_INCLUDED
#define AML_H_INCLUDED

/* start wiringPi.h code */

#define	PI_MODEL_UNKNOWN  0
#define	PI_MODEL_A        1
#define	PI_MODEL_B        2
#define	PI_MODEL_BP       3
#define	PI_MODEL_CM       4
#define	PI_MODEL_AP       5
#define	PI_MODEL_BANANAPIM5  	6
#define  PI_MODEL_BANANAPIM2S  7

// Failure modes

#define	WPI_FATAL	(1==1)
#define	WPI_ALMOST	(1==2)

/* end wiringPi.h code */


/* start wiringPi.c code */

#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(1==2)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

//
// For Bananapi M2S Board
//
#define M2S_GPIO_MASK			(0xFFFFFF00)
#define M2S_GPIO_BASE			0xff634000
#define M2S_GPIO_AO_BASE		0xFF800000

#define M2S_GPIO_PIN_BASE		410

#define M2S_GPIOH_PIN_START	(M2S_GPIO_PIN_BASE + 17) // GPIOH_0
#define M2S_GPIOH_PIN_END		(M2S_GPIO_PIN_BASE + 25) // GPIOH_8
#define M2S_GPIOA_PIN_START	(M2S_GPIO_PIN_BASE + 50) // GPIOA_0
#define M2S_GPIOA_PIN_END		(M2S_GPIO_PIN_BASE + 65) // GPIOA_15
#define M2S_GPIOX_PIN_START	(M2S_GPIO_PIN_BASE + 66) // GPIOX_0
#define M2S_GPIOX_PIN_MID		(M2S_GPIO_PIN_BASE + 81) // GPIOX_15
#define M2S_GPIOX_PIN_END		(M2S_GPIO_PIN_BASE + 85) // GPIOX_19
#define M2S_GPIOAO_PIN_START	(M2S_GPIO_PIN_BASE + 86) // GPIOAO_0
#define M2S_GPIOAO_PIN_END	(M2S_GPIO_PIN_BASE + 97) // GPIOAO_11

#define M2S_GPIOH_FSEL_REG_OFFSET		0x119
#define M2S_GPIOH_OUTP_REG_OFFSET		0x11A
#define M2S_GPIOH_INP_REG_OFFSET		0x11B
#define M2S_GPIOH_PUPD_REG_OFFSET		0x13D
#define M2S_GPIOH_PUEN_REG_OFFSET		0x14B
#define M2S_GPIOH_DS_REG_3A_OFFSET      	0x1D4
#define M2S_GPIOH_MUX_B_REG_OFFSET      	0x1BB

#define M2S_GPIOX_FSEL_REG_OFFSET		0x116
#define M2S_GPIOX_OUTP_REG_OFFSET		0x117
#define M2S_GPIOX_INP_REG_OFFSET		0x118
#define M2S_GPIOX_PUPD_REG_OFFSET		0x13C
#define M2S_GPIOX_PUEN_REG_OFFSET		0x14A
#define M2S_GPIOX_DS_REG_2A_OFFSET      	0x1D2
#define M2S_GPIOX_DS_REG_2B_OFFSET      	0x1D3
#define M2S_GPIOX_MUX_3_REG_OFFSET      	0x1B3
#define M2S_GPIOX_MUX_4_REG_OFFSET      	0x1B4
#define M2S_GPIOX_MUX_5_REG_OFFSET      	0x1B5

#define M2S_GPIOA_FSEL_REG_OFFSET		0x120
#define M2S_GPIOA_OUTP_REG_OFFSET		0x121
#define M2S_GPIOA_INP_REG_OFFSET		0x122
#define M2S_GPIOA_PUPD_REG_OFFSET		0x13F
#define M2S_GPIOA_PUEN_REG_OFFSET		0x14D
#define M2S_GPIOA_DS_REG_5A_OFFSET      	0x1D6
#define M2S_GPIOA_MUX_D_REG_OFFSET      	0x1BD
#define M2S_GPIOA_MUX_E_REG_OFFSET      	0x1BE

#define M2S_GPIOAO_FSEL_REG_OFFSET		0x109
#define M2S_GPIOAO_OUTP_REG_OFFSET		0x10D
#define M2S_GPIOAO_INP_REG_OFFSET		0x10A
#define M2S_GPIOAO_PUPD_REG_OFFSET		0x10B
#define M2S_GPIOAO_PUEN_REG_OFFSET		0x10C
#define M2S_GPIOAO_DS_REG_A_OFFSET      	0x107
#define M2S_GPIOAO_DS_REG_B_OFFSET      	0x108
#define M2S_GPIOAO_MUX_REG0_OFFSET      	0x105
#define M2S_GPIOAO_MUX_REG1_OFFSET      	0x106

//
// For Bananapi M5 Board
//
#define M5_GPIO_MASK			(0xFFFFFF00)
#define M5_GPIO_BASE			0xFF634000
#define M5_GPIO_AO_BASE			0xFF800000

#define M5_GPIO_PIN_BASE		410

#define M5_GPIOH_PIN_START		(M5_GPIO_PIN_BASE + 17)	// GPIOH_0
#define M5_GPIOH_PIN_END		(M5_GPIO_PIN_BASE + 25)	// GPIOH_8
#define M5_GPIOA_PIN_START		(M5_GPIO_PIN_BASE + 50)	// GPIOA_0
#define M5_GPIOA_PIN_END		(M5_GPIO_PIN_BASE + 65)	// GPIOA_15
#define M5_GPIOX_PIN_START		(M5_GPIO_PIN_BASE + 66)	// GPIOX_0
#define M5_GPIOX_PIN_MID		(M5_GPIO_PIN_BASE + 81)   // GPIOX_15
#define M5_GPIOX_PIN_END		(M5_GPIO_PIN_BASE + 85)	// GPIOX_15
#define M5_GPIOAO_PIN_START		(M5_GPIO_PIN_BASE + 86)   // GPIOAO_0
#define M5_GPIOAO_PIN_END		(M5_GPIO_PIN_BASE + 97)   // GPIOAO_11

#define M5_GPIOH_FSEL_REG_OFFSET		0x119
#define M5_GPIOH_OUTP_REG_OFFSET		0x11A
#define M5_GPIOH_INP_REG_OFFSET			0x11B
#define M5_GPIOH_PUPD_REG_OFFSET		0x13D
#define M5_GPIOH_PUEN_REG_OFFSET		0x14B
#define M5_GPIOH_DS_REG_3A_OFFSET       	0x1D4
#define M5_GPIOH_MUX_B_REG_OFFSET       	0x1BB

#define M5_GPIOA_FSEL_REG_OFFSET		0x120
#define M5_GPIOA_OUTP_REG_OFFSET		0x121
#define M5_GPIOA_INP_REG_OFFSET			0x122
#define M5_GPIOA_PUPD_REG_OFFSET		0x13F
#define M5_GPIOA_PUEN_REG_OFFSET		0x14D
#define M5_GPIOA_DS_REG_5A_OFFSET       	0x1D6
#define M5_GPIOA_MUX_D_REG_OFFSET       	0x1BD
#define M5_GPIOA_MUX_E_REG_OFFSET       	0x1BE

#define M5_GPIOX_FSEL_REG_OFFSET		0x116
#define M5_GPIOX_OUTP_REG_OFFSET		0x117
#define M5_GPIOX_INP_REG_OFFSET			0x118
#define M5_GPIOX_PUPD_REG_OFFSET		0x13C
#define M5_GPIOX_PUEN_REG_OFFSET		0x14A
#define M5_GPIOX_DS_REG_2A_OFFSET       	0x1D2
#define M5_GPIOX_DS_REG_2B_OFFSET       	0x1D3
#define M5_GPIOX_MUX_3_REG_OFFSET       	0x1B3
#define M5_GPIOX_MUX_4_REG_OFFSET       	0x1B4
#define M5_GPIOX_MUX_5_REG_OFFSET       	0x1B5

#define M5_GPIOAO_FSEL_REG_OFFSET		0x109
#define M5_GPIOAO_OUTP_REG_OFFSET		0x10D
#define M5_GPIOAO_INP_REG_OFFSET		0x10A
#define M5_GPIOAO_PUPD_REG_OFFSET		0x10B
#define M5_GPIOAO_PUEN_REG_OFFSET		0x10C
#define M5_GPIOAO_DS_REG_A_OFFSET       	0x107
#define M5_GPIOAO_DS_REG_B_OFFSET       	0x108
#define M5_GPIOAO_MUX_REG0_OFFSET       	0x105
#define M5_GPIOAO_MUX_REG1_OFFSET       	0x106

#ifdef DEFINE_AML_VARS

//From c_gpio.c and c_gpio.h
#define PAGE_SIZE  (4*1024)
#define BLOCK_SIZE (4*1024)

#define INPUT  1 // is really 0 for control register!
#define OUTPUT 0 // is really 1 for control register!
#define ALT0   4

#define HIGH 1
#define LOW  0

#define PUD_OFF  0
#define PUD_DOWN 1
#define PUD_UP   2
//End from c_gpio.c and c_gpio.h

//From common.h
#define MAXPINCOUNT 40
extern const int (*pin_to_gpio)[MAXPINCOUNT+1];
//End from common.h

int wiringPiReturnCodes = FALSE ;

static volatile uint32_t *gpio, *gpioao;

// pinToGpio:
//	Take a Wiring pin (0 through X) and re-map it to the BCM_GPIO pin
//	Cope for 3 different board revisions here.
//static int *pinToGpio ;
//static int pin_array_count;

// physToGpio:
//	Take a physical pin (1 through 26) and re-map it to the BCM_GPIO pin
//	Cope for 2 different board revisions here.
//	Also add in the P5 connector, so the P5 pins are 3,4,5,6, so 53,54,55,56

//static int *physToGpio ;

//
// pinToGpio:
//	Take a Wiring pin (0 through X) and re-map it to the AML_GPIO pin
//
static const int pinToGpioBananapiM2S[64] = {
	// wiringPi number to native gpio number
	479, 461,	//  0 |  1 : GPIOX.3, GPIOA.1
	480, 483,	//  2 |  3 : GPIOX.4, GPIOX.7(PWM_F)
	476, 477,	//  4 |  5 : GPIOX.0, GPIOX.1
	478, 481,	//  6 |  7 : GPIOX.2, GPIOX.5(PWM_C)
	493, 494,	//  8 |  9 : GPIOX.17(I2C-2_SDA), GPIOX.18(I2C-2_SCL)
	486, 492,	// 10 | 11 : GPIOX.10(SPI_SS), GPIOX.16(PWM_E)
	484, 485,	// 12 | 13 : GPIOX.8(SPI_MOSI), GPIOX.9(SPI_MISO)
	487, 488,	// 14 | 15 : GPIOX.11(SPI_CLK), GPIOX.12(UART_A_TX)
	489,  -1,	// 16 | 17 : GPIOX.13(UART_A_RX),
	 -1,  -1,	// 18 | 19 :
	 -1, 490,	// 20 | 21 : , GPIOX.14
	491, 482,	// 22 | 23 : GPIOX.15, GPIOX.6(PWM_D)
	462, 460,	// 24 | 25 : GPIOA.2, GPIOA.0
	495, 501,	// 26 | 27 : GPIOX.19, GPIOAO.5
	464, 463,	// 28 | 29 : GPIOA.4, GPIOA.3
	432, 431,	// 30 | 31 : GPIOH.5, GPIOH_4
	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 32...47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 48...63

};

static const int physToGpioBananapiM2S[64] = {
	// physical header pin number to native gpio number
	 -1,		//  0
	 -1,  -1,	//  1 |  2 : 3.3V, 5.0V
	493,  -1,	//  3 |  4 : GPIOX.17(I2C-2_SDA), 5.0V
	494,  -1,	//  5 |  6 : GPIOX.18(I2C-2_SCL), GND
	481, 488,	//  7 |  8 : GPIOX.5(PWM_C), GPIOX.12(UART_A_TX)
	 -1, 489,	//  9 | 10 : GND, GPIOX.13(UART_A_RX)
	479, 461,	// 11 | 12 : GPIOX.3, GPIOA.1
	480,  -1,	// 13 | 14 : GPIOX.4, GND
	483, 476,	// 15 | 16 : GPIOX.7(PWM_F), GPIOX.0
	 -1, 477,	// 17 | 18 : 3.3V, GPIOX.1
	484,  -1,	// 19 | 20 : GPIOX.8(SPI_MOSI), GND
	485, 478,	// 21 | 22 : GPIOX.9(SPI_MISO), GPIOX.2
	487, 486,	// 23 | 24 : GPIOX.11(SPI_CLK), GPIOX.10(SPI_SS)
	 -1, 492,	// 25 | 26 : GND, GPIOX.16(PWM_E)
	432, 431,	// 27 | 28 : GPIOH_5, GPIOH_4
	490,  -1,	// 29 | 30 : GPIOX.14, GND
	491, 495,	// 31 | 32 : GPIOX.15, GPIOX.19
	482,  -1,	// 33 | 34 : GPIOX.6(PWM_D), GND
	462, 501,	// 35 | 36 : GPIOA.2, GPIOAO.5
	460, 464,	// 37 | 38 : GPIOA.0, GPIOA.4
	 -1, 463,	// 39 | 40 : GND, GPIOA.3
	// Not used
	-1, -1, -1, -1, -1, -1, -1, -1,	// 41...48
	-1, -1, -1, -1, -1, -1, -1, -1,	// 49...56
	-1, -1, -1, -1, -1, -1, -1	// 57...63
};

static const int pinToGpioBananapiM5[64] = {
    // wiringPi number to native gpio number
	479, 504,	//  0 |  1 : GPIOX.3, GPIOAO.8
	480, 483,	//  2 |  3 : GPIOX.4, GPIOX.7(PWM_F)
	476, 477,	//  4 |  5 : GPIOX.0, GPIOX.1
	478, 481,	//  6 |  7 : GPIOX.2, GPIOX.5PWM_C)
	493, 494,	//  8 |  9 : GPIOX.17(I2C-2_SDA), GPIOX.18(I2C-2_SCL)
	486, 492,	// 10 | 11 : GPIOX.10(SPI_SS), GPIOX.16(PWM_E)
	484, 485,	// 12 | 13 : GPIOX.8(SPI_MOSI), GPIOX.9(SPI_MISO)
	487, 488,	// 14 | 15 : GPIOX.11(SPI_CLK), GPIOX.12(UART_A_TX)
	489,  -1,	// 16 | 17 : GPIOX.13(UART_A_RX),
	 -1,  -1,	// 18 | 19 :
	 -1, 490,	// 20 | 21 : , GPIOX.14
	491, 482,	// 22 | 23 : GPIOX.15, GPIOX.6(PWM_D)
	503, 505,	// 24 | 25 : GPIOAO.7, GPIOAO.9
	495, 432,	// 26 | 27 : GPIOX.19, GPIOH.5
	506, 500,	// 28 | 29 : GPIOAO.10, GPIOAO.4
	474, 475,	// 30 | 31 : GPIOA.14(I2C-3_SDA), GPIOA.15(I2C-3_SCL)
	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 32...47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 48...63
};

static const int physToGpioBananapiM5[64] = {
	// physical header pin number to native gpio number
	 -1,		//  0
	 -1,  -1,	//  1 |  2 : 3.3V, 5.0V
	493,  -1,	//  3 |  4 : GPIOX.17(I2C-2_SDA), 5.0V
	494,  -1,	//  5 |  6 : GPIOX.18(I2C-2_SCL), GND
	481, 488,	//  7 |  8 : GPIOX.5, GPIOX.12(UART_A_TX)
	 -1, 489,	//  9 | 10 : GND, GPIOX.13(UART_A_RX)
	479, 504,	// 11 | 12 : GPIOX.3, GPIOAO.8
	480,  -1,	// 13 | 14 : GPIOX.4, GND
	483, 476,	// 15 | 16 : GPIOX.7, GPIOX.0
	 -1, 477,	// 17 | 18 : 3.3V, GPIOX.1
	484,  -1,	// 19 | 20 : GPIOX.8(SPI_MOSI), GND
	485, 478,	// 21 | 22 : GPIOX.9(SPI_MISO), GPIOX.2
	487, 486,	// 23 | 24 : GPIOX.11(SPI_CLK), GPIOX.10(SPI_SS)
	 -1, 492,	// 25 | 26 : GND, GPIOX.16
	474, 475,	// 27 | 28 : GPIOA.14(I2C-3_SDA), GPIOA.15(I2C-3_SCL)
	490,  -1,	// 29 | 30 : GPIOX.14, GND
	491, 495,	// 31 | 32 : GPIOX.15, GPIOX.19
	482,  -1,	// 33 | 34 : GPIOX.6(PWM_D), GND
	503, 432,	// 35 | 36 : GPIOAO.7, GPIOH.5
	505, 506,	// 37 | 38 : GPIOAO.9, GPIOAO.10
	 -1, 500,	// 39 | 40 : GND, GPIOAO.4
	// Not used
	-1, -1, -1, -1, -1, -1, -1, -1,	// 41...48
	-1, -1, -1, -1, -1, -1, -1, -1,	// 49...56
	-1, -1, -1, -1, -1, -1, -1	// 57...63

};
/* end wiringPi.c code */


/* Non-static add extern definition below */
int aml_found;
int  piModel;

const int bcmToOGpioBananapiM2S[64] = {	// BCM ModE
     -1,  -1, 493, 494, 481, 490, 491, 492, // 0..7
    486, 485, 484, 487, 495, 482, 488, 489, // 8..15
    501, 479, 461, 462,  -1,  -1, 483, 476, // 16..23
    477, 478,  -1, 480,  -1,  -1,  -1,  -1, // 24..31
// Padding:
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 32..39
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 40..47
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 48..55
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1  // 56..63
};

const int bcmToOGpioBananapiM5[64] = {	// BCM ModE
     -1,  -1, 493, 494, 481, 490, 491, 492, // 0..7
    486, 485, 484, 487, 495, 482, 488, 489, // 8..15
    432, 479, 504, 503,  -1,  -1, 483, 476, // 16..23
    477, 478,  -1, 480,  -1,  -1,  -1,  -1, // 24..31
// Padding:
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 32..39
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 40..47
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, // 48..55
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1  // 56..63
};

const int bcmToOGpioRPi[64] = {	// BCM ModE
      0,   1,   2,   3,   4,   5,   6,   7, // 0..7
      8,   9,  10,  11,  12,  13,  14,  15, // 8..15
     16,  17,  16,  19,  20,  21,  22,  23, // 16..23
     24,  25,  26,  27,  28,  29,  30,  31, // 24..31
// Padding:
     32,  33,  34,  35,  36,  37,  38,  39, // 32..39
     40,  41,  42,  43,  44,  45,  46,  47, // 40..47
     48,  49,  50,  51,  52,  53,  54,  55, // 48..55
     56,  57,  58,  59,  60,  61,  62,  63  // 56..63
};

const int (*bcm_to_amlgpio)[64];


#else /* DEFINE_AML_VARS */

extern int aml_found;
extern int  piModel;
extern const int physToGpioBananapiM2S[64];
extern const int physToGpioBananapiM5[64];
extern const int bcmToOGpioBananapiM2S[64];
extern const int bcmToOGpioBananapiM5[64];
extern const int bcmToOGpioRPi[64];
extern const int (*bcm_to_amlgpio)[64];

int wiringPiSetupAml (void);
void wiringPiCleanupAml (void);
void pinModeAml (int pin, int mode);
void pullUpDnControlAml (int pin, int pud);
int digitalReadAml (int pin);
void digitalWriteAml (int pin, int value);
int analogReadAml (int pin);
void analogWriteAml (int pin, int value);
int pinGetModeAml (int pin);
void setInfoAml(char *hardware, void *vinfo);
void setMappingPtrsAml(void);

#endif /* DEFINE_AML_VARS */


#endif /* AML_H_INCLUDED */
