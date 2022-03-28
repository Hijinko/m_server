#include <check.h>
#include <test_list.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static list * p_list = NULL;
const char * word1 = "hello";
const char * word2 = "hey";
const char * word3 = "happy";
const char * word4 = "foo";
const char * word5 = "bar";

char * convert_const(const char * word)
{
    char * p_word = calloc(strlen(word) + 1, sizeof(* p_word));
    if (NULL != p_word)
    {
        strncat(p_word, word, strlen(word));
        return p_word;
    }
    else
    {
        perror("CALLOC");
        return NULL;
    }
}

static void start_list(void)
{
    const char * words[] = {word1, word2, word3, word4};
    size_t size = sizeof(words) / sizeof(char *);
    p_list = list_init(free, list_str_compare);
    for (size_t index = 0; index < size; index++){
        list_append(p_list, convert_const(words[index]));
    }
}

static void teardown_list(void)
{
    list_destroy(p_list);
}

START_TEST(test_list_append)
{
    ck_assert_int_eq(4, p_list->num_nodes);
    ck_assert_str_eq(p_list->p_tail->p_data, word4);
    list_node * p_node = list_append(p_list, convert_const(word5));
    ck_assert(NULL != p_node);
    ck_assert_str_eq((char *)p_list->p_tail->p_data, word5);
    ck_assert_int_eq(5, p_list->num_nodes);
}
END_TEST

START_TEST(test_list_search)
{
    list_node * p_node = list_search(p_list, word3);
    ck_assert_str_eq(word3, (char *)p_node->p_data);
}
END_TEST

START_TEST(test_list_remove)
{
    size_t start_nodes = p_list->num_nodes; 
    list_remove(p_list, word2);
    ck_assert_int_eq((start_nodes - 1), p_list->num_nodes);
}END_TEST

// create suite
Suite * suite_list(void)
{
    Suite * p_suite = suite_create("List");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_list, teardown_list);
    tcase_add_test(p_core, test_list_append);
    tcase_add_test(p_core, test_list_search);
    tcase_add_test(p_core, test_list_remove);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}