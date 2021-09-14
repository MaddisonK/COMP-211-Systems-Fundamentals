// PID: 730322550
// I pledge the COMP211 honor code.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_objects.h"
#include "parse_functions.h"

#define PAGE_SIZE 4096
char RET1[] = "static";
char RET2[] = "dynamic";

int size_of_memmap_array(memmap* maps){
    int size = 0;
    while (maps[size].name != NULL){
        size++;
    }
    return size;
}

void parseline(char* line, memmap* mm){
    char map_name[100], fd[10], dummy[200];

    unsigned long int mem_start, mem_end;
    char r, w, x;

    // Formatted Input - Scanf (Section 7.4 in C book)
    // Read in all desired variables
    sscanf(line, "%lx-%lx %c%c%cp %s %s %s %s",
            &mem_start, &mem_end, &r, &w, &x, dummy, fd, dummy, map_name);


    // Populate memmap struct
    mm->start_addr = mem_start;
    mm->r = r;
    mm->w = w;
    mm->x = x;
    mm->end_addr = mem_end;

    // Have to allocate space to copy the string fields
    char* name = (char *) malloc(100);
    strcpy(name, map_name);
    mm->name = name;

    char* linecpy = (char *) malloc(200);
    strcpy(linecpy, line);
    mm->original_input = linecpy; 

    // STUDENT DEFINED FUNCTIONS
    mm->length = memmap_size(*mm);
}


// Print contents of a memmap struct in a nice way.
void pprint(memmap mm){
    printf("Original text: %s", mm.original_input);
    printf(" Map name: %s\n Start address: %lu\n End address: %lu\n Permissions: %c%c%c\n",
            mm.name, mm.start_addr, mm.end_addr, mm.r, mm.w, mm.x);
    printf("Dependent attributes:\n Section %d\n\n",
            mm.section);
    // printf("Length %d\n", mm.length);
}


struct memmap* populate_mmap(){
    char line[200];
    struct memmap* maps = malloc(200 * sizeof(struct memmap));

    int i = 0;

    unsigned long int num_dummy;
    char char_dummy;
    char string_dummy[200];
    char map_name[200];
    while (fgets(line, 200, stdin) != NULL){
        // determine if the memory mapping has a name 
        // in the last column of the proc maps file
        strcpy(map_name, "unpopulated");
        sscanf(line, "%lx-%lx %c%c%cp %s %s %s %s",
            &num_dummy, &num_dummy, &char_dummy, &char_dummy, &char_dummy, 
            string_dummy, string_dummy, string_dummy, map_name);

        // Only include mappings with names in the final array of structs
        if (strcmp(map_name, "unpopulated") != 0) { 
            parseline(line, &maps[i]);
            i++;
        }
    }

    // STUDENT DEFINED FUNCTIONS
    assign_sections(maps);
    assign_filetypes(maps);

    return maps;
}

void assign_sections(memmap* maps) {
    int s = size_of_memmap_array(maps);
    char heap_string[] = "[heap]";
    char stack_string[] = "[stack]";
    int btwn = 0;

    for (int i = 0; i<s; i++) {
        if (strcmp(maps[i].name, heap_string) == 0) {
            maps[i].section = HEAP;
            btwn = 1;
            continue;
        } else if (strcmp(maps[i].name, stack_string) == 0) {
            maps[i].section = STACK;
            btwn = 0;
            continue;
        } else {
            if (strstr(maps[i].name,".so") || btwn) {
                maps[i].section = SHAREDLIB;
                continue;
            }
        }

        if (maps[i].r == 'r' && maps[i].w == '-' && maps[i].x == '-') {
            maps[i].section = RODATA;
        } else if (maps[i].r == 'r' && maps[i].w == '-' && maps[i].x == 'x') {
            maps[i].section = TEXT;
        } else if (maps[i].r == 'r' && maps[i].w == 'w' && maps[i].x == '-') {
            maps[i].section = DATA;
        }
    }

}

void assign_filetypes(memmap* maps) {
    int s = size_of_memmap_array(maps);
    for (int i = 0; i<s; i++) {
        if (maps[i].section == RODATA || maps[i].section == DATA || maps[i].section == TEXT) {
            maps[i].file_type = EXEC_OBJ_FILE;
        } else if (maps[i].section == SHAREDLIB) {
            maps[i].file_type = SHARED_OBJ_FILE;
        } else {
            maps[i].file_type = NA;
        }
    }
}

char* linking_type(memmap* maps) {
    int count = 0;
    int s = size_of_memmap_array(maps);
    for (int i = 0; i<s; i++) {
        if (maps[i].section == SHAREDLIB) {
            return RET2;
        }
    }
    return RET1;
}

int total_bytes_of_section_type(memmap* maps, enum ProcSection stype) {
    int count = 0;
    int s = size_of_memmap_array(maps);
    for (int i = 0; i<s; i++) {
        if (maps[i].section == stype) {
            count += maps[i].length;
        }
    }
    return count;
}

int total_bytes_of_file_type(memmap* maps, enum FileType ftype) {
    int count = 0;
    int s = size_of_memmap_array(maps);
    for (int i = 0; i<s; i++) {
        if (maps[i].file_type == ftype) {
            count += maps[i].length;
        }
    }
    return count;
}

int total_pages_of_section_type(memmap* maps, enum ProcSection stype) {
    int count = 0;
    int s = size_of_memmap_array(maps);
    for (int i = 0; i<s; i++) {
        if (maps[i].section == stype) {
            count += maps[i].length;
        }
    }
    return count/PAGE_SIZE;
}

int total_pages_of_file_type(memmap* maps, enum FileType ftype) {
    int count = 0;
    int s = size_of_memmap_array(maps);
    for (int i = 0; i<s; i++) {
        if (maps[i].file_type == ftype) {
            count += maps[i].length;
        }
    }
    return count/PAGE_SIZE;
}

unsigned long int memmap_size(memmap mm) {
    return mm.end_addr - mm.start_addr;
}
