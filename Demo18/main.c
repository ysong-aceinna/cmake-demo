#include <stdio.h>
#include <stdlib.h>
#include "math/MathFunctions.h"

int main(int argc, char *argv[])
{
    char f[256] = "a";
    char* data = "1111111";
    imu(f, (unsigned char*)data, 6);

    printf("a+b=%d \n", addition_int(3, 90));
    return 0;
}
