# image2brick

> An open source implementation of the image2brick program.

This is my open source implementation of the image2brick program, written in C. It can be used normally like the classic image2brick program, but it can also be used as a command-line interface with the `-X` option.

I decided to write it after I realized that such a classic program like image2brick wasn't open source (it was a different time). I wrote it in C after learning that image2brick was written using the _[Irrlicht Engine](https://en.wikipedia.org/wiki/Irrlicht_Engine)_ for some ungodly reason. Why Jookia needed an entire game engine to write a simple console program I'll never know...

## Usage

There are two ways to use this program: either as a typical console program, or as a command-line interface.

To use it normally, just drag an image file onto the program and select your orientation. Make sure there's a `colorSet.txt` file in the same directory as the program.

***

You can also use it as a command-line program: `image2brick (-v | -h) [-c colorset_file] [-X] image_file`

`-v` and `-h` options are the orientation that the macro will be in. The program requires you to specify at least one.

`-c` specifies the colorset file to use. If not specified, image2brick will look for `colorSet.txt` in the current working directory.

`-X` makes the program behave as a command-line interface. Since the vast majority of users of this program will use Windows, it operates as a normal program by default, pausing after messages and making the user select an orientation if one wasn't specified in the arguments. This option disables that and will simply display error/success messages and then immediately exit.

## Building

For Windows, simply open the solution in the `vs2022/` folder in **Visual Studio 2022**. Then choose "Release" and build the project!

For Linux, you get a single GCC build script. Have fun.

I didn't want to figure out CMake.
