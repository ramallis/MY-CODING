#include <stdio.h>

// Print a message multiple times
void make_sound(char sound[], int count) {
  for(int i=0; i<count; i++) {
    printf("%s\n", sound);
  }
}