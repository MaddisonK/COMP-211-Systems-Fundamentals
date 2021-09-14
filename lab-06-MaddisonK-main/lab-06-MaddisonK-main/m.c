#include "parse.c"

int main(){
    memmap* maps = populate_mmap();
    int size = size_of_memmap_array(maps);

    for (int i=0; i<size; i++){
        pprint(maps[i]);
    }
    return 0;
}
