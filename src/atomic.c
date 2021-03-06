/**
 * @file atomic.c
 * Atomic booleans and integers.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <atomic_repr.h>
#include <tools.h>

#define LOCK_ATOMIC_VALUE(val) \
    pthread_mutex_lock(&(val->lock));

#define RELEASE_ATOMIC_VALUE(val) \
    pthread_mutex_unlock(&(val->lock));


// Atomic booleans /////////////////////////////////////////////////////////////

/**
 * Creates a new atomic boolean.
 *
 * @param value Initial value
 * @param error Error stack
 * @return Created atomic boolean
 */
PICC_AtomicBoolean *PICC_create_atomic_bool(bool value, PICC_Error *error)
{
    PICC_ALLOC(atomic_bool, PICC_AtomicBoolean, error) {
        atomic_bool->val = value;
        pthread_mutex_init(&(atomic_bool->lock), NULL);
    }
    return atomic_bool;
}

/**
 * Frees an atomic boolean.
 *
 * @pre atomic_bool != null
 * @param atomic_bool Atomic boolean to be freed
 */
void PICC_free_atomic_bool(PICC_AtomicBoolean *atomic_bool)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    free(atomic_bool);
}

/**
 * Atomically sets the value to the given updated value if the current value
 * is equal to the expected value.
 *
 * @pre atomic_bool != null
 * @post if (atomic_bool.val@pre == expected_val) then atomic_bool.val = new_val
 * @param atomic_bool Atomic boolean to be set
 * @param expected_val Expected current value
 * @param new_val New value of the boolean
 * @return Value of the boolean before the operation
 */
bool PICC_atomic_bool_compare_and_swap(PICC_AtomicBoolean *atomic_bool, bool expected_val, bool new_val)
{
    LOCK_ATOMIC_VALUE(atomic_bool);

    #ifdef CONTRACT_PRE
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        bool val_at_pre = atomic_bool->val;
    #endif

    bool old_val = atomic_bool->val;
    if (old_val == expected_val)
        atomic_bool->val = new_val;

    #ifdef CONTRACT_POST
        // post: if (atomic_bool.val@pre == expected_val) then atomic_bool.val = new_val
        if (val_at_pre == expected_val)
            ASSERT(atomic_bool->val == new_val);
    #endif

    RELEASE_ATOMIC_VALUE(atomic_bool);
    return old_val;
}

/**
 * Atomically sets the value to the given updated value if the current value
 * is equal to the expected value and returns whether the operation has been
 * done.
 *
 * @pre atomic_bool != null
 * @post if (atomic_bool.val@pre == expected_val) then atomic_bool.val = new_val
 * @param atomic_bool Atomic boolean to be set
 * @param expected_val Expected current value
 * @param new_val New value of the boolean
 * @return Whether the operation has been done
 */
bool PICC_atomic_bool_compare_and_swap_check(PICC_AtomicBoolean *atomic_bool, bool expected_val, bool new_val)
{
    LOCK_ATOMIC_VALUE(atomic_bool);

    #ifdef CONTRACT_PRE
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        bool val_at_pre = atomic_bool->val;
    #endif

    bool old_val = atomic_bool->val;
    bool success = false;
    if (old_val == expected_val) {
        atomic_bool->val = new_val;
        success = true;
    }

    #ifdef CONTRACT_POST
        // post: if (atomic_bool.val@pre == expected_val) then atomic_bool.val = new_val
        if (val_at_pre == expected_val)
            ASSERT(atomic_bool->val == new_val);
    #endif

    RELEASE_ATOMIC_VALUE(atomic_bool);
    return success;
}

/**
 * Unconditionally gets the current value of the atomic boolean value
 *
 * @pre atomic_bool != null
 * @param atomic_bool Atomic boolean to get
 * @return Current value of the atomic boolean
 */
bool PICC_atomic_bool_get(PICC_AtomicBoolean *atomic_bool)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    return atomic_bool->val;
}

/**
 * Atomically sets the atomic boolean value.
 *
 * @pre atomic_bool != null
 * @post atomic_bool.val == new_val
 * @param atomic_bool Atomic boolean to be set
 * @param new_val The New value
 * @return The previous value
 */
bool PICC_atomic_bool_get_and_set(PICC_AtomicBoolean *atomic_bool, bool new_val)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    bool current;
    for (;;) {
        current = PICC_atomic_bool_get(atomic_bool);
        if (PICC_atomic_bool_compare_and_swap_check(atomic_bool, current, new_val))
            break;
    }

    #ifdef CONTRACT_POST
        // post: atomic_bool.val == new_val
        ASSERT(atomic_bool->val == new_val);
    #endif

    return current;
}


// Atomic integers /////////////////////////////////////////////////////////////

/**
 * Creates a new atomic integer.
 *
 * @param value Initial value
 * @param error Error stack
 * @return Created atomic integer
 */
PICC_AtomicInt *PICC_create_atomic_int(int value, PICC_Error *error)
{
    PICC_ALLOC(atomic_int, PICC_AtomicInt, error) {
        atomic_int->val = value;
        pthread_mutex_init(&(atomic_int->lock), NULL);        
    }
    return atomic_int;
}

