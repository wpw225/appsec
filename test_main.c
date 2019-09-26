#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"
#define TEST_DICT_MAX "wordlist_x26.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));

    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.

	// Since there are four entries in the test_worlist.txt, 
	//assuming no collision, each should be in their own bucket.

	int count=0;
	int i=0;
	for (i=0;i<HASH_SIZE;++i) {
		if (hashtable[i] != NULL) ++count;
	}
	ck_assert_int_eq(count,4);
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));

	// Test here: What if a word begins and ends with "?

	const char* question_mark_word = "?word?";
	ck_assert(check_word(question_mark_word, hashtable));
	const char* punctuations_before_word = ".?!@word";
	ck_assert(check_word(punctuations_before_word, hashtable));
	const char* punctuations_after_word = "word.?!@";
	ck_assert(check_word(punctuations_after_word, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];

// Punctuation removal was implemented in check_word instead of check_words.
// check_words only seperate word using whitespace as delimiter.
// expected 1 & 2 was adjusted to reflect this programming requirement assumption 
// as it was not explicitly stated where this should be implemented in the assignment

    expected[0] = "sogn";
    expected[1] = "skyn,";
    expected[2] = "betta.";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_words_special_chars)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
//    char* expected[3];

// test2.txt have special characters before and after words.  Test program for proper removal of special
// characters prior to dictionary lookup.

    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test2.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
}
END_TEST


START_TEST(test_input_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];

// test3.txt have 3 lines of txt with each line having more than a million characters.
// First line is all a, second line is all b and third line is all c.  Each line is 100,000+ chars long.
// The program should read the first 44 characters including and truncate the rest of the line.

    expected[0] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    expected[1] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    expected[2] = "cccccccccccccccccccccccccccccccccccccccccccc";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test3.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_multiline)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
//    char* expected[3];

// Punctuation removal was implemented in check_word instead of check_words.
// check_words only seperate word using whitespace as delimiter.
// expected 1 & 2 was adjusted to reflect this programming requirement assumption
// as it was not explicitly stated where this should be implemented in the assignment

    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test4.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 12);
}
END_TEST

START_TEST(test_misspelled_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);

// test5.txt have 6496 lines, each line having 3 spelling errors.  Testing to overflow the misspelled array which should stop at 1000 entries.

    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test5.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 1000);
}
END_TEST

START_TEST(test_dictionary_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TEST_DICT_MAX, hashtable));

    // Create a 83.2 million entry wordlist.  Test if system will buffer overflow the hashtable.

        int count=0;
        int i=0;
        for (i=0;i<HASH_SIZE;++i) {
                if (hashtable[i] != NULL) ++count;
        }
        ck_assert(count > 1995);
}
END_TEST

START_TEST(test_check_numbers)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
//    char* expected[3];

// test6.txt have numbers within the sentence.  Test program for proper handling
// of numbers in dictionary lookup.

    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test6.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
}
END_TEST



Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);

// Additional test cases added for assignment

	tcase_add_test(check_word_case, test_dictionary_normal);
	tcase_add_test(check_word_case, test_input_buffer_overflow);
	tcase_add_test(check_word_case, test_check_multiline);
	tcase_add_test(check_word_case, test_misspelled_buffer_overflow);
	tcase_add_test(check_word_case, test_check_words_special_chars);
	tcase_add_test(check_word_case, test_dictionary_buffer_overflow);
	tcase_add_test(check_word_case, test_check_numbers);

    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

