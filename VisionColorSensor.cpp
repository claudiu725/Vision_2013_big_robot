#include "VisionColorSensor.h"

rgb_color VisionColorSensor::getRgbColor()
{
  RGBC color = colorSensor.read();
  rgb_color rgb;
  rgb.r = map(color.red, 0, 1023, 0, 255);
  rgb.g = map(color.green, 0, 1023, 0, 255);
  rgb.b = map(color.blue, 0, 1023, 0, 255);
  return rgb;
}

hsv_color VisionColorSensor::getHsvColor()
{
  return rgb_to_hsv(getRgbColor());
}

hsv_color VisionColorSensor::rgb_to_hsv(rgb_color rgb) {
    hsv_color hsv;
    unsigned char rgb_min, rgb_max;
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    hsv.val = rgb_max;
    if (hsv.val == 0) {
        hsv.hue = hsv.sat = 0;
        return hsv;
    }
    hsv.sat = 255*long(rgb_max - rgb_min)/hsv.val;
    if (hsv.sat == 0) {
        hsv.hue = 0;
        return hsv;
    }
    /* Compute hue */
    if (rgb_max == rgb.r) {
        hsv.hue = 0 + 43*(rgb.g - rgb.b)/(rgb_max - rgb_min);
    } else if (rgb_max == rgb.g) {
        hsv.hue = 85 + 43*(rgb.b - rgb.r)/(rgb_max - rgb_min);
    } else /* rgb_max == rgb.b */ {
        hsv.hue = 171 + 43*(rgb.r - rgb.g)/(rgb_max - rgb_min);
    }
    return hsv;
}

void VisionColorSensor::initPin(int sensorPin)
{
  colorSensor.initPin(sensorPin);
  colorSensor.init();
  colorSensor.ledOn();
  colorSensor.customCalibrate(152,53,1,0,0);
}

boolean VisionColorSensor::isPurple()
{
  hsv_color hsv = getHsvColor();
  return true;
}

boolean VisionColorSensor::isBlack()
{
  hsv_color hsv = getHsvColor();
  return true;
}
