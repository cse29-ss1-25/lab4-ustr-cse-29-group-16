#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main(){
	char input[256];
	int index = 1;
	while(fgets(input, sizeof(input), stdin)){
		//remove newline characters and replace with terminator
		size_t l = strlen(input);
		if (l > 0 && input[l - 1] == '\n') {
			input[l - 1] = '\0';
		}

		//create list of strings
		 UStr s1 = new_ustr("hello");
		 UStr s2 = new_ustr("this is");
		 UStr s3 = new_ustr("cse29🐕");
		 UStr arr[3] = {s1, s2, s3};
		 UStr* ptr = &arr[0];
		 List lst = new_list_from_array(ptr, 3);
		
	       UStr ustrInput = new_ustr(input);
      		
		insert(lst, ustrInput, index);

		print_ustr(result);
		printf("\n");

		free_ustr(s1);
		free_ustr(s2);
		free_ustr(s3);
		free_ustr(lst.data);
		free_ustr(ustrInput);
	}
	return 0;
}
