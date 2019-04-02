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

  void setLeftToRight1(void);
  void setRightToLeft1(void);
  void setLeftToRight3(void);
  void setRightToLeft3(void);
  void setBothEdgeToMiddle(void);
  
  void update();
  void next();

private:
  uint8_t m_addr;
  uint8_t m_data[GRASS_LED_NUM];

  uint8_t m_state;
  uint8_t m_index;
};

#endif /* GRASS_H */
