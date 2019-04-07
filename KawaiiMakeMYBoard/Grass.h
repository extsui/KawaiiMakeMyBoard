#ifndef GRASS_H
#define GRASS_H

#define GRASS_LED_NUM   (11)

/** 草の点灯パターン */
typedef enum {
  GRASS_PATTERN_ALL_ON = 0,             /**< 全点灯 */
  GRASS_PATTERN_ALL_OFF,                /**< 全消灯 */
  GRASS_PATTERN_LEFT_TO_RIGHT_1,        /**< 左から右へ(1個ずつ) */
  GRASS_PATTERN_RIGHT_TO_LEFT_1,        /**< 右から左へ(1個ずつ) */
  GRASS_PATTERN_LEFT_TO_RIGHT_3,        /**< 左から右へ(3個ずつ) */
  GRASS_PATTERN_RIGHT_TO_LEFT_3,        /**< 右から左へ(3個ずつ) */
  GRASS_PATTERN_BOTH_EDGE_TO_MIDDLE,    /**< 両端から真ん中へ */
  GRASS_PATTERN_VIBRATION,              /**< 交互の連続反転 */
  GRASS_PATTERN_LEFT_TO_RIGHT_BUFFER,   /**< 左から右へバッファ */
  GRASS_PATTERN_RIGHT_TO_LEFT_NEG,      /**< 右から左へ(反転) */
  GRASS_PATTERN_LEFT_TO_RIGHT_VERTICAL, /**< 左から右へ(縦) */
  GRASS_PATTERN_RIGHT_TO_LEFT_VERTICAL, /**< 右から左へ(縦) */
  GRASS_PATTERN_NUM,
} GrassPattern;

class Grass
{
public:
  Grass(uint8_t addr);
  void config(uint8_t brightness);
  int set(GrassPattern pattern);
  void next();
  void update();

private:
  // I2Cアドレス
  uint8_t m_addr;
  // 現在の設定データ
  uint8_t m_data[GRASS_LED_NUM];
  // 現在のパターン
  const uint8_t (*m_current_pattern)[GRASS_LED_NUM];
  // 現在のパターンのフレーム数
  uint8_t m_frame_count;
  // 現在のパターンのフレームのインデックス
  uint8_t m_frame_index;
};

#endif /* GRASS_H */
