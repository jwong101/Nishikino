#include <utils.h>

//TODO make this able to accept negative exponents
int pow(int num, int exp) {
    if(exp == 0)
        return 1;
    return num * pow(num, exp - 1);
}
