/*
*  Bananapi specific code borrowed from Bananapi's wiringPi port
*/

// License and info from Hardkernel's original file:
/*
 * wiringPi:
 *	Arduino compatable (ish) Wiring library for the Raspberry Pi
 *	Copyright (c) 2012 Gordon Henderson
 *	Additional code for pwmSetClock by Chris Hall <chris@kchall.plus.com>
 *
 *	Thanks to code samples from Gert Jan van Loo and the
 *	BCM2835 ARM Peripherals manual, however it's missing
 *	the clock section /grr/mutter/
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

// Revisions:
//	19 Jul 2012:
//		Moved to the LGPL
//		Added an abstraction layer to the main routines to save a tiny
//		bit of run-time and make the clode a little cleaner (if a little
//		larger)
//		Added waitForInterrupt code
//		Added piHiPri code
//
//	 9 Jul 2012:
//		Added in support to use the /sys/class/gpio interface.
//	 2 Jul 2012:
//		Fixed a few more bugs to do with range-checking when in GPIO mode.
//	11 Jun 2012:
//		Fixed some typos.
//		Added c++ support for the .h file
//		Added a new function to allow for using my "pin" numbers, or native
//			GPIO pin numbers.
//		Removed my busy-loop delay and replaced it with a call to delayMicroseconds
//
//	02 May 2012:
//		Added in the 2 UART pins
//		Change maxPins to numPins to more accurately reflect purpose

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/mman.h>

#include "cpuinfo.h"

#define DEFINE_AML_VARS
#include "aml.h"

//****************************************************************************************************

/*
* Code copied directly from Hardkernel's wiringPi port
*/

int wiringPiFailure (int fatal, const char *message, ...)
{
  va_list argp ;
  char buffer [1024] ;

  if (!fatal && wiringPiReturnCodes)
    return -1 ;

  va_start (argp, message) ;
    vsnprintf (buffer, 1023, message, argp) ;
  va_end (argp) ;

  fprintf (stderr, "%s", buffer) ;
  exit (EXIT_FAILURE) ;

  return 0 ;
}

static int isGpioAOPin(int pin)
{
    int start = 0;
    int end = 0;

    if (piModel == PI_MODEL_BANANAPIM2S) {
        start = M2S_GPIOAO_PIN_START;
	end = M2S_GPIOAO_PIN_END;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        start = M5_GPIOAO_PIN_START;
	end = M5_GPIOAO_PIN_END;
    }
	
    if (pin >= start && pin <= end)
        return 1;
    else
        return 0;
}

//
// offset to the GPIO Pin Mux register
//
static int gpioToMuxReg (int pin)
{
    if (piModel == PI_MODEL_BANANAPIM2S) {
        switch (pin) {
        case    M2S_GPIOH_PIN_START     ...M2S_GPIOH_PIN_END:
                return  M2S_GPIOH_MUX_B_REG_OFFSET;
        case    M2S_GPIOA_PIN_START     ...M2S_GPIOA_PIN_START + 7:
                return  M2S_GPIOA_MUX_D_REG_OFFSET;
        case    M2S_GPIOA_PIN_START + 8 ...M2S_GPIOA_PIN_END:
                return  M2S_GPIOA_MUX_E_REG_OFFSET;
        case    M2S_GPIOX_PIN_START     ...M2S_GPIOX_PIN_START + 7:
                return  M2S_GPIOX_MUX_3_REG_OFFSET;
        case    M2S_GPIOX_PIN_START + 8 ...M2S_GPIOX_PIN_START + 15:
                return  M2S_GPIOX_MUX_4_REG_OFFSET;
        case    M2S_GPIOX_PIN_START + 16        ...M2S_GPIOX_PIN_END:
                return  M2S_GPIOX_MUX_5_REG_OFFSET;
        case    M2S_GPIOAO_PIN_START    ...M2S_GPIOAO_PIN_START + 7:
                return  M2S_GPIOAO_MUX_REG0_OFFSET;
        case    M2S_GPIOAO_PIN_START + 8        ...M2S_GPIOAO_PIN_START + 11:
                return  M2S_GPIOAO_MUX_REG1_OFFSET;
        default:
                return -1;
        }
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        switch (pin) {
        case    M5_GPIOH_PIN_START      ...M5_GPIOH_PIN_END:
                return  M5_GPIOH_MUX_B_REG_OFFSET;
        case    M5_GPIOA_PIN_START      ...M5_GPIOA_PIN_START + 7:
                return  M5_GPIOA_MUX_D_REG_OFFSET;
        case    M5_GPIOA_PIN_START + 8  ...M5_GPIOA_PIN_END:
                return  M5_GPIOA_MUX_E_REG_OFFSET;
        case    M5_GPIOX_PIN_START      ...M5_GPIOX_PIN_START + 7:
                return  M5_GPIOX_MUX_3_REG_OFFSET;
        case    M5_GPIOX_PIN_START + 8  ...M5_GPIOX_PIN_START + 15:
                return  M5_GPIOX_MUX_4_REG_OFFSET;
        case    M5_GPIOX_PIN_START + 16 ...M5_GPIOX_PIN_END:
                return  M5_GPIOX_MUX_5_REG_OFFSET;
        case    M5_GPIOAO_PIN_START     ...M5_GPIOAO_PIN_START + 7:
                return  M5_GPIOAO_MUX_REG0_OFFSET;
        case    M5_GPIOAO_PIN_START + 8 ...M5_GPIOAO_PIN_START + 11:
                return  M5_GPIOAO_MUX_REG1_OFFSET;
        default:
                return -1;
        }
    }
    else
        wiringPiFailure(WPI_FATAL, "gpioToMuxReg: This code should only be called for Bananapi\n");

    return -1;
}

