#include <stdio.h>

int main() {
	int *ptr = NULL;
	int x;
	ptr = &x;
	printf("x = %d", x);
	printf("address of x = %ls", &x);
	printf("value of ptr = %ls", ptr);
	printf("value of ptr = %d", *ptr);
	printf("address of ptr = %ls", &ptr);
	return 0;
}
	
