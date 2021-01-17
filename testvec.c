#include <stdio.h>
#include <stdint.h>
#include "defs.h"

typedef struct {
    union {
        unsigned long long vec;
        pixel rgb;
    };
} PIX;

int main() {
    PIX p;

    p.rgb.red = 0x1020;
    p.rgb.green = 0x3040;
    p.rgb.blue = 0x5060;

    printf("%x\n", p.vec);
    printf("%x\n", p.rgb.red);
    printf("%x\n", p.rgb.green);
    printf("%x\n", p.rgb.blue);

    return 0;
}