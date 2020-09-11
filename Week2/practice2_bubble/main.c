//Template
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int compare(int a, int b){
    if(a>b)
    	return 1;
    else
       	return -1;
}


/***************************************************************
define bubble sort that uses pointer to 'compare' function above
***************************************************************/


int main(){
    int n_array[SIZE] = {2,-9, 10, 15, 1, 3, -12, 5, 4, 1};

    int i,j,swapped=1;
    int* ptr = n_array;
    while(swapped){
        swapped=0;
        for(i=0 ; i<SIZE ; i++){
            int result = compare(*(ptr+i),*(ptr+i+1));
            if(result == 1){
                int temp = *(ptr+i);
                *(ptr+i) = *(ptr+i+1);
                *(ptr+i+1) = temp;
                swapped=1;
            }
        }
    }

    for(i=0 ; i<SIZE ; i++){
        printf("%d, " ,n_array[i]);
    }

    return 0;
}
