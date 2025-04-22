#include <stdio.h>
#include "cow.h"
#include "util.h"

// Make a cow sound a specified number of times
void cow_sound(int count) {
  make_sound("muuh", count);
}
