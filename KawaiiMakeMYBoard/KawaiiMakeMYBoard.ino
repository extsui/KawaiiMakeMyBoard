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
  Serial.print("OK.\n");
}

void loop()
{
  Serial.println("loop");
  M5.Lcd.printf("loop");

  for (int i = 0; i < 25; i++) {
    land->on(i);
    land->update();
    grass_front->on(i);
    grass_front->update();
    grass_middle->on(i);
    grass_middle->update();
    tree->on(i);
    tree->update();
    delay(100);
  }

  for (int brightness = 1; brightness < 16; brightness++) {
    land->config(brightness);
    grass_front->config(brightness);
    grass_middle->config(brightness);
    tree->config(brightness);
    delay(100);
  }

  for (int brightness = 16; brightness >= 1; brightness--) {
    land->config(brightness);
    grass_front->config(brightness);
    grass_middle->config(brightness);
    tree->config(brightness);
    delay(100);
  }

  for (int i = 0; i < 25; i++) {
    land->off(i);
    land->update();
    grass_front->off(i);
    grass_front->update();
    grass_middle->off(i);
    grass_middle->update();
    tree->off(i);
    tree->update();
    delay(100);
  }

  int brightness = 1;
  land->config(brightness);
  grass_front->config(brightness);
  grass_middle->config(brightness);
  tree->config(brightness);
}
