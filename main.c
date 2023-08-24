#include <stdio.h>
#include "functions.h"

/*WRITTEN BY: HADE BAYAA*/

/*COMPILED USING THIS COMMAND: */
/*gcc -o records_db -std=c99 -Wall -pedantic-errors main.c functions.c prog2_ex1.c*/

/*EXECUTED USING THIS COMMAND: */
/*./records_db -i input.in -o output.in*/

int main(int argc, char *argv[]) 
{
    /*Using input.in as input file*/
    
    Execute_Records_DB(argc, argv[2], argv[4]);

    return 0;
}
