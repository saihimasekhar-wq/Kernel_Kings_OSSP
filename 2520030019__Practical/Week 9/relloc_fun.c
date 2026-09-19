// Array memory is allocated at run time.
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    int *p;
    int i;

    printf("Enter the size of dynamic memory: ");
    scanf("%d", &n);

    // Allocate memory dynamically
    p = (int *)malloc(n * sizeof(int));

    // Read elements
    for (i = 0; i < n; i++) {
        printf("Enter %d element: ", i + 1);
        scanf("%d", p + i);
    }

    // Display original elements
    for (i = 0; i < n; i++) {
        printf("%d element is: %d\n", i + 1, *(p + i));
    }

    // Increase memory by 3 integers
    p = (int *)realloc(p, (n + 3) * sizeof(int));

    // Add new elements
    p[i++] = 5;
    p[i++] = 10;
    p[i] = 15;

    // Display all elements
    printf("\nAfter reallocating memory:\n");

    for (i = 0; i < n + 3; i++) {
        printf("%d element is: %d\n", i + 1, *(p + i));
    }

    // Release memory
    free(p);

    return 0;
}
