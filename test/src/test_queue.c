#include <check.h>
#include <test_queue.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static queue * p_queue = NULL;

static void start_queue(void)
{
    p_queue = queue_init(queue_compare_ints);
    int nums[] = {10, 20, 30};
    size_t size = sizeof(nums) / sizeof(int);
    for (size_t index = 0; index < size; index++){
        queue_enqueu(p_queue, &nums[index]);
    }
}

static void teardown_queue(void)
{
    queue_destroy(p_queue);
}

START_TEST(test_queue_enqueu)
{
    int num = 40;
    queue_enqueu(p_queue, &num);
    ck_assert_int_eq(4, p_queue->p_stack->num_nodes);
    ck_assert_int_eq(40, *(int *)(p_queue->p_stack->p_tail->p_data));
}
END_TEST

START_TEST(test_queue_dequeue)
{
    int num = *(int *)queue_dequeue(p_queue);
    ck_assert_int_eq(10, num);
}
END_TEST

// create suite
Suite * suite_queue(void)
{
    Suite * p_suite = suite_create("Queue");
    TCase * p_core = tcase_create("Core");
    // add test cases 
    tcase_add_checked_fixture(p_core, start_queue, teardown_queue);
    tcase_add_test(p_core, test_queue_enqueu);
    tcase_add_test(p_core, test_queue_dequeue);
    // add core to suite
    suite_add_tcase(p_suite, p_core);
    return p_suite;
}