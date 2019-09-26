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

int main(int argc,char *argv[]) 
{

	if (argc!=3)
		{
    			printf("Unexpected number of arguments.  Usage: spell_check <file_to_check> <dictionary_file>\n");
    			return -1;
		}
	//printf("arg[1]: %s arg[2] %s",argv[1],argv[2]);

	load_dictionary(argv[2],hashtable);
	FILE *fp = fopen(argv[1], "r"); 
	char * misspelled[1000];
	check_words(fp,hashtable,misspelled);

}
