/**
 * @file main.c
 * Runs all unit tests.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Mickaël MENU
 */

#include <stdio.h>
#include <tests.h>

int main(int argc, char **argv)
{
    printf("== Run unit tests suite ==\n\n");

    PICC_test_queue();

    return 0;
}
