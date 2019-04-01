#ifndef TREE_H
#define TREE_H

#define TREE_LED_NUM  (19)

class Tree
{
public:
  Tree(uint8_t addr);
  void config(uint8_t brightness);
  void on(uint8_t index);
  void off(uint8_t index);
  void update();

private:
  uint8_t m_addr;
  uint8_t m_data[TREE_LED_NUM];
};

#endif /* TREE_H */
