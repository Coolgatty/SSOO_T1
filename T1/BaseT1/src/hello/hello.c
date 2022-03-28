#include <stdio.h>

#include <unistd.h>


int main(int argc, char const *argv[])
{
    sleep(5);
    printf("\033[32mHello World!\n\033[0m");
    return 0;
}