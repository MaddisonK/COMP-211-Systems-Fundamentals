// PID: 730322550
// I pledge the COMP211 honor code.
// -----------------------------------
// COMP 211 - Systems Fundamentals
// Spring 2021
// cache.c source file
// 
// Lab 05
//
// -----------------------------------

#include "cache.h"
#include "memory.h"
#include "bit_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int initializeCache( unsigned int number_of_lines ) {

    int line, retVal;

    retVal = OK;

    // Allocate memory for the cache (array of cache lines).
    cache = malloc( sizeof( cache_line* ) * number_of_lines );

    // For each cache line, allocate memory and initialize the cache line.
    if ( cache != NULL ) {

        for ( line=0; line<number_of_lines; line++ ) {

            cache[line] = (cache_line*) malloc( sizeof( cache_line ) );
            cache[line]->tag = EMPTY;
            cache[line]->hit_count = ZERO;

        }

    } else
        retVal = FAIL;

    return retVal;

} // end initializeCache function


int cread( unsigned int cmf, unsigned int* hex_addr, unsigned int* found, unsigned int* replace ) {

    // retVal is either FAIL or the value of the requested hexadecimal address.
    // open_line indicates a line that is open, and thus usable.
    // replace_line indicates the cache line that should be replaced (in the direct mapping case).
    // min_hit_cnt is used in the fully associative case, and used to keep track of the LFU cache line.

    int retVal = FAIL;
    int line, replace_line, min_hit_cnt;

    min_hit_cnt = (int)1E10;

    // tag is the tag bits
    // block_offset is the number of bits into a block the location is (Hint: Need to initialize).
    unsigned int tag, block_offset;

    // Variables to be passed by reference. - we want to indicate to the driver class whether we found the item we wanted in the cache and whether we had to overwrite and replace something
    (*found) = MISS;
    (*replace) = NO;

    if ( ( cmf == DM ) && ( (*hex_addr) < exp2( addr_bits ) ) ) {
        // TO-DO: PART 1 - DIRECT MAPPING ALGORITHM
        block_offset = bit_select(*hex_addr, NUM_BLOCK_OFFSET_BITS - 1, 0); // 1,0
        line = bit_select(*hex_addr, NUM_OF_LINE_BITS + (NUM_BLOCK_OFFSET_BITS - 1), NUM_BLOCK_OFFSET_BITS); // 4,2
        tag = bit_select(*hex_addr, NUM_OF_TAG_BITS_DM + NUM_OF_LINE_BITS + (NUM_BLOCK_OFFSET_BITS - 1), NUM_OF_LINE_BITS + NUM_BLOCK_OFFSET_BITS); // 7,5
        unsigned int block_index = bit_select(*hex_addr,NUM_OF_TAG_BITS_DM + NUM_OF_LINE_BITS + (NUM_BLOCK_OFFSET_BITS - 1), NUM_BLOCK_OFFSET_BITS); // 7,2
        
        if (cache[line]->tag == EMPTY) {
            cache[line]->tag = tag;
            for (int offset = 0; offset<4; offset++) {
                cache[line]->block[offset] = phy_memory[block_location[block_index] + offset];
            }
            cache[line]->hit_count++;

        } else if (cache[line]->tag != tag){
            cache[line]->tag = tag;
            for (int offset = 0; offset<4; offset++) {
                cache[line]->block[offset] = phy_memory[block_location[block_index] + offset];
            }
            (*replace) = YES;
            cache[line]->hit_count = 1;

        } else {
            (*found) = HIT;
            cache[line]->hit_count++;
        }
        
    } else if ( ( cmf == FA ) && ( (*hex_addr) < exp2( addr_bits ) ) ) {
        // TO-DO: PART 2 - FULLY ASSOCIATIVE ALGORITHM
        block_offset = bit_select(*hex_addr, NUM_BLOCK_OFFSET_BITS - 1, 0); // 1,0
        tag = bit_select(*hex_addr, NUM_OF_TAG_BITS_FA + (NUM_BLOCK_OFFSET_BITS - 1), NUM_BLOCK_OFFSET_BITS); // 7,2
        unsigned int block_index = tag;
        for (line = 0; line<NUM_OF_LINES; line++) {

            if (cache[line]->tag == tag) {
                (*found) = HIT;
                cache[line]->hit_count++;
                (*replace) = NO;
                break;
            }

            if (cache[line]->tag == EMPTY) {
                cache[line]->tag = tag;
                for (int offset = 0; offset<4; offset++) {
                    cache[line]->block[offset] = phy_memory[block_location[block_index] + offset];
                }
                cache[line]->hit_count++;
                (*replace) = NO;
                break;
            }

            if (cache[line]->hit_count < min_hit_cnt) {
                min_hit_cnt = cache[line]->hit_count;
                replace_line = line;
                (*replace) = YES;
            }
        }

        if (*replace == YES) {
            cache[replace_line]->tag = tag;
            for (int offset = 0; offset<4; offset++) {
                cache[replace_line]->block[offset] = phy_memory[block_location[block_index] + offset];
            }
            cache[replace_line]->hit_count = 1;
            line = replace_line;
        }
    }

    // Print out the state of the cache after the mapping algorithm has been applied.
    cprint();

    retVal = cache[line]->block[block_offset];
    return retVal;

} // end cread function



void cprint() {

    unsigned int line;

    printf("\n---------------------------------------------\n");
    printf("line\ttag\tnum of hits\n");
    printf("---------------------------------------------\n");

    for ( line=0; line<NUM_OF_LINES; line++ ) { 

        if ( cache[line]->tag == EMPTY ) {

            printf("%d\t%d\t%d\n", line, cache[line]->tag, cache[line]->hit_count );

        } else {

            printf("%d\t%02X\t%d\n", line, cache[line]->tag, cache[line]->hit_count );

        }

    }

} // end cprint function
