/**
 * @file atomic_test.c
 * Unit testing of atomic values
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <atomic_repr.h>
#include <error.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))

void test_creation(PICC_Error *error)
{
    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(false, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_bool_get(abool) == false);

    PICC_AtomicInt *aint = PICC_create_atomic_int(1, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_int_get(aint) == 1);

    PICC_free_atomic_bool(abool);
    PICC_free_atomic_int(aint);
}

void test_compare_and_swap(PICC_Error *error)
{
    bool bret;
    int iret;

    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(false, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_bool_get(abool) == false);

    bret = PICC_atomic_bool_compare_and_swap(abool, false, true);
    ASSERT(bret == false);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    bret = PICC_atomic_bool_compare_and_swap(abool, false, false);
    ASSERT(bret == true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    bret = PICC_atomic_bool_compare_and_swap(abool, true, true);
    ASSERT(bret == true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    PICC_AtomicInt *aint = PICC_create_atomic_int(0, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_int_get(aint) == 0);

    iret = PICC_atomic_int_compare_and_swap(aint, 0, 2);
    ASSERT(iret == 0);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    iret = PICC_atomic_int_compare_and_swap(aint, 1, 3);
    ASSERT(iret == 2);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    iret = PICC_atomic_int_compare_and_swap(aint, 2, 2);
    ASSERT(iret == 2);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    PICC_free_atomic_bool(abool);
    PICC_free_atomic_int(aint);
}

void test_compare_and_swap_check(PICC_Error *error)
{
    bool ret;

    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(false, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_bool_get(abool) == false);

    ret = PICC_atomic_bool_compare_and_swap_check(abool, false, true);
    ASSERT(ret == true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    ret = PICC_atomic_bool_compare_and_swap_check(abool, false, false);
    ASSERT(ret == false);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    ret = PICC_atomic_bool_compare_and_swap_check(abool, true, true);
    ASSERT(ret == true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    PICC_AtomicInt *aint = PICC_create_atomic_int(0, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_int_get(aint) == 0);

    ret = PICC_atomic_int_compare_and_swap_check(aint, 0, 2);
    ASSERT(ret == true);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    ret = PICC_atomic_int_compare_and_swap_check(aint, 1, 3);
    ASSERT(ret == false);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    ret = PICC_atomic_int_compare_and_swap_check(aint, 2, 2);
    ASSERT(ret == true);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    PICC_free_atomic_bool(abool);
    PICC_free_atomic_int(aint);
}


void test_get_and_set(PICC_Error *error)
{
    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(false, error);
    ASSERT_NO_ERROR();
    PICC_AtomicInt *aint = PICC_create_atomic_int(0, error);
    ASSERT_NO_ERROR();

    ASSERT(PICC_atomic_bool_get_and_set(abool, true) == false);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    ASSERT(PICC_atomic_int_get_and_set(aint, 3) == 0);
    ASSERT(PICC_atomic_int_get(aint) == 3);
}

void test_increment_and_decrement(PICC_Error *error)
{
    PICC_AtomicInt *aint = PICC_create_atomic_int(0, error);
    ASSERT_NO_ERROR();

    ASSERT(PICC_atomic_int_get_and_increment(aint) == 0);
    ASSERT(PICC_atomic_int_get(aint) == 1);
    ASSERT(PICC_atomic_int_get_and_increment(aint) == 1);
    ASSERT(PICC_atomic_int_get(aint) == 2);
    ASSERT(PICC_atomic_int_get_and_decrement(aint) == 2);
    ASSERT(PICC_atomic_int_get(aint) == 1);
    ASSERT(PICC_atomic_int_get_and_decrement(aint) == 1);
    ASSERT(PICC_atomic_int_get(aint) == 0);
}

/**
 * Runs all atomic values tests.
 */
void PICC_test_atomic()
{
    ALLOC_ERROR(error);
    test_creation(&error);
    test_compare_and_swap(&error);
    test_compare_and_swap_check(&error);
    test_get_and_set(&error);
    test_increment_and_decrement(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