/**
 * Frees an atomic integer.
 *
 * @pre atomic_int != null
 * @param atomic_int Atomic integer to be freed
 */
void PICC_free_atomic_int(PICC_AtomicInt *atomic_int)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    free(atomic_int);
}

/**
 * Atomically sets the value to the given updated value if the current value
 * is equal to the expected value.
 *
 * @pre atomic_int != null
 * @post if (atomic_int.val@pre == expected_val) then atomic_int.val = new_val
 * @param atomic_int Atomic integer to be set
 * @param expected_val Expected current value
 * @param new_val New value of the integer
 * @return Value of the integer before the operation
 */
int PICC_atomic_int_compare_and_swap(PICC_AtomicInt *atomic_int, int expected_val, int new_val)
{
    LOCK_ATOMIC_VALUE(atomic_int);

    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        int val_at_pre = atomic_int->val;
    #endif

    int old_val = atomic_int->val;
    if (old_val == expected_val)
        atomic_int->val = new_val;

    #ifdef CONTRACT_POST
        // post: if (atomic_int.val@pre == expected_val) then atomic_int.val = new_val
        if (val_at_pre == expected_val)
            ASSERT(atomic_int->val == new_val);
    #endif

    RELEASE_ATOMIC_VALUE(atomic_int);
    return old_val;
}

/**
 * Atomically sets the value to the given updated value if the current value
 * is equal to the expected value, and returns whether the operation has been
 * done.
 *
 * @pre atomic_int != null
 * @post if (atomic_int.val@pre == expected_val) then atomic_int.val = new_val
 * @param atomic_int Atomic integer to be set
 * @param expected_val Expected current value
 * @param new_val New value of the integer
 * @return Whether the operation has been done
 */
bool PICC_atomic_int_compare_and_swap_check(PICC_AtomicInt *atomic_int, int expected_val, int new_val)
{
    LOCK_ATOMIC_VALUE(atomic_int);

    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        int val_at_pre = atomic_int->val;
    #endif

    int old_val = atomic_int->val;
    bool success = false;
    if (old_val == expected_val) {
        atomic_int->val = new_val;
    	success = true;
    }
    #ifdef CONTRACT_POST
        // post: if (atomic_int.val@pre == expected_val) then atomic_int.val = new_val
        if (val_at_pre == expected_val)
            ASSERT(atomic_int->val == new_val);
    #endif

    RELEASE_ATOMIC_VALUE(atomic_int);
    return success;
}

/**
 * Unconditionally gets the current value of the atomic integer value
 *
 * @pre atomic_int != null
 * @param atomic_int Atomic integer to get
 * @return Current value of the atomic integer
 */
int PICC_atomic_int_get(PICC_AtomicInt *atomic_int)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    return atomic_int->val;
}

/**
 * Atomically sets the atomic integer value.
 *
 * @pre atomic_int != null
 * @post atomic_int.val == new_val
 * @param atomic_int Atomic integer to be set
 * @param new_val New value
 * @return Previous value
 */
int PICC_atomic_int_get_and_set(PICC_AtomicInt *atomic_int, int new_val)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    int current;
    for (;;) {
        current = PICC_atomic_int_get(atomic_int);
        if (PICC_atomic_int_compare_and_swap_check(atomic_int, current, new_val))
            break;
    }

    #ifdef CONTRACT_POST
        // post: atomic_int.val == new_val
        ASSERT(atomic_int->val == new_val);
    #endif

    return current;
}

/**
 * Atomically increments the atomic integer value.
 *
 * @pre atomic_int != null
 * @post atomic_int.val > atomic_int.val@pre
 * @param atomic_int Atomic integer to be incremented
 * @return Previous value
 */
int PICC_atomic_int_get_and_increment(PICC_AtomicInt *atomic_int)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        int val_at_pre = atomic_int->val;
    #endif

    int current;
    for (;;) {
        current = PICC_atomic_int_get(atomic_int);
        if (PICC_atomic_int_compare_and_swap_check(atomic_int, current, current + 1))
            break;
    }

    #ifdef CONTRACT_POST
        // post: atomic_int.val > atomic_int.val@pre
        ASSERT(atomic_int->val > val_at_pre);
    #endif

    return current;
}

/**
 * Atomically decrements the atomic integer value.
 *
 * @pre atomic_int != null
 * @post atomic_int.val < atomic_int.val@pre
 * @param atomic_int Atomic integer to be decremented
 * @return Previous value
 */
int PICC_atomic_int_get_and_decrement(PICC_AtomicInt *atomic_int)
{
    #ifdef CONTRACT_PRE
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        int val_at_pre = atomic_int->val;
    #endif

    int current;
    for (;;) {
        current = PICC_atomic_int_get(atomic_int);
        if (PICC_atomic_int_compare_and_swap_check(atomic_int, current, current - 1))
            break;
    }

    #ifdef CONTRACT_POST
        // post: atomic_int.val < atomic_int.val@pre
        ASSERT(atomic_int->val < val_at_pre);
    #endif

    return current;
}
