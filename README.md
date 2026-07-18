# chip8
My personal implementation of the CHIP-8 specification. Other than ncurses, it requires no external dependencies to run.
The emulator works well with all ROMs I've found online, and passes Timendus' test suite for base CHIP-8.
THIS EMULATOR DOES NOT IMPLEMENT SUPER CHIP-48

# How to build
- Clone the repository using
 $ git clone https://github.com/Bouzenad/chip8
- Make sure you have ncurses installed. If you don't install it using your distro's package manager.
- Build the binary using
 $ make
- Enter the new 'builds' directory, and behold, there is your binary!
- (Optional) You can remove the build directory along with whatever is in it with
 $ make clean

# How to use
- Use the (1, 2, 3, 4, q, w, e, r, a, s, d, f, z, x, c, v) letters on your keyboard for input. They're analogous to the keypads used in the original CHIP-8 implementations (check Cowgod's reference).
- On the CLI, add the path to the ROM you want to play.
- I've added arguments for toggling different quirks, for compatbility purposes (they're all OFF, by default):
    * -vf-reset: OR, AND, XOR instructions all set the flag register to 0.
    * -memory-increment: Store and load instructions increment the address register I to I + X + 1
    * -legacy-shift: Vx is set to Vy shifted, instead of Vx being shifted in-place
    * -x-jump: In the Jp V0 addr instruction, jump to XNN + Vx instead of NNN + V0
- If it suits you, you may also tweak the frequency of the buzzer using the argument:
    * -sound-freq=(mycoolfrequency)
    * Example: -sound-freq=500
    * Sound frequency is 440 by default, a clean A note.


# Credits

- Cowgod's chip-8 technical reference
- Animal Race rom by Brian Astle (for testing purposes)
- Space Invaders by David Winter (for testing purposes)
- Miniaudio by David Reid
- Make

# Challenges and design decisions

- The CHIP-8 specification requires that the sound and delay timers each decrement at 60Hz. the CPU frequency, however, has no canonical value. A common value I see around is 500Hz, however I decided to settle on 480Hz, on average. The reason is that it's a nice multiple of 60Hz, so 8 instructions are executed in each timer cycle. I could've chosen 500Hz, but I would've had to make the instruction count in each clock cycle vary between 8, 9, or 10, and the code more complex by calculating remainder time at each cycle, accumulate it, blah blah blah... I decided it isn't worth it, since the clock frequency varies between implementations from 300Hz all the way to 1000Hz anyway, so the difference between 500 and 480 is barely noticeable.

- For the display, I had multiple options, between SDL2, OpenGL, etc... I decided to keep the display on the terminal, for the retro aesthetics. I was originally going to spin up my own rendering library, but between handling user inputs, frame buffering, etc... I decided it was too much effort for what little worth it has, and instead picked the ncurses library. I would like to keep dependencies to a minimum, but ncurses is just way too nice, and besides, I was curious about it and wanted to learn how to use it.

- Input handling was probably the most challenging part of this project. Common implementations of CHIP-8 require being able to detect positive edges, but given that everything runs on the terminal, that's difficult to do, given that input is monitored every ~16ms, and ncurses isn't supposed to be used for these sorts of applications, that was a fault on my part. There were some alternatives, like using other libraries like SDL2, using raw evdev, etc... But I didn't like any of them, since they're require me to dual-run multiple environments (for example, if I'd used SDL2, I'd need to run a second, invisible window solely for detecting input), or hurt portability in some way.