//
// offset to the GPIO Set regsiter
//
static int gpioToGPSETReg (int pin)
{
    if (piModel == PI_MODEL_BANANAPIM2S) {
        if (pin >= M2S_GPIOH_PIN_START && pin <= M2S_GPIOH_PIN_END)
                return  M2S_GPIOH_OUTP_REG_OFFSET;
        if (pin >= M2S_GPIOA_PIN_START && pin <= M2S_GPIOA_PIN_END)
                return  M2S_GPIOA_OUTP_REG_OFFSET;
        if (pin >= M2S_GPIOX_PIN_START && pin <= M2S_GPIOX_PIN_END)
                return  M2S_GPIOX_OUTP_REG_OFFSET;
        if (pin >= M2S_GPIOAO_PIN_START && pin <= M2S_GPIOAO_PIN_END)
                return  M2S_GPIOAO_OUTP_REG_OFFSET;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        if (pin >= M5_GPIOH_PIN_START && pin <= M5_GPIOH_PIN_END)
                return  M5_GPIOH_OUTP_REG_OFFSET;
        if (pin >= M5_GPIOA_PIN_START && pin <= M5_GPIOA_PIN_END)
                return  M5_GPIOA_OUTP_REG_OFFSET;
        if (pin >= M5_GPIOX_PIN_START && pin <= M5_GPIOX_PIN_END)
                return  M5_GPIOX_OUTP_REG_OFFSET;
        if (pin >= M5_GPIOAO_PIN_START && pin <= M5_GPIOAO_PIN_END)
                return  M5_GPIOAO_OUTP_REG_OFFSET;
    }
    else
	wiringPiFailure(WPI_FATAL, "gpioToGPSETReg: This code should only be called for Bananapi\n");

    return  -1;
}

//
// offset to the GPIO Input regsiter
//
static int  gpioToGPLEVReg (int pin)
{
    if (piModel == PI_MODEL_BANANAPIM2S) {
        if (pin >= M2S_GPIOH_PIN_START && pin <= M2S_GPIOH_PIN_END)
                return  M2S_GPIOH_INP_REG_OFFSET;
        if (pin >= M2S_GPIOA_PIN_START && pin <= M2S_GPIOA_PIN_END)
                return  M2S_GPIOA_INP_REG_OFFSET;
        if (pin >= M2S_GPIOX_PIN_START && pin <= M2S_GPIOX_PIN_END)
                return  M2S_GPIOX_INP_REG_OFFSET;
        if (pin >= M2S_GPIOAO_PIN_START && pin <= M2S_GPIOAO_PIN_END)
                return  M2S_GPIOAO_INP_REG_OFFSET;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        if (pin >= M5_GPIOH_PIN_START && pin <= M5_GPIOH_PIN_END)
                return  M5_GPIOH_INP_REG_OFFSET;
        if (pin >= M5_GPIOA_PIN_START && pin <= M5_GPIOA_PIN_END)
                return  M5_GPIOA_INP_REG_OFFSET;
        if (pin >= M5_GPIOX_PIN_START && pin <= M5_GPIOX_PIN_END)
                return  M5_GPIOX_INP_REG_OFFSET;
        if (pin >= M5_GPIOAO_PIN_START && pin <= M5_GPIOAO_PIN_END)
                return  M5_GPIOAO_INP_REG_OFFSET;
    }
    else
	wiringPiFailure(WPI_FATAL, "gpioToGPLEVReg: This code should only be called for Bananapi\n");

    return  -1;
}

