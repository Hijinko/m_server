#include <check.h>
#include <test_list.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static list * p_list = NULL;

static void start_list(void)
{
    p_list = list_init(NULL, NULL);
    list_append(p_list, "hello");
    list_append(p_list, "hey");
    list_append(p_list, "happy");
    list_append(p_list, "foo");
}

static void teardown_list(void)
{
    list_destroy(p_list);
}

START_TEST(test_list_append)
{
    ck_assert_int_eq(4, p_list->num_nodes);
    list_node * p_node = list_append(p_list, "bar");
    ck_assert(NULL != p_node);
    ck_assert_int_eq(5, p_list->num_nodes);
}
END_TEST

START_TEST(test_list_search)
{
    list_node * p_node = list_search(p_list, "happy");
    ck_assert_str_eq("happy", (char *)p_node->p_data);
}
END_TEST

// create suite
Suite * suite_list(void)
{
    Suite * p_suite = suite_create("List");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_list, teardown_list);
    tcase_add_test(p_core, test_list_append);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

