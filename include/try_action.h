
/**
 * @file try_action.h
 * Runtime support functions for input/output tries.
 *
 * This project is released under MIT License.
 *
 * @author Frederic Peschanski
 */

#ifndef TRY_ACTION_H
#define TRY_ACTION_H

#include <channel.h>
#include <commit.h>

extern PICC_Commit *PICC_try_output_action(PICC_Channel *out_chan, PICC_TryResult *try_result);
extern PICC_Commit *PICC_try_input_action(PICC_Channel *in_chan, PICC_TryResult *try_result);

#endif
