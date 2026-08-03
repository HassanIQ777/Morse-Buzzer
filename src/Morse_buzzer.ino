#include <ctype.h>
#include <string.h>

class Buzzer {
  int pin_number;

public:
  explicit Buzzer(int pin) : pin_number(pin) { pinMode(pin, OUTPUT); }

  void start(int hz, int duration) { tone(pin_number, hz, duration); }

  void stop() { noTone(pin_number); }
};

Buzzer buz(8);

const int UNIT = 120;    // pause duration
const int TONE_HZ = 800; // frequency of the buzzer

const char *morseCode[36] = {
    ".-",    "-...",  "-.-.",  "-..",   ".",     "..-.",  "--.",   "....",
    "..",    ".---",  "-.-",   ".-..",  "--",    "-.",    "---",   ".--.",
    "--.-",  ".-.",   "...",   "-",     "..-",   "...-",  ".--",   "-..-",
    "-.--",  "--..",  "-----", ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----."};

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
    return; // skip punctuation
  }

  const char *code = morseCode[idx];
  int len = strlen(code);
  for (int i = 0; i < len; i++) {
    playSymbol(code[i]);
    delay(UNIT);
  }
  delay(UNIT * 2);
}

const int BUF_SIZE = 64;
char lineBuf[BUF_SIZE];
uint8_t lineLen = 0;

void playMorseWord(const char *text) {
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
      lineBuf[lineLen] = '\0';
      if (lineLen > 0) {
        Serial.print("Playing: ");
        Serial.println(lineBuf);
        playMorseWord(lineBuf);
      }
      lineLen = 0;
    } else if (c != '\r' && lineLen < BUF_SIZE - 1) {
      lineBuf[lineLen++] = c;
    }
  }
}