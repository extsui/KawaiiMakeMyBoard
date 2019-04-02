#include <Wire.h>
#include "Grass.h"

Grass::Grass(uint8_t addr)
{
  m_addr = addr;
  memset(m_data, 0x00, sizeof(m_data));
}

void Grass::config(uint8_t brightness)
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

void Grass::on(uint8_t index)
{
  m_data[index] = 1;
}

void Grass::off(uint8_t index)
{
  m_data[index] = 0;
}

/**
 * 草LED配置
 * 
 * D3      D5      D7      D9
 *     D4      D6      D8    
 *                           
 * D2    D1          D11  D10
 */

/**
 * 草LEDの点灯パターン
 * 
 * ・左から右へ(1個)
 * ・右から左へ(1個)
 * ・左から右へ(3個)
 * ・右から左へ(3個)
 * ・両端から真ん中へ
 * ・
 */

const uint8_t PATTERN_LEFT_TO_RIGHT_1[][GRASS_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

const uint8_t PATTERN_RIGHT_TO_LEFT_1[][GRASS_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

const uint8_t PATTERN_LEFT_TO_RIGHT_3[][GRASS_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

const uint8_t PATTERN_RIGHT_TO_LEFT_3[][GRASS_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

const uint8_t PATTERN_BOTH_EDGE_TO_MIDDLE[][GRASS_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, },
  { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

void Grass::setLeftToRight1(void)
{
  m_state = 0;
  m_index = 0;
}

void Grass::setRightToLeft1(void)
{
  m_state = 1;
  m_index = 0;
}

void Grass::setLeftToRight3(void)
{
  m_state = 2;
  m_index = 0;
}

void Grass::setRightToLeft3(void)
{
  m_state = 3;
  m_index = 0;
}

void Grass::setBothEdgeToMiddle(void)
{
  m_state = 4;
  m_index = 0;
}

void Grass::next(void)
{
  switch (m_state) {
  case 0:
    memcpy(m_data, PATTERN_LEFT_TO_RIGHT_1[m_index], GRASS_LED_NUM);
    break;
  
  case 1:
    memcpy(m_data, PATTERN_RIGHT_TO_LEFT_1[m_index], GRASS_LED_NUM);
    break;

  case 2:
    memcpy(m_data, PATTERN_LEFT_TO_RIGHT_3[m_index], GRASS_LED_NUM);
    break;
  
  case 3:
    memcpy(m_data, PATTERN_RIGHT_TO_LEFT_3[m_index], GRASS_LED_NUM);
    break;

  case 4:
    memcpy(m_data, PATTERN_BOTH_EDGE_TO_MIDDLE[m_index], GRASS_LED_NUM);
    break;
    
  default:
    break;
  }

  m_index++;
}

void Grass::update()
{
  uint8_t data[GRASS_LED_NUM/8 + 1];

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