//
// offset to the GPIO Pull up/down enable regsiter
//
static int  gpioToPUENReg (int pin)
{
	if (piModel == PI_MODEL_BANANAPIM2S) {
        if (pin >= M2S_GPIOH_PIN_START && pin <= M2S_GPIOH_PIN_END)
                return  M2S_GPIOH_PUEN_REG_OFFSET;
        if (pin >= M2S_GPIOA_PIN_START && pin <= M2S_GPIOA_PIN_END)
                return  M2S_GPIOA_PUEN_REG_OFFSET;
        if (pin >= M2S_GPIOX_PIN_START && pin <= M2S_GPIOX_PIN_END)
                return  M2S_GPIOX_PUEN_REG_OFFSET;
        if (pin >= M2S_GPIOAO_PIN_START && pin <= M2S_GPIOAO_PIN_END)
                return  M2S_GPIOAO_PUEN_REG_OFFSET;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        if (pin >= M5_GPIOH_PIN_START && pin <= M5_GPIOH_PIN_END)
                return  M5_GPIOH_PUEN_REG_OFFSET;
        if (pin >= M5_GPIOA_PIN_START && pin <= M5_GPIOA_PIN_END)
                return  M5_GPIOA_PUEN_REG_OFFSET;
        if (pin >= M5_GPIOX_PIN_START && pin <= M5_GPIOX_PIN_END)
                return  M5_GPIOX_PUEN_REG_OFFSET;
        if (pin >= M5_GPIOAO_PIN_START && pin <= M5_GPIOAO_PIN_END)
                return  M5_GPIOAO_PUEN_REG_OFFSET;
    }
    else
        wiringPiFailure(WPI_FATAL, "gpioToPUENReg: This code should only be called for Bananapi\n");

    return  -1;
}

//
// offset to the GPIO Pull up/down regsiter
//
static int  gpioToPUPDReg (int pin)
{
    if (piModel == PI_MODEL_BANANAPIM2S) {
        if (pin >= M2S_GPIOH_PIN_START && pin <= M2S_GPIOH_PIN_END)
                return  M2S_GPIOH_PUPD_REG_OFFSET;
        if (pin >= M2S_GPIOA_PIN_START && pin <= M2S_GPIOA_PIN_END)
                return  M2S_GPIOA_PUPD_REG_OFFSET;
        if (pin >= M2S_GPIOX_PIN_START && pin <= M2S_GPIOX_PIN_END)
                return  M2S_GPIOX_PUPD_REG_OFFSET;
        if (pin >= M2S_GPIOAO_PIN_START && pin <= M2S_GPIOAO_PIN_END)
                return  M2S_GPIOAO_PUPD_REG_OFFSET;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        if (pin >= M5_GPIOH_PIN_START && pin <= M5_GPIOH_PIN_END)
                return  M5_GPIOH_PUPD_REG_OFFSET;
        if (pin >= M5_GPIOA_PIN_START && pin <= M5_GPIOA_PIN_END)
                return  M5_GPIOA_PUPD_REG_OFFSET;
        if (pin >= M5_GPIOX_PIN_START && pin <= M5_GPIOX_PIN_END)
                return  M5_GPIOX_PUPD_REG_OFFSET;
        if (pin >= M5_GPIOAO_PIN_START && pin <= M5_GPIOAO_PIN_END)
                return  M5_GPIOAO_PUPD_REG_OFFSET;
    }
    else
        wiringPiFailure(WPI_FATAL, "gpioToPUPDReg: This code should only be called for Bananapi\n");

    return  -1;
}

