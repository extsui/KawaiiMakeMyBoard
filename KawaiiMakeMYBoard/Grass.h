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
  /** 輝度設定(I2C通信有) */
  void config(uint8_t brightness);
  /** パターン設定 */
  int set(GrassPattern pattern);
  /** 現在のパターンの長さ */
  int length();
  /** 現在のパターンを進める */
  void next();
  /** 表示更新(I2C通信有) */
  void update();

private:
  /** I2Cアドレス */
  uint8_t m_addr;
  /** 現在の設定データ */
  uint8_t m_data[GRASS_LED_NUM];
  /** 現在のパターンのインデックス */
  int m_pattern_index;
  /** 現在のパターンのフレームのインデックス */
  int m_frame_index;
};

#endif /* GRASS_H */
