#include<stdio.h>
/*#include<stdlib.h>
#include<signal.h>
void myhandler(int signo) {
 printf("\nSignal Recieved\n");
 if(signo == SIGSEGV){
   printf("\nhandling Segmentation fault\n");
   printf("\nSignal Number = %d", signo);
}
if(signo == SIGINT){
 printf("\nhandling Segmentation fault\n");
 printf("\nSignal Number = %d", signo);
 }
 exit(1);
 }*/
int main(){
 //signal(SIGSEGV, myhandler);
 //signal(SIGINT, myhandler);
 int x,y,res;
 printf("Enter X");
 scanf("%d", &x);
 printf("Enter Y");
 scanf("%d", &y);

 //raise(SIGINT);
 res = x/y;
 printf("%d", &res);

 return 0;
}