//
// offset to the GPIO bit
//
static int  gpioToShiftReg (int pin)
{
    if (piModel == PI_MODEL_BANANAPIM2S) {
        if (pin >= M2S_GPIOH_PIN_START && pin <= M2S_GPIOH_PIN_END)
                return  pin - M2S_GPIOH_PIN_START;
        if (pin >= M2S_GPIOA_PIN_START && pin <= M2S_GPIOA_PIN_END)
                return  pin - M2S_GPIOA_PIN_START;
        if (pin >= M2S_GPIOX_PIN_START && pin <= M2S_GPIOX_PIN_END)
                return  pin - M2S_GPIOX_PIN_START;
        if (pin >= M2S_GPIOAO_PIN_START && pin <= M2S_GPIOAO_PIN_END)
                return  pin - M2S_GPIOAO_PIN_START;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        if (pin >= M5_GPIOH_PIN_START && pin <= M5_GPIOH_PIN_END)
                return  pin - M5_GPIOH_PIN_START;
        if (pin >= M5_GPIOA_PIN_START && pin <= M5_GPIOA_PIN_END)
                return  pin - M5_GPIOA_PIN_START;
        if (pin >= M5_GPIOX_PIN_START && pin <= M5_GPIOX_PIN_END)
                return  pin - M5_GPIOX_PIN_START;
        if (pin >= M5_GPIOAO_PIN_START && pin <= M5_GPIOAO_PIN_END)
                return  pin - M5_GPIOAO_PIN_START;
    }
    else
        wiringPiFailure(WPI_FATAL, "gpioToShiftReg: This code should only be called for Bananapi\n");

    return  -1;
}

//
// offset to the GPIO Function register
//
static int  gpioToGPFSELReg (int pin)
{
    if (piModel == PI_MODEL_BANANAPIM2S) {
        if (pin >= M2S_GPIOH_PIN_START && pin <= M2S_GPIOH_PIN_END)
                return  M2S_GPIOH_FSEL_REG_OFFSET;
        if (pin >= M2S_GPIOA_PIN_START && pin <= M2S_GPIOA_PIN_END)
                return  M2S_GPIOA_FSEL_REG_OFFSET;
        if (pin >= M2S_GPIOX_PIN_START && pin <= M2S_GPIOX_PIN_END)
                return  M2S_GPIOX_FSEL_REG_OFFSET;
        if (pin >= M2S_GPIOAO_PIN_START && pin <= M2S_GPIOAO_PIN_END)
                return  M2S_GPIOAO_FSEL_REG_OFFSET;
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        if (pin >= M5_GPIOH_PIN_START && pin <= M5_GPIOH_PIN_END)
                return  M5_GPIOH_FSEL_REG_OFFSET;
        if (pin >= M5_GPIOA_PIN_START && pin <= M5_GPIOA_PIN_END)
                return  M5_GPIOA_FSEL_REG_OFFSET;
        if (pin >= M5_GPIOX_PIN_START && pin <= M5_GPIOX_PIN_END)
                return  M5_GPIOX_FSEL_REG_OFFSET;
        if (pin >= M5_GPIOAO_PIN_START && pin <= M5_GPIOAO_PIN_END)
                return  M5_GPIOAO_FSEL_REG_OFFSET;
    }
    else
        wiringPiFailure(WPI_FATAL, "gpioToGPFSELReg: This code should only be called for Bananapi\n");

    return  -1;
}

//***************************************************************************************************

/*
* Note: Unlike the above code, this is not copied directly from wiringPi
* Much of the code is identical, but un-necessary parts are deleted
*/

int wiringPiSetupAml (void)
{
    int fd;

    // Open the master /dev/memory device
    if (access("/dev/gpiomem", 0) == 0)
    {
        if ((fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            return wiringPiFailure(WPI_ALMOST, "wiringPiSetupAml: Unable to open /dev/gpiomem: %s\n", strerror(errno));
    }
    else
    {
        if (geteuid() != 0)
            (void)wiringPiFailure(WPI_FATAL, "wiringPiSetupAml: Must be root. (Did you forget sudo?)\n");

        if ((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            return wiringPiFailure(WPI_ALMOST, "wiringPiSetupAml: Unable to open /dev/mem: %s\n", strerror(errno));
    }

    //  piBoardId (&model, &rev, &mem, &maker, &overVolted) ;

    //wiringPi pin numbers are unused in rPI.GPIO
//    pinToGpio = NULL;
//    pin_array_count = 0;
    //physToGPIO replaced by pin_to_gpio in rPI.GPIO
//    physToGpio = NULL;

    if (piModel == PI_MODEL_BANANAPIM2S)
    {

        // GPIO:
        gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, M2S_GPIO_BASE);
        if ((int32_t)gpio == -1)
            return wiringPiFailure(WPI_ALMOST, "wiringPiSetupAml: mmap (GPIO) failed: %s\n", strerror(errno));
        gpioao = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, M2S_GPIO_AO_BASE);
        if ((int32_t)gpioao == -1)
            return wiringPiFailure(WPI_ALMOST, "wiringPiSetupAml: mmap (GPIO_AO) failed: %s\n", strerror(errno));
    }
    else if (piModel == PI_MODEL_BANANAPIM5) {
        // GPIO:
        gpio = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, M5_GPIO_BASE);
        if ((int32_t)gpio == -1)
            return wiringPiFailure(WPI_ALMOST, "wiringPiSetupAml: mmap (GPIO) failed: %s\n", strerror(errno));
        gpioao = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, M5_GPIO_AO_BASE);
        if ((int32_t)gpioao == -1)
            return wiringPiFailure(WPI_ALMOST, "wiringPiSetupAml: mmap (GPIO_AO) failed: %s\n", strerror(errno));
    }

    return 0;
}

