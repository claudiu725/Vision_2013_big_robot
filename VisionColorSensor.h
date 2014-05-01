#ifndef VisionColorSensor_h
#define VisionColorSensor_h

#include <Wire.h>
#include <ADJDS311.h>
#include "Arduino.h"

#define MIN3(x,y,z)  ((y) <= (z) ? \
                         ((x) <= (y) ? (x) : (y)) \
                     : \
                         ((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ? \
                         ((x) >= (y) ? (x) : (y)) \
                     : \
                         ((x) >= (z) ? (x) : (z)))

struct rgb_color {
    unsigned char r, g, b;    /* Channel intensities between 0 and 255 */
};

struct hsv_color {
    unsigned char hue;        /* Hue degree between 0 and 255 */
    unsigned char sat;        /* Saturation between 0 (gray) and 255 */
    unsigned char val;        /* Value between 0 (black) and 255 */
};

class VisionColorSensor {
  private:
    rgb_color getRgbColor();
    hsv_color getHsvColor();
    hsv_color rgb_to_hsv(rgb_color rgb);
  public:
    void initPin(int sensorPin);
    boolean isPurple();
    boolean isBlack();
  private:
    ADJDS311 colorSensor;
    int sensorPin;
};

#endif
