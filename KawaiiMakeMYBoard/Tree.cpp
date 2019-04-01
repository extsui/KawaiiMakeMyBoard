#include <Wire.h>
#include "Tree.h"

Tree::Tree(uint8_t addr)
{
  m_addr = addr;
  memset(m_data, 0x00, sizeof(m_data));
}

void Tree::config(uint8_t brightness)
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

void Tree::on(uint8_t index)
{
  if ((0 <= index) && (index <= 7)) {
    m_data[0] |= (1 << (index - 0));
  }
  if ((8 <= index) && (index <= 15)) {
    m_data[1] |= (1 << (index - 8));
  }
  if ((16 <= index) && (index <= 23)) {
    m_data[2] |= (1 << (index - 16));
  }
  if ((24 <= index) && (index <= 31)) {
    m_data[3] |= (1 << (index - 24));
  }
}

void Tree::off(uint8_t index)
{
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
}

void Tree::update()
{
  Wire.beginTransmission(m_addr);
  Wire.write(0x00);
  Wire.write(m_data[0]);
  Wire.write(m_data[1]);
  Wire.write(m_data[2]);
  Wire.write(m_data[3]);
  Wire.endTransmission();
}
