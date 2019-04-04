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
  /*
  grass_front->set(2);

  for (int i = 0; i < 14; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  grass_front->set(3);

  for (int i = 0; i < 14; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  grass_front->set(4);

  for (int i = 0; i < 12; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  grass_front->set(5);

  for (int i = 0; i < 20; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }

  for (int i = 0; i < 3; i++) {
    grass_front->set(6);
    grass_front->update();
    delay(delay_ms * 4);
    
    grass_front->set(7);
    grass_front->update();
    delay(delay_ms * 4);
  }
  */
  /*
  grass_front->set(8);

  for (int i = 0; i < 67*2; i++) {
    grass_front->update();
    delay(delay_ms / 2);
    grass_front->next();
  }
  */
  /*
  grass_front->set(9);

  for (int i = 0; i < 14; i++) {
    grass_front->update();
    delay(delay_ms);
    grass_front->next();
  }
  */
  
  grass_front->set(10);

  for (int i = 0; i < 8; i++) {
    grass_front->update();
    delay(delay_ms / 2);
    grass_front->next();
  }
  
  grass_front->set(11);

  for (int i = 0; i < 8; i++) {
    grass_front->update();
    delay(delay_ms / 2);
    grass_front->next();
  }
}
