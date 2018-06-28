#include <Adafruit_SSD1306.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET)


const int ROWS = 64;
const int COLS = 128;


enum screens {
  CPU,
  CORES,
  MEMORY
}

screens activeScreen = CPU;
int cpu;
int[100] cores;
int coreCount;
byte memory;
int group;


void setup() {
  // init display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C)
  
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
  display.clearDisplay();
  
  switch (activeScreen) {
    case CPU:
      renderCpu();
      break;
    case CORES:
      renderCores();
      break;
    case MEMORY:
      renderMemory();
      break;
    default:
      renderSplash();
  }
}

void renderCpu() {
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(padAsPercentage(cpu));
}


void renderCores() {
  int width = COLS / coreCount;
  int curs = 0;

  for (int i = 0; i < coreCount; i++) {
    int height = (int)100 / ROWS * cores[i];
    display.drawRect(width * i, 0, width, height, WHITE)
  }
}


void renderMemory() {
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(padAsPercentage(cpu));
}


void renderSplash() {
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("PC Stats");
}


char[] padAsPercentage(byte number) {
  char out[4];
  int percent = (int)number;
  if (i < 10) {
    sprintf(out, "  %d%%", percent);
  } else if (i < 100) {
    sprintf(out, " %d%%", percent);
  } else {
    sprintf(out, "%d%%", percent);
  }

  return out;
}


void loop() {
  parse(
    readFromSerial()
  );
  render();
}
