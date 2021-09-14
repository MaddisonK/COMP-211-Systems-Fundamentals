// Given a memory mapping, what is the total length of the address space?
unsigned long int memmap_size(memmap mm);

// Given memory mappings for a process, determine a section label for each. 
void assign_sections(memmap* maps);

// Given memory mappings for a process, determine a file type label for each. 
void assign_filetypes(memmap* maps);

// Determine if a process was statically or dynamically compiled.
// Given an array of memmaps, return "static" if the file was statically
// compiled, "dynamic" otherwise.
char* linking_type(memmap* maps);

// Given an array of memory mappings and a section label, for all
// mappings in the array with that section label, what is their total combined
// number of bytes?
int total_bytes_of_section_type(memmap* maps, enum ProcSection stype);

// Given an array of memory mappings and a file type, for all
// mappings in the array with that file type, what is their total combined
// number of bytes?
int total_bytes_of_file_type(memmap* maps, enum FileType ftype);

// Given an array of memory mappings and a section label, for all
// mappings in the array with that section label, what is their total combined
// number of pages?
int total_pages_of_section_type(memmap* maps, enum ProcSection stype);

// Given an array of memory mappings and a file type, for all
// mappings in the array with that file types, what is their total combined
// number of pages?
int total_pages_of_file_type(memmap* maps, enum FileType ftype);

