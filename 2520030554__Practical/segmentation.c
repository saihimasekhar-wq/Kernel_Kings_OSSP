#include <stdio.h>

int main() {
    int *ptr = NULL;
    int x = 25;

    ptr = &x;

    printf("x = %d\n", x);
    printf("address of x = %p\n", (void *)&x);
    printf("value of ptr = %p\n", (void *)ptr);
    printf("value pointed by ptr = %d\n", *ptr);

    return 0;
}
