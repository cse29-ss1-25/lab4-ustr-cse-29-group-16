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
        int* codepoint_starts = malloc(len(s) * sizeof(int));
        if (!codepoint_starts) return new_ustr("");
        
        int byte_pos = 0;
        int cp_count = 0;
        
        while (byte_pos < s.bytes && cp_count < len(s)) {
            codepoint_starts[cp_count] = byte_pos;
            
            unsigned char first_byte = s.contents[byte_pos];
            if ((first_byte & 0x80) == 0) {
                byte_pos += 1;
            } else if ((first_byte & 0xE0) == 0xC0) {
                byte_pos += 2;
            } else if ((first_byte & 0xF0) == 0xE0) {
                byte_pos += 3;
            } else if ((first_byte & 0xF8) == 0xF0) {
                byte_pos += 4;
            } else {
                byte_pos += 1;
            }
            cp_count++;
        }
        
        char* result = malloc(s.bytes + 1);
        if (!result) {
            free(codepoint_starts);
            return new_ustr("");
        }
        
        int result_pos = 0;
        
        for (int i = cp_count - 1; i >= 0; i--) {
            int start_byte = codepoint_starts[i];
            int end_byte = (i == cp_count - 1) ? s.bytes : codepoint_starts[i + 1];
            int cp_length = end_byte - start_byte;
            
            memcpy(result + result_pos, s.contents + start_byte, cp_length);
            result_pos += cp_length;
        }
        
        result[result_pos] = '\0';
        free(codepoint_starts);
        
        UStr reversed_str = new_ustr(result);
        free(result);
        return reversed_str;

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
        memcpy(content + s1.bytes, s2.contents, s2.bytes);
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
    if (index < 0 || index >= len(s) || s.contents == NULL){
        return new_ustr(s.contents);
    }


    UStr before = substring(s, 0, index);
    printf("Before is %s\n", before.contents);
    UStr after = substring(s, index + 1, len(s));
    printf("After is %s\n", after.contents);
    UStr result = concat(before, after);
    
    printf("Result is %s\n", result.contents);
    return result;
}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this
    if (s.contents == NULL || len(s) == 0 ){
        return new_ustr("");
    }
    else if (len(s) == 1){
        return new_ustr(s.contents);
    }
    else{
        if (s.is_ascii){
            char* result = malloc(s.bytes + 1);
            for (int32_t i = 0; i < s.bytes; i++){
                result[i] = s.contents[s.bytes - 1 - i];

            }
            result[s.bytes] = '\0';
            UStr reversed_str = new_ustr(result);
            free(result);
            return reversed_str;
        }
        else {
        char* result_buffer = malloc(s.bytes + 1);
        int result_pos = 0;
        
        for (int32_t i = len(s) - 1; i >= 0; i--) {
            UStr char_at_i = substring(s, i, i + 1);
            memcpy(result_buffer + result_pos, char_at_i.contents, char_at_i.bytes);
            result_pos += char_at_i.bytes;
        }
        
        result_buffer[result_pos] = '\0';
        UStr reversed_str = new_ustr(result_buffer);
        free(result_buffer);
        return reversed_str;    
        }
    }
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

