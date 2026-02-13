#include <Arduino.h>
#include <touchArea.h>


/***************************************************************************************
** Function name:           class constructor
** Description:             
***************************************************************************************/
touchArea::touchArea(void) {
  clearAllItems();
}

/***************************************************************************************
** Function name:           clear
** Description:             claer all defined touch area from list
***************************************************************************************/
void touchArea::clearAllItems(void) {
  _count = 0;
  _onLostFocusCallback = NULL;
}

/***************************************************************************************
** Function name:           getCount
** Description:             return number of defined touch area
***************************************************************************************/
uint8_t touchArea::getItemCount(void) const {
  return _count;
}

/***************************************************************************************
** Function name:           getItem
** Description:             return coordinates for selected item - just for debugging
***************************************************************************************/
void touchArea::getItem(uint8_t index, uint16_t *xmin, uint16_t *ymin, uint16_t *xmax, uint16_t *ymax) {
  *xmin = _touchArea[index].x_min;
  *xmax = _touchArea[index].x_max;
  *ymin = _touchArea[index].y_min;
  *ymax = _touchArea[index].y_max;
}

/***************************************************************************************
** Function name:           addItem
** Description:             add a touch area to list with clicked and released function
***************************************************************************************/
bool touchArea::addItem(uint16_t x_min, uint16_t y_min, uint16_t x_max, uint16_t y_max, uint8_t tag, void (*isClickedFunc)(uint8_t, uint16_t, uint16_t), void (*isReleasedFunc)(uint8_t, uint16_t, uint16_t)=NULL) {
  // check for overrun
  if (_count == MAX_TOUCH_AREA_COUNT - 1) return(false);
  
  _touchArea[_count].x_min = x_min;
  _touchArea[_count].y_min = y_min;
  _touchArea[_count].x_max = x_max;
  _touchArea[_count].y_max = y_max;
  _touchArea[_count].tag = tag;
  _touchArea[_count].isClickedCallback = isClickedFunc;
  _touchArea[_count].isReleasedCallback = isReleasedFunc;
  _touchArea[_count].isDown = false;

  _count++;
  return(true);
}

/***************************************************************************************
** Function name:           addItem
** Description:             add a touch area to list with clicked function only
***************************************************************************************/
bool touchArea::addItem(uint16_t x_min, uint16_t y_min, uint16_t x_max, uint16_t y_max, uint8_t tag, void (*isClickedFunc)(uint8_t, uint16_t, uint16_t)) {
  return(addItem(x_min, y_min, x_max, y_max, tag, isClickedFunc, NULL));
}


/***************************************************************************************
** Function name:           addItem
** Description:             add a touch area to list with clicked function only
***************************************************************************************/
void getActionPostion(uint16_t *x, uint16_t *y) {
  *x = _lastX;
  *y = _lastY;
}

/***************************************************************************************
** Function name:           setLostFocusCallback
** Description:             set callback function which will be called if area focus is lost
***************************************************************************************/
void touchArea::setLostFocusCallback(void (*onLostFocusFunc)(void)) {
  _onLostFocusCallback = onLostFocusFunc;
}

/***************************************************************************************
** Function name:           checkEvent
** Description:             check if touched pos is within a defined touch area
**                          if yes call function (if not NULL)
***************************************************************************************/
bool touchArea::checkEvent(uint16_t x, uint16_t y, bool release = false) {
  uint8_t loop;

    // Serial.println("----- Touch Check ------");
    // Serial.printf("X: %d, Y: %d\n", x, y);
    // for (loop=0; loop<_count; loop++) {
    //   Serial.printf(" [%2d]: Xmin: %3d, Ymin: %3d, Xmax: %3d, Ymax: %3d\n", loop, _touchArea[loop].x_min, _touchArea[loop].y_min, _touchArea[loop].x_max, _touchArea[loop].y_max);
    // }

  _lastX = x;
  _lastY = y;

  if (release == false) {
    for (loop=0; loop<_count; loop++) {
      // check coordinates
      if (x >= _touchArea[loop].x_min && x <= _touchArea[loop].x_max && y >= _touchArea[loop].y_min && y <= _touchArea[loop].y_max) {
        _touchArea[loop].isDown = true;
        if (_touchArea[loop].isClickedCallback != NULL) {
          // call function
          _touchArea[loop].isClickedCallback(_touchArea[loop].tag, x, y);
          return(true);
        }
      }
    }

    // if no touchArea is identified call onLostFocusCallback if not NULL
    if (_onLostFocusCallback != NULL) _onLostFocusCallback();
  }
  else {
    for (loop=0; loop<_count; loop++) {
      // check coordinates
      if (x >= _touchArea[loop].x_min && x <= _touchArea[loop].x_max && y >= _touchArea[loop].y_min && y <= _touchArea[loop].y_max) {
        if (_touchArea[loop].isDown == true){
          _touchArea[loop].isDown = false;
          if (_touchArea[loop].isReleasedCallback != NULL) {
            // call function
            _touchArea[loop].isReleasedCallback(_touchArea[loop].tag, x, y);
            return(true);
          }
        }
      }
    }
  }

  return(false);
}

/***************************************************************************************
** Function name:           isClicked
** Description:             check defined touch areas and call isClicked function if found
***************************************************************************************/
bool touchArea::isClicked(uint16_t x, uint16_t y) {
  // Serial.print("Clicked X,Y: "); Serial.print(x); Serial.print(","); Serial.println(y);
  return(checkEvent(x, y, false));
}

/***************************************************************************************
** Function name:           isReleased
** Description:             call isReleased function if released above defined touch area
***************************************************************************************/
bool touchArea::isReleased(uint16_t x, uint16_t y) {
  // Serial.print("Released X,Y: "); Serial.print(x); Serial.print(","); Serial.println(y);
  return(checkEvent(x, y, true));
}
