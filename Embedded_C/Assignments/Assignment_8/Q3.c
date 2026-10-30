//Study about various functions which can convert from string to different types atoi, atol, atoll, atof, strtoimax, strtol, strtoul, strtof, strtod etc

#include <stdio.h>
#include <stdlib.h>   // for atoi, atol, atoll, atof, strtol, strtoul, strtof, strtod
#include <string.h>
#include <inttypes.h> // for strtoimax

int main() {
    char str[] = "2026Hello";
    char *endptr;

    printf("atoi: %d\n", atoi(str));
    printf("atol: %ld\n", atol(str));
    printf("atoll: %lld\n", atoll(str));
    printf("atof: %f\n", atof(str));

    // strtoimax needs endptr and base
    intmax_t val_imax = strtoimax(str, &endptr, 10);
    printf("strtoimax: %jd\n", val_imax);

    long val_l = strtol(str, &endptr, 10);
    printf("strtol: %ld\n", val_l);

    unsigned long val_ul = strtoul(str, &endptr, 10);
    printf("strtoul: %lu\n", val_ul);

    float val_f = strtof(str, &endptr);
    printf("strtof: %f\n", val_f);

    double val_d = strtod(str, &endptr);
    printf("strtod: %f\n", val_d);

    return 0;
}//main

