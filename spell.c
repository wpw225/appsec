/****************************************************************************
 * spell.c
 *
 * Application Security, Assignment 1
 *
 * Student: Sun Ting Wong 
 * email: wpw225@nyu.edu
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#ifndef HAVE_STRLCPY
/*
 * '_cups_strlcpy()' - Safely copy two strings.
 * As per suggested by Professor on 9/21 Office Hour zoom session, strlcpy is better than strncpy.
 * This section is added to provide for strlcpy since it's not part of standard library..
 * Section of strlcpy code from 
 * https://stackoverflow.com/questions/18547251/when-i-use-strlcpy-function-in-c-the-compilor-give-me-an-error
 */

size_t                  	/* O - Length of string */
strlcpy(char       *dst,        /* O - Destination string */
        const char *src,      	/* I - Source string */
        size_t      size)     	/* I - Size of destination string buffer */
{
  	size_t    srclen;       /* Length of source string */


// Figure out how much room is needed...

  	size --;

  	srclen = strlen(src);

// Copy the appropriate amount...

  	if (srclen > size)
    		srclen = size;

  	memcpy(dst, src, srclen);
  	dst[srclen] = '\0';

  	return (srclen);
}
#endif /* !HAVE_STRLCPY */

// Maps a word to an integer value to place it in the hash table.
// Sum the value of each character in the word, then find the 
// remainder after dividing by the size of the hash table.

int get_bucket_num(const char* word) {

// Convert word to lower case for hash function use

              char lcase_word[LENGTH];
              strlcpy(lcase_word,word,LENGTH);
              int c = 0;
              while (lcase_word[c] != '\0') {
                      if (lcase_word[c] >= 'A' && lcase_word[c] <= 'Z') {
                              lcase_word[c] = lcase_word[c] + 32;
                      }
                      c++;
              }

// Get node hash value (bucket number) of lower case word

                return(hash_function(lcase_word));

}

bool check_word(const char* word, hashmap_t hashtable[])
{

// Set pointer to bucket_list where dictionary word might be stored

	node * bucket_list = hashtable[get_bucket_num(word)];

	while(bucket_list != NULL) {
		if (strcmp(bucket_list->word,word) == 0) {
//			printf("True\n");
			return true;
		}
		bucket_list = bucket_list->next;
	}

// If the word does not match any dictionary entries in hashtable, reture false.

//	printf("False\n");
	return false;
}



bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{

// initialize all values in hash table to NULL.

//	printf("Initialize all values in hash table size %d to NULL\n",HASH_SIZE);
	int i;
	for(i=0;i<HASH_SIZE;i++)
		hashtable[i] = NULL;
//	printf("Dictionary file path is at %s\n",dictionary_file);

// Open dict_file from path stored in dictionary.

    	FILE *fptr = fopen(dictionary_file, "r");

// If dict_file is NULL return false.

    	if (fptr == NULL)
    	{
//       	printf("Cannot open dictionary file \n");
        	return(0);
    	}

	char dic_word[LENGTH];

//  Read word in dict_file until EOF (end of file):

	while (fgets(dic_word, LENGTH, fptr) != NULL) {

// Remove newline character from string

    		int word_length = strlen(dic_word) -1;
    		if (dic_word[word_length] == '\n')
        		dic_word[word_length] = '\0';

// Create new_node pointer, allocate memory for node and assign dictionary word to node

		node *new_node = (node *) malloc(sizeof(node));
		
//Check if memory has been successfully allocated
		
		if (new_node == NULL) {
			printf("Memory allocation error.\n");
			exit(0);
		}

		strlcpy(new_node->word, dic_word,LENGTH);
		new_node->next = NULL;

// Insert new_node into hashmap

		int bucket = get_bucket_num(dic_word);
//		printf("%s %s %d\n",dic_word,new_node->word,bucket);
		if (hashtable[bucket] == NULL)
			hashtable[bucket] = new_node;			
		else {
			new_node->next=hashtable[bucket];
			hashtable[bucket]=new_node;
		}
	}

// Close dict_file.

	fclose(fptr);
	return(1);
} 

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{

	int num_misspelled=0;
	char line[LENGTH*10];
	char *ptr;
	char delim[] = " \n";
	char c;

        while (fgets(line, LENGTH*10, fp) != NULL) {
		ptr = strtok(line,delim);

// Prevent overrunning MAX_MISSPELLED array buffer

                if (num_misspelled >= MAX_MISSPELLED)
               		 break;

// Iterate though each word in the line and check spelling 

		while(ptr != NULL)
		{

                       char *new_word = (char *) malloc(sizeof(char)*LENGTH);
                       if (new_word == NULL) {
                                //printf("Memory allocation error.\n");
                                exit(0);
                        }

			strlcpy(new_word,ptr,LENGTH);
			//printf("new_word %s %ld\n",new_word,strlen(new_word));

// Remove the rest of very long word from input if the portion read in already is at LENGTH limit

			if (strlen(new_word) >= LENGTH - 1) {
				//printf("Trimming ...\n");
				while ( true ) {
     					c = fgetc ( fp ) ; // reading the file
     					if ( isspace(c) )
     						break ;
				}
			}

// Remove trailing punctuation.

			char *end = new_word + strlen(new_word) - 1;
			while(end > new_word && ispunct((unsigned char)*end)) end--;
// Write new null terminator character
			end[1] = '\0';

// Remove leading punctuation.

			int index=0;
			while( ispunct(new_word[index]) ) {
				index++;
			}
			if (index != 0) {
				int i=0;
				while(new_word[i + index] != '\0') {
					new_word[i] = new_word[i + index];
					i++;
				}	
				new_word[i] = '\0';
			}
			//printf("new_word %s\n",new_word);
				
// Convert first character of word to lower case
			new_word[0] = tolower(new_word[0]);

// Check if word is in hashtable
			if (!check_word(new_word,hashtable)) {

// Insert misspelled word into array

				misspelled[num_misspelled] = new_word;
				++num_misspelled;

// Prevent overrunning MAX_MISSPELLED array buffer
				if (num_misspelled >= MAX_MISSPELLED)
					break;
			} else {
// Free memory if word is not added to misspelled list and assign new_word back to NULL to avoid Double Free Attack
				free(new_word);
				new_word = NULL;
			}

			//printf("'%s'\n", ptr);
			ptr = strtok(NULL,delim);
		}
	}
//	printf("num_misspelled: %d",num_misspelled);
	return num_misspelled;
}

/*
node* hashtable[HASH_SIZE];

int main(void) 
{
load_dictionary("wordlist.txt",hashtable);
FILE *fp = fopen("test3.txt", "r"); 
char * misspelled[1000];
check_words(fp,hashtable,misspelled);

}

*/
