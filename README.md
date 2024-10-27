# sand_napalm

**N**ot **A** **P**haser **AL**though **M**aybe  (**NAPALM**)  VST3 Plugin

![preview](https://i.imgur.com/KDBIDgO.png)

# What does it do?

Creates n equally spaced copies of input, up to the specified range, <br>
which then can be shifted by an amount up to the range.

So it basically works like a phaser, but it doesn't use all-pass filters, <br> 
and instead works by shifting copies of an input (kinda like a combs filter or flanger).

In short: Square4 but as an audio effect

# Parameters

| Parameter    | Description |
| ------------ | ------------ |
| Invert Phase | Inverts phase of every even copy |
| MIDI         | Uses midi input instead of given range <br> Works by converting one cycle of a note frequency to it's size in samples, which is then used as range <br> Useful for consistency between pitches |
| Amount       | Shift copies by an amount |
| Range        | Specifies maximum range to which copies can be shifted to (in samples) <br> Works by multiplying amount <br> Ignored if midi input is on |
| Pitch        | Shifts copies by semitones <br> Move the number to the right to change the pitch range <br> Ignored if midi input is off |
| Copies       | How many copies to create |

## How to build from source

### Prerequesities

#### Required

- JUCE
- Your system C++ build toolchain (Visual Studio on Windows, XCode on Mac, GCC/Clang on Linux, etc.)

#### Optional

- CMake (**NOTE**: CMake script is only tested on Linux at the moment. Please contribute)
- Ninja (useful on Linux)

### Building (the Projucer way)

- Open Projucer
- Open `Napalm.jucer` in Projucer
- Add your system build configuration if neccessary (there is only Windows target at the moment), then save the `.jucer` file
- Build the plugin using the generated project in the `Build` folder. Which means:
  + Run Visual Studio/Xcode and open the appropriate `.sln`/`xcodeproject`, then compile (Win/Mac)
  + `cd Builds/LinuxMakefile && make` (Linux)

### Building (the CMake way)

Assuming you are in the root folder, then run these 2 commands in the terminal/command prompt:

```
cmake -Bbuild
cmake --build build --config Release
```

Optionally, on Linux, use Ninja to speed up the build by replacing the first command with `cmake -Bbuild -GNinja`.

# Credits
[Beats Basteln](https://www.youtube.com/@Beatsbasteln)

[JUCE](https://www.juce.com)

big thanks to [dottokuya](https://github.com/dottokuya) for the build instructions 

# Support
If you like it, consider donating thru [gumroad!](https://s4n7r0.gumroad.com/l/napalm) <br>
[Soundcloud](https://www.soundcloud.com/s4n7r0) <br>
[Twitter](https://www.twitter.com/s4n7r0) <br>
[Bluesky](https://bsky.app/profile/sandr0.bsky.social) <br>
[Bandcamp](https://s4n7r0.bandcamp.com/track/dragonfly) <br>
