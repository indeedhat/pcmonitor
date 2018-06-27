#include <Adafruit_SSD1306.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>



int cpu;
int[100] cores;
int coreCount;
byte memory;
int group;

void setup() {
  // init serial connection
  Serial.begin(9600);
  while(!Serial) {
    delay(10);
  }
}

byte[] readFromSerial() {
  byte[] stream;
  int i = 0;
  do {
    delay(10);
    if (0 < Serial.available()) {
      char byt = Serial.read();
      stream[i++] = (byte)byt;

      if (';' == byt) {
        break;
      }
    }
  } while (Serial.available());

  return stream;
}

void reset() {
  cup       = 0;
  cores     = int[100]{};
  coreCount = 0;
  memory    = 0;
  group     = 0;
}
 
void parse(byte[] steam) {
  reset();
  
  for (int i = 0; i < 100; i++) {
    if (';' == (char)stream[i]) break;
    if (',' == (char)stream[i]) {
      group++;
      continue;
    }

    switch (group) {
      case 0: // cpu
        cpu = (int)stream[i];
        break;
      case 1: // cores
        cores[coreCount++] = (int)stream[i];
        break;
      case 2: // memory
        memory = (int)stream[i];
        break
      default:
        return;
    }
  }
}

void render() {
  
}

void loop() {
  parse(
    readFromSerial()
  );
  render();
}
