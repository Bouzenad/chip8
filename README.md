# chip8
My personal implementation of the CHIP-8 specification

# Credits

- Cowgod's chip-8 technical reference
- Animal Race rom by Brian Astle (for testing purposes)
- Space Invaders by David Winter (for testing purposes)
- Miniaudio by David Reid

# Challenges and design decisions

- The CHIP-8 specification requires that the sound and delay timers each decrement at 60Hz. the CPU frequency, however, has no canonical value. A common value I see around is 500Hz, however I decided to settle on 480Hz, on average. The reason is that it's a nice multiple of 60Hz, so 8 instructions are executed in each timer cycle. I could've chosen 500Hz, but I would've had to make the instruction count in each clock cycle vary between 8, 9, or 10, and the code more complex by calculating remainder time at each cycle, accumulate it, blah blah blah... I decided it isn't worth it, since the clock frequency varies between implementations from 300Hz all the way to 1000Hz anyway, so the difference between 500 and 480 is barely noticeable.

- For the display, I had multiple options, between SDL2, OpenGL, etc... I decided to keep the display on the terminal, for the retro aesthetics. I was originally going to spin up my own rendering library, but between handling user inputs, frame buffering, etc... I decided it was too much effort for what little worth it has, and instead picked the ncurses library. I would like to keep dependencies to a minimum, but ncurses is just way too nice, and besides, I was curious about it and wanted to learn how to use it.

- Handling user inputs required a bit of challenge. I originally tried to process inputs at the end of each timer cycle using ncurses, but decided there's too much latency, and instead chose to process keyboard inputs on a separate thread. The code was a bit more complicated, but it had the benefit of better user experience. Thankfully, I didn't suffer too much from concurrency bugs, since the design itself was fairly simple (one thread produces data, the other consumes it).
