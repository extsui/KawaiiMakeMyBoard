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

int Grass::set(int pattern)
{
  // TODO: パターン番号異常の場合は-1を返す
  
  switch (pattern) {
  case 0:
    m_current_pattern = PATTERN_LEFT_TO_RIGHT_1;
    //m_frame_count = xxx;  // TODO:
    break;
  
  case 1:
    m_current_pattern = PATTERN_RIGHT_TO_LEFT_1;
    break;

  case 2:
    m_current_pattern = PATTERN_LEFT_TO_RIGHT_3;
    break;
  
  case 3:
    m_current_pattern = PATTERN_RIGHT_TO_LEFT_3;
    break;

  case 4:
    m_current_pattern = PATTERN_BOTH_EDGE_TO_MIDDLE;
    break;
    
  default:
    break;
  }

  m_frame_index = 0;
  return 0;
}

void Grass::next(void)
{
  memcpy(m_data, m_current_pattern[m_frame_index], GRASS_LED_NUM);
  m_frame_index++;

  // TODO: フレーム数を超えたら0に戻す処理を入れる
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
