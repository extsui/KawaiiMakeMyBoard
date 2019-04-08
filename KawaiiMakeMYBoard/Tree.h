#ifndef TREE_H
#define TREE_H

#define TREE_LED_NUM   (19)

/** 草の点灯パターン */
typedef enum {
  TREE_PATTERN_ALL_ON = 0,                  /**< 全点灯 */
  TREE_PATTERN_ALL_OFF,                     /**< 全消灯 */
  TREE_PATTERN_ONE_BY_ONE,                  /**< 1個ずつ */
  TREE_PATTERN_TOP_TO_BOTTOM_HORIZONTAL_1,  /**< 上から下へ(1行) */
  TREE_PATTERN_TOP_TO_BOTTOM_HORIZONTAL_3,  /**< 上から下へ(3行) */
  TREE_PATTERN_TOP_TO_BOTTOM_BUFFER,        /**< 上から下へ(バッファ) */
  TREE_PATTERN_NUM,
} TreePattern;

class Tree
{
public:
  Tree(uint8_t addr);
  /** 輝度設定(I2C通信有) */
  void config(uint8_t brightness);
  /** パターン設定 */
  int set(TreePattern pattern);
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
  uint8_t m_data[TREE_LED_NUM];
  /** 現在のパターンのインデックス */
  int m_pattern_index;
  /** 現在のパターンのフレームのインデックス */
  int m_frame_index;
};

#endif /* TREE_H */