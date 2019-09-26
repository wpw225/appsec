/****************************************************************************
 * main.c
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

node* hashtable[HASH_SIZE];
/*
void mem_cleanup(hashmap_t hashtable[], char* misspelled[])
{

        int freed = 0;
        int i;
	node* old_node;

        for (i=0;i<MAX_MISSPELLED;++i) {
		if (misspelled[i] != NULL) {
                	free(misspelled[i]);
//			printf("Freeing %d\n",i);
                	misspelled[i] = NULL;
		}
                ++freed;
        }

	for (i=0;i<HASH_SIZE;++i) {
		if (hashtable[i] != NULL) 
			while (hashtable[i] != NULL) {
				old_node = hashtable[i];
				hashtable[i] = hashtable[i]->next;
				free(old_node);
				old_node=NULL;
			}				
	}

}
*/
int main(int argc,char *argv[]) 
{

	if (argc!=3)
		{
    			printf("Unexpected number of arguments.  Usage: spell_check <file_to_check> <dictionary_file>\n");
    			return -1;
		}
	//printf("arg[1]: %s arg[2] %s",argv[1],argv[2]);

	load_dictionary(argv[2],hashtable);
	FILE* fp = fopen(argv[1], "r"); 

// Define and initialize the misspelled queue

	char* misspelled[MAX_MISSPELLED];
	for (int i=0;i<MAX_MISSPELLED;++i)
		misspelled[i] = NULL;	

	check_words(fp,hashtable,misspelled);
	mem_cleanup(hashtable,misspelled);

}