void wiringPiCleanupAml (void)
{
    munmap((void *)gpio, BLOCK_SIZE);
    if (gpioao != NULL) munmap((void *)gpioao, BLOCK_SIZE);
}

/*
 * pinModeAml:
 *	Sets the mode of a pin to be input, output or PWM output
 *********************************************************************************
 */

void pinModeAml (int pin, int mode)
{
    int fsel, mux, target, shift;

    //Aml: For our purposes pin comes in as gpio, original code converted
    //pin to gpio and kept origPin as pin#
	
    fsel = gpioToGPFSELReg(pin);
    mux = gpioToMuxReg(pin);
    shift = gpioToShiftReg (pin);
    target = shift * 4;

    if (mode == INPUT)
    {
        if (piModel == PI_MODEL_BANANAPIM2S ||
            piModel == PI_MODEL_BANANAPIM5) {
            *((isGpioAOPin(pin) ? gpioao : gpio) + mux)  = *((isGpioAOPin(pin) ? gpioao : gpio) + mux) & ~(0xF << target);
            *((isGpioAOPin(pin) ? gpioao : gpio) + fsel) = *((isGpioAOPin(pin) ? gpioao : gpio) + fsel) | (1 << shift);
	}
	else
            wiringPiFailure(WPI_FATAL, "pinModeAml: This code should only be called for Bananapi\n");
    }
    else if (mode == OUTPUT)
    {
        if (piModel == PI_MODEL_BANANAPIM2S ||
            piModel == PI_MODEL_BANANAPIM5) {
            *((isGpioAOPin(pin) ? gpioao : gpio) + mux)  = *((isGpioAOPin(pin) ? gpioao : gpio) + mux) & ~(0xF << target);
            *((isGpioAOPin(pin) ? gpioao : gpio) + fsel) = *((isGpioAOPin(pin) ? gpioao : gpio) + fsel) & ~(1 << shift);
	}
	else
            wiringPiFailure(WPI_FATAL, "pinModeAml: This code should only be called for Bananapi\n");
    }
}


/*
 * pullUpDnControlAml:
 *	Control the internal pull-up/down resistors on a GPIO pin
 *	The Arduino only has pull-ups and these are enabled by writing 1
 *	to a port when in input mode - this paradigm doesn't quite apply
 *	here though.
 *********************************************************************************
 */

void pullUpDnControlAml (int pin, int pud)
{
    int puen, pupd, shift;

    shift = gpioToShiftReg(pin);
    puen = gpioToPUENReg(pin);
    pupd = gpioToPUPDReg(pin);

    if (piModel == PI_MODEL_BANANAPIM2S ||
        piModel == PI_MODEL_BANANAPIM5)
    {

        if (pud)
        {
            // Enable Pull/Pull-down resister
            *((isGpioAOPin(pin) ? gpioao : gpio) + puen) = (*((isGpioAOPin(pin) ? gpioao : gpio) + puen) | (1 << shift));

            if (pud == PUD_UP)
                *((isGpioAOPin(pin) ? gpioao : gpio) + pupd) = (*((isGpioAOPin(pin) ? gpioao : gpio) + pupd) | (1 << shift));
            else
                *((isGpioAOPin(pin) ? gpioao : gpio) + pupd) = (*((isGpioAOPin(pin) ? gpioao : gpio) + pupd) & ~(1 << shift));
        }
        else // Disable Pull/Pull-down resister
            *((isGpioAOPin(pin) ? gpioao : gpio) + puen) = (*((isGpioAOPin(pin) ? gpioao : gpio) + puen) & ~(1 << shift));
    }
    else
        wiringPiFailure(WPI_FATAL, "pullUpDnControlAml: This code should only be called for Bananapi\n");
}

