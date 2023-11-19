# image2brick

> An open source implementation of the Image2Brick program.

This is my open source implementation of the Image2Brick program, written in C. It can be used normally like the classic Image2Brick program, but it can also be used as a command-line interface with the `-X` option.

I decided to write it after I realized that such a classic program like Image2Brick wasn't open source (it was a different time). I wrote it in C after learning that Image2Brick was written using the _[Irrlicht Engine](https://en.wikipedia.org/wiki/Irrlicht_Engine)_ for some ungodly reason. Why Jookia needed an entire game engine to write a simple console program I'll never know...

## Usage

`image2brick (-v | -h) [-X] image_file`

`-v` and `-h` options are the orientation that the macro will be in. The program requires you to specify at least one.

`-X` makes the program behave as a command-line interface. Since the vast majority of users of this program will use Windows, it operates as a normal program by default, pausing after messages and making the user select an orientation if one wasn't specified in the arguments. This option disables that and will simply display error/success messages and then immediately exit.
