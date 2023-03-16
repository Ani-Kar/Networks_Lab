#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	FILE* ptr;
	char ch = 'a';
	ptr = fopen("large_input.txt", "r");
	if (NULL == ptr) {
		printf("file can't be opened \n");
	}
    char input[] = "hello ";
	while(ch != EOF){
        strncat(input, &ch, 1);
		ch = fgetc(ptr);
    }

	fclose(ptr);
	return 0;
}
