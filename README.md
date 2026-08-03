# morse-buzzer

Type a character, a word, or a whole sentence into Serial Monitor and an Arduino-driven buzzer plays it back in Morse code, with proper dot/dash/gap timing.

## How it works

- A `Buzzer` class wraps `tone()`/`noTone()` behind a tiny interface.
- A lookup table (`morseCode[36]`) maps `A-Z` and `0-9` to their dot/dash patterns.
- Timing follows the standard Morse ratio: dot = 1 unit, dash = 3 units, gap between symbols = 1 unit, gap between letters = 3 units, gap between words = 7 units. Change one constant (`UNIT`) to speed everything up or down.
- Serial input is read into a fixed-size char buffer (no `String`, no heap churn) until `\n`, then played back symbol by symbol.

## Wiring

| Arduino | Buzzer |
|---|---|
| Digital pin 8 | `+` lead (through an optional 100Ω resistor) |
| GND | `-` lead |

Piezo buzzer: resistor is optional, cheap insurance against current spikes. Magnetic buzzer: resistor matters more, or drive it through a transistor if it pulls more current than a digital pin should supply.

## Usage

1. Wire the buzzer to pin 8 and GND as above.
2. Flash `src/morse_buzzer.ino`.
3. Open Serial Monitor at 9600 baud, set line ending to Newline.
4. Type a word or sentence, hit Enter. Listen.

```
> SOS
Playing: SOS
... --- ...

> HELLO WORLD
Playing: HELLO WORLD
.... . .-.. .-.. ---   .-- --- .-. .-.. -..
```

## Notes / known limitations

- Playback is **blocking** — while a line plays, the board won't respond to anything else (buttons, sensors) until it finishes. Fine for a standalone toy, not for running alongside other real-time logic. A non-blocking version would need a `millis()`-driven state machine instead of `delay()`.
- Punctuation and unsupported characters are silently skipped rather than erroring.
- Input buffer caps at 64 characters per line; anything beyond that is dropped, not queued.

## Possible extensions

- Non-blocking playback via `millis()` state machine
- Reverse mode: button taps timed and decoded back into text
- Bit-packed Morse table (each pattern fits in a single byte via a sentinel-bit trick) if you're tight on RAM

## License

MIT — do whatever you want with it.
