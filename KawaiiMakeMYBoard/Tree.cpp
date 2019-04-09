#include <Wire.h>
#include "Tree.h"

/************************************************************
 *  点灯パターン定義
 ************************************************************/
/**
 * 木LED配置
 * 
 * 物理配置が分かりにくいため変換テーブルを噛まして
 * 論理配置でパターンを指定できるようにする。
 * 
 * ------------------------------
 * 論理配置(こちらを使用)
 * ------------------------------
 *           D1
 * 
 *        D2    D3
 * 
 *     D4    D5    D6
 * 
 *   D7   D8    D9    D10
 * 
 * D11  D12  D13   D14   D15
 * 
 *       D16    D17
 * 
 *       D18    D19
 * ------------------------------
 * 物理配置
 * ------------------------------
 *           D7
 * 
 *       D10    D6
 * 
 *     D11   D9    D5
 * 
 *   D13  D12   D4    D3
 * 
 * D15  D14  D8    D2    D1
 * 
 *       D19    D17
 * 
 *       D18    D16
 * ------------------------------
 */
static const uint8_t PHYSICAL_TO_LOGICAL_POS_TABLE[] = {
//D1  D2  D3  D4  D5  D6  D7  D8  D9  D10 (PHY)
  15, 14, 10, 9,  6,  3,  1,  13, 5,  2,
//D11 D12 D13 D14 D15 D16 D17 D18 D19 (PHY)
  4,  8,  7,  12, 11, 19, 17, 18, 16,
};

static const uint8_t PATTERN_ALL_ON[][TREE_LED_NUM] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

