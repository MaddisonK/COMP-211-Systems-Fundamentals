// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//		argv = array of argument values
//
//
int main( int argc, char** argv ) {

    int done = FALSE;
    int cmd_cnt = 0;
    int i = 0;

    char shell_cmd[100];

    command_t command;

    // -------------------------------------------
    // Go ahead and allocate memory
    // makes things less complicated :)
    command.path = malloc(sizeof(char)*100);
    command.argv = malloc(sizeof(char*)*50);
    for ( i=0; i<50; i++ ) command.argv[i] = malloc(sizeof(char)*100);

    while ( !done ) {

        printf("[%s](%d)$ ", getenv("USER"), cmd_cnt++ );
        fgets( shell_cmd, 500, stdin );

        parse( shell_cmd, &command );

        if ( ( command.argc > 0 ) && equals( command.path, "exit" )  ) {

            done = TRUE;

        } else if ( command.argc == ERROR ) { 

            fprintf( stderr, "%s: command not found!\n", command.path );

        } else if ( command.argc > 0 ) {

            if ( is_builtin( &command ) ) {

                if ( do_builtin( &command ) != SUCCESSFUL ) {

                    fprintf( stderr, "%s command failed: requested folder [%s] does not exist!\n", 
                            command.path, command.argv[0] );

                }

            } else if ( execute( &command ) != SUCCESSFUL ) {

                fprintf( stderr, "%s command failed!\n", command.path );

            } 

        }

    }

    cleanup( &command );

    return 0;

} // end main function
