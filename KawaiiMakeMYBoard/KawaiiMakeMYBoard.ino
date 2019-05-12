#include <M5Stack.h>
#include <Arduino.h>
#include <Wire.h>
#include "Tree.h"
#include "Grass.h"
#include "Land.h"

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

void loop()
{
  static uint32_t next_update_time_ms = 0;
  uint32_t current_time_ms;
  
  current_time_ms = millis();

  // 一定周期でエフェクト更新
  if (next_update_time_ms <= current_time_ms) {
    next_update_time_ms += 5000;

    land->set((LandPattern)random(0, LAND_PATTERN_NUM));
    grass_front->set((GrassPattern)random(0, GRASS_PATTERN_NUM));
    grass_middle->set((GrassPattern)random(0, GRASS_PATTERN_NUM));
    tree->set((TreePattern)random(0, TREE_PATTERN_NUM));
  } else {
    step();
  }

  static int brightness = 1;

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