static const uint8_t PATTERN_ALL_OFF[][TREE_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_ONE_BY_ONE[][TREE_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_TOP_TO_BOTTOM_HORIZONTAL_1[][TREE_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_TOP_TO_BOTTOM_HORIZONTAL_3[][TREE_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_TOP_TO_BOTTOM_BUFFER[][TREE_LED_NUM] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_UPPER_LEFT_TO_RIGHT[][TREE_LED_NUM] = {
  { 1, 1, 0, 1, 0, 0, 1, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 0, 1, 0, 0, 1, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_UPPER_RIGHT_TO_LEFT[][TREE_LED_NUM] = {
  { 1, 0, 1, 0, 0, 1, 0, 0, 0, 1,  0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 1, 0, 0, 1, 0, 0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_LOWER_LEFT_TO_RIGHT[][TREE_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 1, 0, 1, 0, 0, 1, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 1, 0, 0, 1, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_LOWER_RIGHT_TO_LEFT[][TREE_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 0, 0, 1, 0, 0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 1, 0, 1, 0, 0, 1, 0, 0, 0, 1,  0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_CANDLE[][TREE_LED_NUM] = {
  // 赤(外周)
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 0, 1, 1, 0, 0, 1,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  // 黄(内周)
  { 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,  1, 0, 1, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,  1, 0, 1, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },

  { 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,  1, 0, 1, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,  1, 0, 1, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },

  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,  1, 0, 1, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,  1, 0, 1, 0, 1, 1, 1, 1, 1, },
  
  { 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 0, 1, 1, 0, 0, 1,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 1, },
 
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

/************************************************************
 *  点灯パターン一覧
 ************************************************************/
typedef struct {
  const uint8_t (*pattern)[TREE_LED_NUM];
  int frame_count;
} TreePatternRecord;

static const TreePatternRecord TREE_PATTERN_TABLE[] = {
  { PATTERN_ALL_ON,                     1   },
  { PATTERN_ALL_OFF,                    1   },
  { PATTERN_ONE_BY_ONE,                 20  },
  { PATTERN_TOP_TO_BOTTOM_HORIZONTAL_1, 8   },
  { PATTERN_TOP_TO_BOTTOM_HORIZONTAL_3, 10  },
  { PATTERN_TOP_TO_BOTTOM_BUFFER,       14  },
  { PATTERN_UPPER_LEFT_TO_RIGHT,        6   },
  { PATTERN_UPPER_RIGHT_TO_LEFT,        6   },
  { PATTERN_LOWER_LEFT_TO_RIGHT,        6   },
  { PATTERN_LOWER_RIGHT_TO_LEFT,        6   },
  { PATTERN_CANDLE,                     22  },
};

/************************************************************
 *  メソッド定義
 ************************************************************/
Tree::Tree(uint8_t addr)
{
  m_addr = addr;
  memset(m_data, 0x00, sizeof(m_data));
  m_pattern_index = 0;
  m_frame_index = 0;
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

int Tree::set(TreePattern pattern)
{
  if (pattern >= TREE_PATTERN_NUM) {
    return -1;
  }

  m_pattern_index = pattern;
  m_frame_index = 0;

  // パターンをセットした時点で表示を更新されても
  // 問題ないようにするため先頭データを読み込んでおく。
  next();
    
  return 0;
}

int Tree::length(void)
{
  return TREE_PATTERN_TABLE[m_pattern_index].frame_count;
}

void Tree::next(void)
{ 
  const uint8_t (*current_pattern)[TREE_LED_NUM] = TREE_PATTERN_TABLE[m_pattern_index].pattern;
  int frame_count = TREE_PATTERN_TABLE[m_pattern_index].frame_count;
  
  // ループ可能にするためにフレーム数を超えたら先頭フレームに戻す
  memcpy(m_data, current_pattern[m_frame_index], TREE_LED_NUM);
  m_frame_index = (m_frame_index + 1) % frame_count;
}

void Tree::update()
{
  uint8_t data[TREE_LED_NUM/8 + 1];

  memset(data, 0, sizeof(data));

  // 回路の制約上、[15]はdata[1]の7bit目ではなくdata[2]の0bit目。
  // また、物理論理変換が必要なこともあって直接書き下す実装とした。
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[0] - 1] == 1) ? 0x01 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[1] - 1] == 1) ? 0x02 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[2] - 1] == 1) ? 0x04 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[3] - 1] == 1) ? 0x08 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[4] - 1] == 1) ? 0x10 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[5] - 1] == 1) ? 0x20 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[6] - 1] == 1) ? 0x40 : 0);
  data[0] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[7] - 1] == 1) ? 0x80 : 0);

  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[8] - 1]  == 1) ? 0x01 : 0);
  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[9] - 1]  == 1) ? 0x02 : 0);
  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[10] - 1] == 1) ? 0x04 : 0);
  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[11] - 1] == 1) ? 0x08 : 0);
  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[12] - 1] == 1) ? 0x10 : 0);
  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[13] - 1] == 1) ? 0x20 : 0);
  data[1] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[14] - 1] == 1) ? 0x40 : 0);

  data[2] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[15] - 1] == 1) ? 0x01 : 0);
  data[2] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[16] - 1] == 1) ? 0x02 : 0);
  data[2] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[17] - 1] == 1) ? 0x04 : 0);
  data[2] |= ((m_data[PHYSICAL_TO_LOGICAL_POS_TABLE[18] - 1] == 1) ? 0x08 : 0);

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
void Tree::test()
{
  int delay_ms = 100;
  
  for (int i = 0; i < this->length(); i++) {
    this->set(TREE_PATTERN_ALL_ON);
    this->update();
    delay(delay_ms * 4);
    
    this->set(TREE_PATTERN_ALL_OFF);
    this->update();
    delay(delay_ms * 4);
  }

  this->set(TREE_PATTERN_ONE_BY_ONE);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  this->set(TREE_PATTERN_TOP_TO_BOTTOM_HORIZONTAL_1);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(TREE_PATTERN_TOP_TO_BOTTOM_HORIZONTAL_3);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(TREE_PATTERN_TOP_TO_BOTTOM_BUFFER);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(TREE_PATTERN_LOWER_LEFT_TO_RIGHT);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  this->set(TREE_PATTERN_LOWER_RIGHT_TO_LEFT);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(TREE_PATTERN_UPPER_LEFT_TO_RIGHT);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(TREE_PATTERN_UPPER_RIGHT_TO_LEFT);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  this->set(TREE_PATTERN_CANDLE);
  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
}
