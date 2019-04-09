#include <Wire.h>
#include "Land.h"

/************************************************************
 *  点灯パターン定義
 ************************************************************/
/**
 * 土LED配置
 * 
 * D7  D6  D5  D4  D3  D2  D1
 * D8
 * D9
 * D10 D11 D12 D13 D14 D15 D16
 *                         D17
 *                         D18
 * D25 D24 D23 D22 D21 D20 D19
 */
static const uint8_t PATTERN_ALL_ON[][LAND_LED_NUM] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, },
};

static const uint8_t PATTERN_ALL_OFF[][LAND_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_ONE_BY_ONE[][LAND_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 0, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  0, 0, 0, 0, 0, },
  
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, },
};

/************************************************************
 *  点灯パターン一覧
 ************************************************************/
typedef struct {
  const uint8_t (*pattern)[LAND_LED_NUM];
  int frame_count;
} LandPatternRecord;

static const LandPatternRecord LAND_PATTERN_TABLE[] = {
  { PATTERN_ALL_ON,                     1   },
  { PATTERN_ALL_OFF,                    1   },
  { PATTERN_ONE_BY_ONE,                 26  },
};

/************************************************************
 *  メソッド定義
 ************************************************************/
Land::Land(uint8_t addr)
{
  m_addr = addr;
  memset(m_data, 0x00, sizeof(m_data));
  m_pattern_index = 0;
  m_frame_index = 0;
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

int Land::set(LandPattern pattern)
{
  if (pattern >= LAND_PATTERN_NUM) {
    return -1;
  }

  m_pattern_index = pattern;
  m_frame_index = 0;

  // パターンをセットした時点で表示を更新されても
  // 問題ないようにするため先頭データを読み込んでおく。
  next();
    
  return 0;
}

int Land::length(void)
{
  return LAND_PATTERN_TABLE[m_pattern_index].frame_count;
}

void Land::next(void)
{ 
  const uint8_t (*current_pattern)[LAND_LED_NUM] = LAND_PATTERN_TABLE[m_pattern_index].pattern;
  int frame_count = LAND_PATTERN_TABLE[m_pattern_index].frame_count;
  
  // ループ可能にするためにフレーム数を超えたら先頭フレームに戻す
  memcpy(m_data, current_pattern[m_frame_index], LAND_LED_NUM);
  m_frame_index = (m_frame_index + 1) % frame_count;
}

void Land::update()
{
  uint8_t data[LAND_LED_NUM/8 + 1];

  memset(data, 0, sizeof(data));

  data[0] |= ((m_data[0] == 1) ? 0x01 : 0);
  data[0] |= ((m_data[1] == 1) ? 0x02 : 0);
  data[0] |= ((m_data[2] == 1) ? 0x04 : 0);
  data[0] |= ((m_data[3] == 1) ? 0x08 : 0);
  data[0] |= ((m_data[4] == 1) ? 0x10 : 0);
  data[0] |= ((m_data[5] == 1) ? 0x20 : 0);
  data[0] |= ((m_data[6] == 1) ? 0x40 : 0);
  data[0] |= ((m_data[7] == 1) ? 0x80 : 0);

  data[1] |= ((m_data[8]  == 1) ? 0x01 : 0);
  data[1] |= ((m_data[9]  == 1) ? 0x02 : 0);
  data[1] |= ((m_data[10] == 1) ? 0x04 : 0);
  data[1] |= ((m_data[11] == 1) ? 0x08 : 0);
  data[1] |= ((m_data[12] == 1) ? 0x10 : 0);
  data[1] |= ((m_data[13] == 1) ? 0x20 : 0);
  data[1] |= ((m_data[14] == 1) ? 0x40 : 0);
  data[1] |= ((m_data[15] == 1) ? 0x80 : 0);

  data[2] |= ((m_data[16] == 1) ? 0x01 : 0);
  data[2] |= ((m_data[17] == 1) ? 0x02 : 0);
  data[2] |= ((m_data[18] == 1) ? 0x04 : 0);
  data[2] |= ((m_data[19] == 1) ? 0x08 : 0);
  data[2] |= ((m_data[20] == 1) ? 0x10 : 0);
  data[2] |= ((m_data[21] == 1) ? 0x20 : 0);
  data[2] |= ((m_data[22] == 1) ? 0x40 : 0);
  data[2] |= ((m_data[23] == 1) ? 0x80 : 0);

  data[3] |= ((m_data[24] == 1) ? 0x01 : 0);

  Wire.beginTransmission(m_addr);
  Wire.write(0x00);
  for (int i = 0; i < sizeof(data); i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
}

/************************************************************
 *  サンプル
 ************************************************************/
void Land::test()
{
  int delay_ms = 100;
  
  for (int i = 0; i < this->length(); i++) {
    this->set(LAND_PATTERN_ALL_ON);
    this->update();
    delay(delay_ms * 4);
    
    this->set(LAND_PATTERN_ALL_OFF);
    this->update();
    delay(delay_ms * 4);
  }

  this->set(LAND_PATTERN_ONE_BY_ONE);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
}
