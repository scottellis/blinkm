  Overview
--------

A command line program for controlling BlinkM leds connected to the I2C bus 
of a embedded Linux machine. 

The program uses the built-in i2c-dev Linux driver to talk to the BlinkMs. 
The i2c-dev driver exposes i2c busses to user programs as /dev/i2c-* entries. 

To avoid having to specify the i2c bus on every run of the program, the bus 
number is hard coded in i2c_functions.c

The default configuration assumes a Gumstix Overo board using /dev/i2c-3.

If you are using an RPi or another embedded Linux system, then edit 
i2c_functions.c before building. 

The required change should be self-explanatory.


  Building
--------

There is a make file for building the project.

        $ git pull git://github.com/scottellis/overo-blinkm.git
        $ cd overo-blinkm
        $ <optional> edit i2c_functions.c
        $ make


  Running
--------

RPi kernels might not load the i2c drivers automatically. If you are
running the Debian Wheezy image, you can add the following two lines
to /etc/modules

        i2c_dev
        i2c_bcm2708

Gumstix kernels load the i2c drivers by default.

You will need to change permissions for /dev/i2c-* device if you are
running blinkm as someone other then root.

A one-time fix is to run the following as root (Gumstix example)

        chmod 0666 /dev/i2c-3

This will get overwritten on your next boot though.

A more permanent fix is to create a udev rule file like this 

        /etc/udev/rules.d/90-i2c.rules

With the following single line

        KERNEL=="i2c-[0-3]", MODE="0666"


Run the blinkm program without arguments to get the available commands.

        $ ./blinkm

        Usage: blinkm <command> <args>

        The led address is optional and defaults to 0x09.
        Use a comma separated list to address multiple devices in one command.
        The color arguments are optional and default to zero.

        Available Commands
                find-leds 
                set-rgb [-d led] [-r red] [-g green] [-b blue]
                get-rgb [-d led]
                fade-rgb [-d led] [-r red] [-g green] [-b blue]
                fade-hsb [-d led] [-h hue] [-s saturation] [-b brightness]
                fade-random-rgb [-d led] [-r red] [-g green] [-b blue]
                fade-random-hsb [-d led] [-h hue] [-s saturation] [-b brightness]
                play-script [-d led] -s <script id or name> -n num_repeats
                stop-script [-d led]
                set-fade-speed [-d led] -f speed
                set-time-adjust [-d led] -t adjust
                show-scripts 
                read-script [-d led]
                write-script-line [-d led] -n line_no -t ticks -c cmd -a arg1[,arg2[,arg3]]
                set-script-length-and-repeats [-d led] -l length -n repeats
                set-address -d new_led_address


The first command you probably want to run is find-leds.

        $ ./blinkm find-leds

        Scanning I2C bus for BlinkM devices...
        Found a BlinkM at address 1 (0x01)
        Found a BlinkM at address 2 (0x02)
        Found a BlinkM at address 3 (0x03)
        Found a BlinkM at address 4 (0x04)
        Found 4 devices



  TODO
--------

1. Write script function hasn't been tested much.

2. Need to recover the bus after the command to change an led address.
   Right now you need to power cycle the led which is okay, but you
   also need to power cycle the overo or the i2c bus stays hung.
   
3. GetAddress is not implemented. Investigate if i2c broadcasts are
   possible with the overos. 

4. Add code to read in scripts from a file.
