#include <Wire.h>
#include "Grass.h"

/************************************************************
 *  点灯パターン定義
 ************************************************************/
/**
 * 草LED配置
 * 
 * D3      D5      D7      D9
 *     D4      D6      D8    
 *                           
 * D2    D1         D11   D10
 */

static const uint8_t PATTERN_ALL_ON[][GRASS_LED_NUM] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

static const uint8_t PATTERN_ALL_OFF[][GRASS_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_LEFT_TO_RIGHT_1[][GRASS_LED_NUM] = {
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

static const uint8_t PATTERN_RIGHT_TO_LEFT_1[][GRASS_LED_NUM] = {
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

static const uint8_t PATTERN_LEFT_TO_RIGHT_3[][GRASS_LED_NUM] = {
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

static const uint8_t PATTERN_RIGHT_TO_LEFT_3[][GRASS_LED_NUM] = {
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

static const uint8_t PATTERN_BOTH_EDGE_TO_MIDDLE[][GRASS_LED_NUM] = {
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

static const uint8_t PATTERN_VIBRATION[][GRASS_LED_NUM] = {
  { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
  { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
};

static const uint8_t PATTERN_LEFT_TO_RIGHT_BUFFER[][GRASS_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
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
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, },
  { 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, },
  { 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },

  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, },
  { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, },
  { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, },
  { 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, },
  { 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
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

static const uint8_t PATTERN_LEFT_TO_RIGHT_NEG[][GRASS_LED_NUM] = {
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

static const uint8_t PATTERN_RIGHT_TO_LEFT_NEG[][GRASS_LED_NUM] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, },
  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, },
  { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, },
  { 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, },
  { 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, },
  { 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, },
  { 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, },
  { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

static const uint8_t PATTERN_LEFT_TO_RIGHT_VERTICAL[][GRASS_LED_NUM] = {
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

static const uint8_t PATTERN_RIGHT_TO_LEFT_VERTICAL[][GRASS_LED_NUM] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, },
  { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, },
  { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};

/************************************************************
 *  点灯パターン一覧
 ************************************************************/
typedef struct {
  const uint8_t (*pattern)[GRASS_LED_NUM];
  int frame_count;
} GrassPatternRecord;

static const GrassPatternRecord GRASS_PATTERN_TABLE[] = {
  { PATTERN_ALL_ON,                     1   },
  { PATTERN_ALL_OFF,                    1   },
  { PATTERN_LEFT_TO_RIGHT_1,            12  },
  { PATTERN_RIGHT_TO_LEFT_1,            12  },
  { PATTERN_LEFT_TO_RIGHT_3,            14  },
  { PATTERN_RIGHT_TO_LEFT_3,            14  },
  { PATTERN_BOTH_EDGE_TO_MIDDLE,        12  },
  { PATTERN_VIBRATION,                  2   },
  { PATTERN_LEFT_TO_RIGHT_BUFFER,       134 },
  { PATTERN_LEFT_TO_RIGHT_NEG,          14  },
  { PATTERN_RIGHT_TO_LEFT_NEG,          14  },
  { PATTERN_LEFT_TO_RIGHT_VERTICAL,     8   },
  { PATTERN_RIGHT_TO_LEFT_VERTICAL,     8   }, 
};

/************************************************************
 *  メソッド定義
 ************************************************************/
Grass::Grass(uint8_t addr)
{
  m_addr = addr;
  memset(m_data, 0x00, sizeof(m_data));
  m_pattern_index = 0;
  m_frame_index = 0;
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

int Grass::set(GrassPattern pattern)
{
  if (pattern >= GRASS_PATTERN_NUM) {
    return -1;
  }

  m_pattern_index = pattern;
  m_frame_index = 0;

  // パターンをセットした時点で表示を更新されても
  // 問題ないようにするため先頭データを読み込んでおく。
  next();
    
  return 0;
}

int Grass::length(void)
{
  return GRASS_PATTERN_TABLE[m_pattern_index].frame_count;
}

void Grass::next(void)
{ 
  const uint8_t (*current_pattern)[GRASS_LED_NUM] = GRASS_PATTERN_TABLE[m_pattern_index].pattern;
  int frame_count = GRASS_PATTERN_TABLE[m_pattern_index].frame_count;
  
  // ループ可能にするためにフレーム数を超えたら先頭フレームに戻す
  memcpy(m_data, current_pattern[m_frame_index], GRASS_LED_NUM);
  m_frame_index = (m_frame_index + 1) % frame_count;
}

void Grass::update()
{
  uint8_t data[GRASS_LED_NUM/8 + 1];
  
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
void Grass::test()
{
  int delay_ms = 100;
  
  this->set(GRASS_PATTERN_LEFT_TO_RIGHT_3);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  this->set(GRASS_PATTERN_RIGHT_TO_LEFT_3);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  this->set(GRASS_PATTERN_BOTH_EDGE_TO_MIDDLE);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  this->set(GRASS_PATTERN_VIBRATION);

  for (int i = 0; i < this->length() * 10; i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }

  for (int i = 0; i < this->length(); i++) {
    this->set(GRASS_PATTERN_ALL_ON);
    this->update();
    delay(delay_ms * 4);
    
    this->set(GRASS_PATTERN_ALL_OFF);
    this->update();
    delay(delay_ms * 4);
  }
  
  this->set(GRASS_PATTERN_LEFT_TO_RIGHT_BUFFER);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms / 2);
    this->next();
  }
  
  this->set(GRASS_PATTERN_LEFT_TO_RIGHT_NEG);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(GRASS_PATTERN_RIGHT_TO_LEFT_NEG);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms);
    this->next();
  }
  
  this->set(GRASS_PATTERN_LEFT_TO_RIGHT_VERTICAL);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms / 2);
    this->next();
  }
  
  this->set(GRASS_PATTERN_RIGHT_TO_LEFT_VERTICAL);

  for (int i = 0; i < this->length(); i++) {
    this->update();
    delay(delay_ms / 2);
    this->next();
  }
}