/*
 * digitalReadAml:
 *	Read the value of a given Pin, returning HIGH or LOW
 *********************************************************************************
 */

int digitalReadAml (int pin)
{

    if (piModel == PI_MODEL_BANANAPIM2S ||
        piModel == PI_MODEL_BANANAPIM5)
    {
        if ((*((isGpioAOPin(pin) ? gpioao : gpio) + gpioToGPLEVReg(pin)) & (1 << gpioToShiftReg(pin))) != 0)
            return HIGH;
        else
            return LOW;
    }
    else
        wiringPiFailure(WPI_FATAL, "digitalReadAml: This code should only be called for Bananapi\n");

    return 0;
}

/*
 * digitalWriteAml:
 *	Set an output bit
 *********************************************************************************
 */

void digitalWriteAml (int pin, int value)
{

    if (piModel == PI_MODEL_BANANAPIM2S ||
        piModel == PI_MODEL_BANANAPIM5)
    {
        if (value == LOW)
            *((isGpioAOPin(pin) ? gpioao : gpio) + gpioToGPSETReg(pin)) &= ~(1 << gpioToShiftReg(pin));
        else
            *((isGpioAOPin(pin) ? gpioao : gpio) + gpioToGPSETReg(pin)) |= (1 << gpioToShiftReg(pin));
    }
    else
        wiringPiFailure(WPI_FATAL, "digitalWriteAml: This code should only be called for Bananapi\n");
}

/*
 * analogReadAml:
 *	Read the analog value of a given Pin.
 *	There is no on-board Pi analog hardware,
 *	so this needs to go to a new node.
 *********************************************************************************
 */

int analogReadAml (int pin)
{
    wiringPiFailure(WPI_FATAL, "analogReadAml: No ADC pin on Bananapi\n");
}

/*
 * analogWriteAml:
 *	Write the analog value to the given Pin.
 *	There is no on-board Pi analog hardware,
 *	so this needs to go to a new node.
 *********************************************************************************
 */

void analogWriteAml (int pin, int value)
{
    wiringPiFailure(WPI_FATAL, "analogWriteAml: No DAC pin on Bananapi\n");
}

/*
 * pinGetModeAml:
 *	Gets the mode of a pin to be input, output
 * Added by JF
 *********************************************************************************
 */

int pinGetModeAml (int pin)
{
    int shift;
    int rwbit, regval, retval=0;
    //aml: pin comes in as gpio

    if (piModel == PI_MODEL_BANANAPIM2S ||
        piModel == PI_MODEL_BANANAPIM5)
    {
        regval = (*((isGpioAOPin(pin) ? gpioao : gpio) + gpioToGPFSELReg(pin)));
        rwbit = regval & (1 << gpioToShiftReg(pin));
        retval = ((rwbit!=0) ? 0 : 1);
    }
    else
        wiringPiFailure(WPI_FATAL, "pinGetModeAml: This code should only be called for Bananapi\n");

    return retval;
}

void setInfoAml(char *hardware, void *vinfo)
{
   rpi_info *info = (rpi_info *)vinfo;

   if (strcmp(hardware, "BPI-M2S") == 0)
    {
        piModel = PI_MODEL_BANANAPIM2S;
        info->type = "BPI-M2S";
        info->p1_revision = 3;
        info->ram = "2048M/4096M";
        info->manufacturer = "Bananapi";
        info->processor = "AMLS922X/AMLA311D";
    }
    else if ((strcmp(hardware, "BPI-M5") == 0) || (strcmp(hardware, "BPI-M2-Pro") == 0))
    {
        piModel = PI_MODEL_BANANAPIM5;
        info->type = "BPI-M5";
        info->p1_revision = 3;
        info->ram = "2048M/4096M";
        info->manufacturer = "Bananapi";
        info->processor = "AMLS905X3";
    }
    else
        wiringPiFailure(WPI_FATAL, "setInfoAml: This code should only be called for Bananapi\n");
   
    return;
}

void setMappingPtrsAml(void)
{
    if (piModel == PI_MODEL_BANANAPIM2S)
    {
        pin_to_gpio = (const int(*)[41]) & physToGpioBananapiM2S;
        bcm_to_amlgpio = &bcmToOGpioBananapiM2S;
    }
    else if (piModel == PI_MODEL_BANANAPIM5)
    {
        pin_to_gpio = (const int(*)[41]) & physToGpioBananapiM5;
        bcm_to_amlgpio = &bcmToOGpioBananapiM5;
    }
}
