#ifndef GRASS_H
#define GRASS_H

#define GRASS_LED_NUM   (11)

class Grass
{
public:
  Grass(uint8_t addr);
  void config(uint8_t brightness);
  void on(uint8_t index);
  void off(uint8_t index);
  void update();

private:
  uint8_t m_addr;
  uint8_t m_data[GRASS_LED_NUM];
};

#endif /* GRASS_H */
