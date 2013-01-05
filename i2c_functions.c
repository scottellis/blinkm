/*
  Copyright (c) 2009-2013, Scott Ellis
  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the <organization> nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY Scott Ellis ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Scott Ellis BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> 

// Uncomment one of these definitions or create a new one
//#define GUMSTIX_OVERO
#define RASPBERRYPI_v1
//#define RASPBERRYPI_v2


#if defined GUMSTIX_OVERO
static char i2c_bus[] = "/dev/i2c-3";
#elif defined RASPBERRYPI_v1
static char i2c_bus[] = "/dev/i2c-0";
#elif defined RASPBERRYPI_v2
static char i2c_bus[] = "/dev/i2c-1";
#else
#error "I2C bus definition needed!"
#endif


static int i2c_open_device()
{
	int fh = -1;

	fh = open(i2c_bus, O_RDWR);

	if (fh < 0) {
		fprintf(stderr, "Failed to open file %s: %s\n", 
				i2c_bus, strerror(errno));
	}

	return fh;
}

static int i2c_set_slave_address(int fh, uint8_t address)
{
	if (fh < 0) 
		return -1;

	if (ioctl(fh, I2C_SLAVE, address) < 0) {
		fprintf(stderr, "ioctl set slave failed: %s", strerror(errno));
		return -1;
	}

	return 1;
}

int i2c_start_transaction(uint8_t slave_address)
{
	int fh, result;

	fh = i2c_open_device();

	if (fh < 0) 
		return -1;

	result = i2c_set_slave_address(fh, slave_address);	

	if (result < 0) {
		close(fh);
		return result;
	}

	return fh;
}

int i2c_end_transaction(int fh)
{
	if (fh > 0) 
		close(fh);

	return 1;
}

