/*
  touchArea.h - Library for Touch LCD - touch area definitions
  Created by Jochen Kober, December 2021.
  Released into the public domain.
*/

#ifndef _TOUCH_AREA_
#define _TOUCH_AREA_


#define MAX_TOUCH_AREA_COUNT    32



typedef struct {
  uint16_t x_min;
  uint16_t x_max;
  uint16_t y_min;
  uint16_t y_max;
  void (*isClickedCallback)(uint8_t);
  void (*isReleasedCallback)(uint8_t);
  uint8_t tag;
  bool isDown;
} touchArea_t;



class touchArea
{
  public:
    touchArea(void);

    void clearAllItems(void);
    uint8_t getItemCount(void) const;
    bool  addItem(uint16_t x_min, uint16_t y_min, uint16_t x_max, uint16_t y_max, uint8_t tag, void (*isClickedFunc)(uint8_t), void (*isReleasedFunc)(uint8_t)),
          addItem(uint16_t x_min, uint16_t y_min, uint16_t x_max, uint16_t y_max, uint8_t tag, void (*isClickedFunc)(uint8_t));
    void setLostFocusCallback(void (*onLostFocusFunc)(void));
    bool isClicked(uint16_t x, uint16_t y);
    bool isReleased(uint16_t x, uint16_t y);

    // function can be used for debugging
    void getItem(uint8_t index, uint16_t *xmin, uint16_t *ymin, uint16_t *xmax, uint16_t *ymax);
    
  private:

    bool checkEvent(uint16_t x, uint16_t y, bool release);

    touchArea_t _touchArea[MAX_TOUCH_AREA_COUNT];
    uint8_t _count = 0;
    void (*_onLostFocusCallback)(void);

  protected:

};

#endif