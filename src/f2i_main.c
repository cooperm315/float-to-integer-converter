#include <stdlib.h>
#include <inttypes.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
int32_t f2i(uint32_t in);

int main()
{
    // tests a valid input
    int32_t act_val = 0x0aaaaaa0;
    uint32_t start_val = 0x4d2aaaaa;
    int32_t output_val = f2i(start_val);
    if (act_val != output_val)
    {
        fprintf(stderr, "Incorrect value of %i, expected %i\n", output_val, act_val);
        exit(-1);
    }
    // tests a value that is NaN
    uint32_t nan = 0xfff00000;
    int32_t return_val = 0x80000000;
    int32_t output = f2i(nan);
    if (return_val != output)
    {
        fprintf(stderr, "Incorrect return value for Nan %i, expected -2147483658\n", output);
        exit(-1);
    }

    // for (uint32_t i = 0x43ff0000; i <= 0x440cdd00; i++)
    // {
    //     int32_t output = f2i(i);
    //     int32_t expected;
    //     if (expected != output)
    //     {
    //         fprintf(stderr, "Incorrect value of %i, expected %i\n", output, expected);
    //         exit(-1);
    //     }
    // }
    printf("Everything worked!\n");
    return 0;
}