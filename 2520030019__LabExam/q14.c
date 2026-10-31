#include <stdio.h>
#include <stdlib.h>
struct Student {
    int id;
    float marks;
};
int main(void) {
    int n = 3;
    struct Student *s = malloc(n * sizeof(*s));
    if (s == NULL) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        s[i].id = i + 1;
        s[i].marks = 80.0f + i * 5.0f;
    }
    printf("Student Records:\n");
    for (int i = 0; i < n; i++) {
        printf("ID: %d, Marks: %.2f\n",
               s[i].id, s[i].marks);
    }
    free(s);
    s = NULL;
    printf("All allocated memory released.\n");
    return 0;
}

