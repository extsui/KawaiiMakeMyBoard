#ifndef LAND_H
#define LAND_H

#define LAND_LED_NUM   (25)

/** 土の点灯パターン */
typedef enum {
  LAND_PATTERN_ALL_ON = 0,                  /**< 全点灯 */
  LAND_PATTERN_ALL_OFF,                     /**< 全消灯 */
  LAND_PATTERN_ONE_BY_ONE,                  /**< 1個ずつ */
  LAND_PATTERN_STREAM,                      /**< 小川 */
  LAND_PATTERN_NUM,
} LandPattern;

class Land
{
public:
  Land(uint8_t addr);
  /** 輝度設定(I2C通信有) */
  void config(uint8_t brightness);
  /** パターン設定 */
  int set(LandPattern pattern);
  /** 現在のパターンの長さ */
  int length();
  /** 現在のパターンを進める */
  void next();
  /** 表示更新(I2C通信有) */
  void update();
  /** テスト */
  void test();

private:
  /** I2Cアドレス */
  uint8_t m_addr;
  /** 現在の設定データ */
  uint8_t m_data[LAND_LED_NUM];
  /** 現在のパターンのインデックス */
  int m_pattern_index;
  /** 現在のパターンのフレームのインデックス */
  int m_frame_index;
};

#endif /* LAND_H */
