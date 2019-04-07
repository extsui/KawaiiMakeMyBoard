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
  int delay_ms = 100;
  
  Serial.println("loop");
  M5.Lcd.printf("loop");
  
  grass_front->set(GRASS_PATTERN_LEFT_TO_RIGHT_3);

  for (int i = 0; i < 14; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  grass_front->set(GRASS_PATTERN_RIGHT_TO_LEFT_3);

  for (int i = 0; i < 14; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  grass_front->set(GRASS_PATTERN_BOTH_EDGE_TO_MIDDLE);

  for (int i = 0; i < 12; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  grass_front->set(GRASS_PATTERN_VIBRATION);

  for (int i = 0; i < 20; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  for (int i = 0; i < 3; i++) {
    grass_front->set(GRASS_PATTERN_ALL_ON);
    grass_front->update();
    delay(delay_ms * 4);
    
    grass_front->set(GRASS_PATTERN_ALL_OFF);
    grass_front->update();
    delay(delay_ms * 4);
  }
  
  grass_front->set(GRASS_PATTERN_LEFT_TO_RIGHT_BUFFER);

  for (int i = 0; i < 67*2; i++) {
    grass_front->update();
    delay(delay_ms / 2);
    grass_front->next();
  }
  
  grass_front->set(GRASS_PATTERN_RIGHT_TO_LEFT_NEG);

  for (int i = 0; i < 14; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }
  
  grass_front->set(GRASS_PATTERN_LEFT_TO_RIGHT_VERTICAL);

  for (int i = 0; i < 8; i++) {
    grass_front->update();
    delay(delay_ms / 2);
    grass_front->next();
  }
  
  grass_front->set(GRASS_PATTERN_RIGHT_TO_LEFT_VERTICAL);

  for (int i = 0; i < 8; i++) {
    grass_front->update();
    delay(delay_ms / 2);
    grass_front->next();
  }
}
