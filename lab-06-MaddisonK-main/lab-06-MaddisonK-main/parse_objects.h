// Section label of a memory mapping, based on the different sections
// that make up a process
enum ProcSection {STACK, SHAREDLIB, HEAP, TEXT, RODATA, DATA};
enum FileType {SHARED_OBJ_FILE, EXEC_OBJ_FILE, NA};

// Represents information about a memory mapping from a single line in a
// /proc/<PID>/maps file. 
typedef struct memmap{
    // fields populated by parse.c
    char* original_input;
    char* name;
    unsigned long int start_addr;
    unsigned long int end_addr; 
    char r, w, x; 

    // fields populated by you.
    int length;
    enum ProcSection section;
    enum FileType file_type;
} memmap;
