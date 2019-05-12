#include <M5Stack.h>
#include <Arduino.h>
#include <Wire.h>
#include "Tree.h"
#include "Grass.h"
#include "Land.h"

#define KMMB_VERSION  "v0.1"

Land *land;
Grass *grass_front;
Grass *grass_middle;
Tree *tree;

void setup()
{
  M5.begin();

  Serial.begin(115200);
  while (!Serial);
  Serial.println("begin...");

  Wire.begin(SDA, SCL, 400000);

  land = new Land(0x70);
  grass_front = new Grass(0x71);
  grass_middle = new Grass(0x72);
  tree = new Tree(0x73);

  land->config(1);
  grass_front->config(1);
  grass_middle->config(1);
  tree->config(1);

  // ボタン初期化
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);

  M5.Lcd.fillScreen(ILI9341_BLACK);

  Serial.print("OK.\n");
}

void step(void)
{
  land->next();
  grass_front->next();
  grass_middle->next();
  tree->next();

  land->update();
  grass_front->update();
  grass_middle->update();
  tree->update();
}

void setBrightness(uint8_t brightness)
{
  land->config(brightness);
  grass_front->config(brightness);
  grass_middle->config(brightness);
  tree->config(brightness);
}

// 各パーツの輝度(共通)
static int brightness = 1;

void loop()
{
  static uint32_t next_update_time_ms = 0;
  uint32_t current_time_ms;
  
  current_time_ms = millis();

  // 一定周期でエフェクト更新
  if (next_update_time_ms <= current_time_ms) {
    next_update_time_ms += 5000;

    uint8_t patterns[4];
    patterns[0] = random(0, LAND_PATTERN_NUM);
    patterns[1] = random(0, GRASS_PATTERN_NUM);
    patterns[2] = random(0, GRASS_PATTERN_NUM);
    patterns[3] = random(0, TREE_PATTERN_NUM);

    land->set((LandPattern)patterns[0]);
    grass_front->set((GrassPattern)patterns[1]);
    grass_middle->set((GrassPattern)patterns[2]);
    tree->set((TreePattern)patterns[3]);

    drawScreen(patterns);
  } else {
    step();
  }

  // ボタンAで輝度DOWN
  if (M5.BtnA.isPressed()) {
    if (brightness >= 1) {
      brightness--;
    }
  }

  // ボタンBが押されている間更新停止
  if (M5.BtnB.isPressed()) {
    while (M5.BtnB.isPressed()) {
      M5.update();
    }
  }

  // ボタンCで輝度UP
  if (M5.BtnC.isPressed()) {
    if (brightness < 15) {
      brightness++;
    }
  }

  setBrightness(brightness);

  // ボタン情報更新
  M5.update();

  delay(50);
}

#define TEXT_HEIGHT (16)

void drawScreen(uint8_t patterns[])
{
  /*
   * [画面サンプル]
   * ------------------------------
   * KawaiiMakeMYBoard <version>
   *   0x70 Land  <pattern#>
   *   0x71 Grass <pattern#>
   *   0x72 Grass <pattern#>
   *   0x73 Tree  <pattern#>
   * ------------------------------
   */
  char str[32];

  struct {
    char *text;
    uint16_t color;
  } format[] = {
    { "KawaiiMakeMYBoard %s", ILI9341_WHITE,  },
    { "  0x70 Land  %d",      ILI9341_YELLOW, },
    { "  0x71 Grass %d",      ILI9341_GREEN,  },
    { "  0x72 Grass %d",      ILI9341_GREEN,  },
    { "  0x73 Tree  %d",      ILI9341_BLUE,   },
  };

  M5.Lcd.fillScreen(ILI9341_BLACK);

  M5.Lcd.setTextSize(2);

  M5.Lcd.setTextColor(format[0].color);
  M5.Lcd.setCursor(0, 0*TEXT_HEIGHT);
  sprintf(str, format[0].text, KMMB_VERSION, brightness);
  M5.Lcd.print(str);

  for (int i = 1; i <= 4; i++) {
    M5.Lcd.setTextColor(format[i].color);
    M5.Lcd.setCursor(0, (i)*TEXT_HEIGHT);
    sprintf(str, format[i].text, patterns[i-1]);
    M5.Lcd.print(str);
  }
  // TODO: 家用
  //M5.Lcd.setTextColor(ILI9341_RED);
}
