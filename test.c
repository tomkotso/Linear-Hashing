#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){


    char lk[10];
    char test[25];
    int a;

    while(1){

        scanf("%s",lk);

        if ( strcmp(lk,"thanasis") == 0 ){

            printf("mpravo malaka\n");
            printf("%d",scanf("%s %d",test,&a));
            //printf("%s %s %d",lk,test,a);

        }else if ( strcmp(lk,"thanos") == 0 ){
            printf("ksana mpravo malaka\n");
            scanf("%d",&a);
            //printf("%s %d",lk,a);

        }






    }


    return 0;


}