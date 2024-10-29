# sand_napalm

**N**ot **A** **P**haser **AL**though **M**aybe  (**NAPALM**) Audio Plugin

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

## Packages for linux

### Building (the Projucer way)

- ```git clone https://github.com/s4n7r0/sand_napalm.git```
- Open Projucer
- Open `Napalm.jucer` in Projucer
- Select your system build configuration, save the project
- Build the plugin using the generated project in the `Build` folder. Which means:
  + Run Visual Studio/Xcode and open the appropriate project, either the generated `.sln`/`xcodeproject` or thru Projucer (Win/Mac)
  # MAC
  + If wanna build the plugin in AU format, you need to add AU plugin format in Project Settings's in Projucer
  + Product > Scheme > Edit Scheme... > Run > Build Configuration > Select "Release"
  + Build
  + Built plugin in any format should be copied to "~/Library/Audio/Plug-Ins/" to their respective folders, but if not:
  + Copy the compiled sand_napalm.vst3 from "Builds/MaxOSX/build/Release/sand_napalm.vst3" to "~/Library/Audio/Plug-Ins/VST3"
  + Copy the compiled sand_napalm.component from "Builds/MaxOSX/build/Release/sand_napalm.component" to "~/Library/Audio/Plug-Ins/Components" ( if building as an AU )
  # WINDOWS
  + Select "Release" configuration (Win)
  + Build
  + Copy the compiled sand_napalm.vst3 from "Builds\VisualStudio2022\x64\Release\VST3\sand_napalm.vst3\Contents\x86_64-win\" to "C:\Program Files\Common Files\VST3\" (Win)
  # LINUX
  + `cd Builds/LinuxMakefile && make CONFIG=Release`
  + Built VST3 should be copied to "~/.vst3/" but if not:
  + `mkdir ~/.vst3`
  + `mv "Builds/LinuxMakefile/build/sand_napalm.vst3/Contents/x86_64-linux/sand_napalm.so" "~/.vst3/sand_napalm.vst3"`

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
