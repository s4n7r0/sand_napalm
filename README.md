# sand_napalm
**N**ot **A** **P**haser **AL**though **M**aybe

aka **NAPALM**

![preview](https://i.imgur.com/wHq8iV9.png)

# What does it do?

Creates N equally spaces copies of input, up to the specified range, 
which then can be shifted by an amount up to the range.

So it basically works like a phaser, but it doesn't use an all-pass phaser, 
and instead works by shifting copies of input.

# Parameters

| Syntax | Description |
| ----------- | ----------- |
| Invert Phase | Inverts phase of every even copy |
| MIDI | Uses midi input instead of given range <br> Works by converting one cycle of a note frequency to it's size in samples, which is then used as range <br> Useful for consistency between pitches |
| Amount | Shift copies by an amount |
| Range | Specifies maximum range to which copies can be shifted to (in samples) <br> Works by multiplying amount <br> Ignored if midi input is on |
| Copies | How many copies to create |

# Credits
[Beats Basteln](https://www.youtube.com/@Beatsbasteln)

[JUCE](https://www.juce.com)

# Support
[Soundcloud](https://www.soundcloud.com/s4n7r0)
[Twitter](https://www.twitter.com/s4n7r0)
[Bluesky](https://www.bluesky.com/sandr0.bsky.social)
[Bandcamp](https://www.bandcamp.com/s4n7r0)
