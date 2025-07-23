#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
	// TODO: implement this

    if (start < 0 || end > len(s)){
        return new_ustr("");
    }

    int32_t codepoints = end - start;

    if (codepoints == 0 || s.contents == NULL){
        return new_ustr("");
    }

    if (s.is_ascii){
        char* content = (char*) malloc(codepoints + 1); // +1 for null terminator
        memcpy(content, s.contents+start, codepoints);
        content[codepoints] = '\0';
        UStr substr = new_ustr(content);
        free(content);
        return substr;
    }
    else{
        int32_t byte_start = 0;
        int32_t byte_end = 0;
        int32_t current_point = 0;
        int32_t temp = 0;
        while (current_point < start && temp < s.bytes){
            if ((s.contents[temp] & 0xC0) != 0x80){
                current_point++;
            }
            temp++;
        }
        byte_start = temp;
        byte_end = byte_start;
        current_point = start;
        for (; current_point < end && byte_end < s.bytes; byte_end++){
            if ((s.contents[byte_end] & 0xC0) != 0x80){
                current_point += 1;
            }
        }

        int32_t num_bytes = byte_end - byte_start;
        char* content = (char*) malloc(num_bytes + 1);
        memcpy(content, s.contents + byte_start, num_bytes);
        content[num_bytes] = '\0';
        printf("Content is %s\n", content);
        UStr substr = new_ustr(content);
        free(content);
        return substr;
    }


}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
	// TODO: implement this
    if (s1.contents == NULL && s2.contents == NULL){
        return new_ustr("");
    }
    else if (s1.contents == NULL){
        return new_ustr(s2.contents);
    }
    else if (s2.contents == NULL){
        return new_ustr(s1.contents);
    }
    else{
        int32_t num_bytes = s1.bytes + s2.bytes;
        char* content = malloc(num_bytes + 1);
        memcpy(content, s1.contents, s1.bytes);
        memcpy(content + s1.bytes, s1.contents, s2.bytes);
        content[num_bytes] = '\0';

        UStr concated = new_ustr(content);
        free(content);
        return concated;
    }
}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	// TODO: implement this

}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this

}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

