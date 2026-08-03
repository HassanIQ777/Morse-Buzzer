#include <ctype.h>
#include <string.h>

class Buzzer {
  int pin_number;

public:
  explicit Buzzer(int pin)
    : pin_number(pin) {
    pinMode(pin, OUTPUT);
  }

  void start(int hz, int duration) {
    tone(pin_number, hz, duration);
  }

  void stop() {
    noTone(pin_number);
  }
};

Buzzer buz(8);

const int UNIT = 120;
const int TONE_HZ = 800;

const char* morseCode[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

void playSymbol(char sym) {
  int dur = (sym == '.') ? UNIT : UNIT * 3;
  buz.start(TONE_HZ, dur);
  delay(dur);
  buz.stop();
}

void playMorseChar(char c) {
  if (c == ' ') {
    delay(UNIT * 7);
    return;
  }

  int idx;
  if (isalpha((unsigned char)c)) {
    idx = toupper((unsigned char)c) - 'A';
  } else if (isdigit((unsigned char)c)) {
    idx = c - '0' + 26;
  } else {
    return;  // punctuation etc. — silently skip
  }

  const char* code = morseCode[idx];
  int len = strlen(code);
  for (int i = 0; i < len; i++) {
    playSymbol(code[i]);
    delay(UNIT);
  }
  delay(UNIT * 2);
}

// --- new: whole-word / whole-line handling ---

const int BUF_SIZE = 64;  // fixed buffer, no String heap churn
char lineBuf[BUF_SIZE];
uint8_t lineLen = 0;

void playMorseWord(const char* text) {
  for (int i = 0; text[i] != '\0'; i++) {
    playMorseChar(text[i]);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Type a word or sentence, hit Enter.");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      lineBuf[lineLen] = '\0';  // terminate string
      if (lineLen > 0) {
        Serial.print("Playing: ");
        Serial.println(lineBuf);
        playMorseWord(lineBuf);
      }
      lineLen = 0;  // reset buffer for next line
    } else if (c != '\r' && lineLen < BUF_SIZE - 1) {
      lineBuf[lineLen++] = c;
    }
    // if buffer's full, extra chars just get silently dropped —
    // your 64-char sentence limit, take it or leave it
  }
}