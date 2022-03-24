#include <check.h>
#include <test_list.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static list * p_list = NULL;

static void start_list(void)
{
    list_init(p_list, NULL);
}

static void teardown_list(void)
{
    list_destroy(p_list);
}

// create suite
Suite * suite_list(void)
{
    Suite * p_suite = suite_create("List");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_list, teardown_list);
    // tcase_add_test(p_core, test_list_size);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}

