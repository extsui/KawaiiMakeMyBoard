#ifndef LAND_H
#define LAND_H

#define LAND_LED_NUM   (25)

class Land
{
public:
  Land(uint8_t addr);
  void config(uint8_t brightness);
  void on(uint8_t index);
  void off(uint8_t index);
  void update();

private:
  uint8_t m_addr;
  uint8_t m_data[LAND_LED_NUM];
};

#endif /* LAND_H */
