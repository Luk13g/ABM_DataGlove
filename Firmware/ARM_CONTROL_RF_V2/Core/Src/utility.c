#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "stm32f1xx_hal.h"
#include "main.h"

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
