// PID: 730322500
// I pledge the COMP211 honor code.

// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

const char* valid_builtin_commands[] = {"cd", "exit", NULL};


void parse( char* line, command_t* p_cmd ) {
   
    p_cmd->path = malloc(sizeof(char)*100);
    p_cmd->argv = malloc(sizeof(char*)*50);
    for (int i=0; i<50; i++ ) p_cmd->argv[i] = malloc(sizeof(char)*100);

    p_cmd->argc = 0;

    int i = 0;
    char* token = strtok(line, " \n");

    while (token !=NULL) {
        strcpy(p_cmd->argv[i], token);
        token = strtok(NULL, " \n");
        i++;
    }
    p_cmd->argc = i;
    p_cmd->argv[i] = NULL;  

    find_fullpath(p_cmd->argv[0], p_cmd);

} // end parse function


int find_fullpath( char* command_name, command_t* p_cmd ) {

    char fullpath[200];
    int exists = FALSE;
    struct stat buffer;

    //check no command
    if (p_cmd->argc == 0) {
        return exists;
    }

    //check built in
    if (equals(p_cmd->argv[0],valid_builtin_commands[0]) || equals(p_cmd->argv[0],valid_builtin_commands[1])) {
        strcpy(fullpath, p_cmd->argv[0]);
        strcpy(p_cmd->path, fullpath);
        return exists;
    }

    //making copy of path string
    char * path_string = getenv("PATH");
    char path_string_array[100];
    
    int i = 0;

    while (path_string[i] != '\0') {
        path_string_array[i] = path_string[i];
        i++;
    }
    path_string_array[i] = '\0';

    // loop through the string to extract all other tokens
    char * token = strtok(path_string_array, ":");
    
    while( token != NULL ) {
        //create fullpath
        int i = 0;
        for (i=0; i<strlen(token); i++) {
            fullpath[i] = token[i];
        }
        fullpath[i++] = '/';
        int j = 0;
        while (p_cmd->argv[0][j] != '\0') {
            fullpath[i] = p_cmd->argv[0][j];
            i++;
            j++;
        }
        fullpath[i] = '\0';
        exists = stat( fullpath, &buffer );
        if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) )  {
                  // Directory exists
        } else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
                  // File exists;
            strcpy(p_cmd->path, fullpath);
            return exists;
        } else { 

        }
        token = strtok(NULL, ":");

    }

    //if did not find path file [non valid command]
    p_cmd->argc = -1;
    strcpy(p_cmd->path,p_cmd->argv[0]);
    return 0;

} // end find_fullpath function


int execute( command_t* p_cmd ) {
    int status = SUCCESSFUL;
    if ( p_cmd->argc > 0 ) {

        if ( fork() == 0 ) {
            status = execv( p_cmd->path, p_cmd->argv );
            return status;
        }
        wait(NULL);
    }

    return status;

} // end execute function


int is_builtin( command_t* p_cmd ) {

    int cnt = 0;

    while ( valid_builtin_commands[cnt] != NULL ) {

        if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

            return TRUE;

        }

        cnt++;

    }

    return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

    // only builtin command is cd

    if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

    struct stat buff;
    int status = ERROR;

    if ( p_cmd->argc == 1 ) {

        // -----------------------
        // cd with no arg
        // -----------------------
        // change working directory to that
        // specified in HOME environmental 
        // variable

        status = chdir( getenv("HOME") );

    } else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


        // -----------------------
        // cd with one arg 
        // -----------------------
        // only perform this operation if the requested
        // folder exists

        status = chdir( p_cmd->argv[1] );

    } 

    return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

    int i=0;

    while ( p_cmd->argv[i] != NULL ) {
        free( p_cmd->argv[i] );
        i++;
    }

    free( p_cmd->argv );
    free( p_cmd->path );	

} // end cleanup function


int equals( char* str1, const char* str2 ) {
    int len[] = {0,0};

    char* b_str1 = str1;
    const char* b_str2 = str2;

    while( (*str1) != '\0' ) { 
        len[0]++;
        str1++;
    }

    while( (*str2) != '\0' ) {
        len[1]++;
        str2++;
    }

    if ( len[0] != len[1] ) {

        return FALSE;

    } else {

        while ( (*b_str1) != '\0' ) {

            if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

                return FALSE;

            }

            b_str1++;
            b_str2++;

        }

    } 

    return TRUE;


} // end compare function definition

