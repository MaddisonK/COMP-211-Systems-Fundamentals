// PID: 730322550
// I pledge the COMP211 honor code.
// -----------------------------------
// COMP 211 - Systems Fundamentals
// Spring 2021
// memory.c source file
// 
// Lab 05
//
// -----------------------------------

#include "memory.h"
#include "bit_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int readMemoryFile( char* file_name ) {

    // -------------------------------------
    // Please do not modify this code
    // -------------------------------------

    int addr_cnt = 0;
    int addr_bits = 0;
    int r_flag;

    FILE* fhnd;

    // Open file handler
    fhnd = fopen( file_name, "r" );

    if ( fhnd != NULL ) {

        phy_memory = malloc( sizeof(unsigned int)*MAX_SIZE );

        // Store the values in memory in the phy_memory array until EOF is reached.
        // r_flag is the return value of fscanf; will be EOF when file is reached; number of fields assigned otherwie.
        r_flag = fscanf( fhnd, "%x\n", &phy_memory[addr_cnt] );

        // For each address, increment total address count and store value in physical memory.
        while ( r_flag != EOF ) {

            addr_cnt++;

            r_flag = fscanf( fhnd, "%x\n", &phy_memory[addr_cnt] );

        }

        phy_memory[addr_cnt] = '\0';

        // If the file was found to be empty, no addresses of physical memory were counted.
    } else addr_cnt = READ_ERROR;

    // Close file handler.
    fclose( fhnd );

    // Optional debug statement.
    if ( MEM_DEBUG ) {
        printf("Number of bytes read %d\n", addr_cnt );
    }

    // If a valid file was read, then set the number of address bits to the corresponding log of 2.
    if ( addr_cnt != READ_ERROR )
        addr_bits = log2( addr_cnt );
    // Otherwise, set number of address bits to 0.
    else
        addr_bits = addr_cnt;

    return addr_bits;

} // end readMemoryFile function




int numberOfBlocks( unsigned int addr_bits, unsigned int num_block_offset_bits ) {
    if (addr_bits < 1) {
        return FAIL;
    }

    if (num_block_offset_bits < 1) {
        return FAIL;
    }

    int block_bits = addr_bits - num_block_offset_bits;
    if (block_bits < 0) {
        return FAIL;
    }

    return pow(2, block_bits);
    
} // end numberOfBlocks function



void initializeBlockPointers( unsigned int num_of_blocks, unsigned int num_block_offset_bits  ) {

    int i;
    // Here we create a mapping to the starting addresses of each block (blocks are just groupings of physical memory into chunks).
    // i.e. if num_block_offset_bits is 3, then each block is size 2^3. the block locations will be: [0, 8, 16, 24, ...]
    block_location = malloc( sizeof( unsigned int) * num_of_blocks );

    for ( i=0; i<num_of_blocks; i++ ) {

        block_location[i] = i*exp2( num_block_offset_bits );

        if ( MEM_DEBUG ) {
            printf("starting block address (base-16) %02X (%d base-10)\n", block_location[i], block_location[i] );
        }

    }

} // end initalizeBlockPointers function
