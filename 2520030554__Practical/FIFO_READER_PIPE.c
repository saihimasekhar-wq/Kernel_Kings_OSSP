# include<stdio.h>
# include<string.h>
# include<fcntl.h>
# include<sys/stat.h>
# include<sys/types.h>
# include<unistd.h>
int main() {
	char bfr[100];
	printf("receiving message from writer\n");
	int fd = open("/temp/fifo_pipe", O_RDONLY);
	read(fd, bfr, sizeof(bfr));
	printf("received message:%s", bfr);
	close(fd); 
	return 0;
}
