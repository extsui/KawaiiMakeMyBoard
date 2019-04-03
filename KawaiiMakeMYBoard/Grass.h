#ifndef GRASS_H
#define GRASS_H

#define GRASS_LED_NUM   (11)

class Grass
{
public:
  Grass(uint8_t addr);
  void config(uint8_t brightness);
  int set(int pattern);
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
  //uint8_t m_frame_count;
  // 現在のパターンのフレームのインデックス
  uint8_t m_frame_index;
};

#endif /* GRASS_H */
