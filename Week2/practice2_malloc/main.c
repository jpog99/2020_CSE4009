#include <stdio.h>
#include <stdlib.h>

int main(void){
	char** pp_data = NULL;
	int n;
	int length;
	int i;

	printf("Enter word count: ");
	scanf("%d", &n);

	pp_data = (char**)malloc(sizeof(char*) * n);
	printf("Enter max length: ");
    scanf("%d",&length);
    for(i=0 ; i<n ; i++){
        pp_data[i] = (char*)malloc(sizeof(char) * length);
        printf("Enter word: ");
        scanf("%s",pp_data[i]);
    }


	for(i=0 ; i<n ; i++){
        printf("%s\n",pp_data[i]);
    }


	for(i=0 ; i<n ; i++){
        free(pp_data[i]);
    }

    free(pp_data);

	return 0;
}
