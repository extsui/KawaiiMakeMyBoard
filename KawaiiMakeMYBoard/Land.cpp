#include <Wire.h>
#include "Land.h"

Land::Land(uint8_t addr)
{
  m_addr = addr;
  memset(m_data, 0x00, sizeof(m_data));
}

void Land::config(uint8_t brightness)
{
  // オシレータON
  Wire.beginTransmission(m_addr);
  Wire.write(0x21);
  Wire.endTransmission();

  // 輝度設定(0~15)
  Wire.beginTransmission(m_addr);
  Wire.write(0xE0 | brightness);
  Wire.endTransmission();
  
  // 点滅設定
  Wire.beginTransmission(m_addr);
  Wire.write(0x80 | 0x01 | 0);
  Wire.endTransmission();
  
  update();
}

void Land::on(uint8_t index)
{
  m_data[index] = 1;
}

void Land::off(uint8_t index)
{
  m_data[index] = 0;
}

void Land::update()
{
  uint8_t data[LAND_LED_NUM/8 + 1];

  memset(data, 0, sizeof(data));
  
  // m_data[0]をdata[0]の0bit目とする
  for (int i = 0; i < sizeof(data); i++) {
    for (int bit = 0; bit < 8; bit++) {
      if (m_data[i*8 + bit] == 1) {
        data[i] |= (1<<bit);
      }
    }
  }
  
  Wire.beginTransmission(m_addr);
  Wire.write(0x00);
  for (int i = 0; i < sizeof(data); i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
}

/*
  if ((0 <= index) && (index <= 7)) {
    m_data[0] &= ~(1 << (index - 0));
  }
  if ((8 <= index) && (index <= 15)) {
    m_data[1] &= ~(1 << (index - 8));
  }
  if ((16 <= index) && (index <= 23)) {
    m_data[2] &= ~(1 << (index - 16));
  }
  if ((24 <= index) && (index <= 31)) {
    m_data[3] &= ~(1 << (index - 24));
  }
 */
