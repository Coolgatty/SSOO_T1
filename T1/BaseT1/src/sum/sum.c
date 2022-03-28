#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    float n1 = atof(argv[1]);
    float n2 = atof(argv[2]);
    float n3 = n1 + n2;
    printf("\033[32m %f + %f = %f \n\033[0m", n1, n2, n3);
    return 0;
}