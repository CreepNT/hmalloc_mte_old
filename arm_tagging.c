#include <stddef.h>
#include "util.h"

#include "arm_tagging.h"

//Check if tag is different than cmp, if cmp is not MEM_TAG_FREE
#define TAGS_DIFFERENT_IGNFREE(tag, cmp) ((tag != cmp) && (cmp == MEM_TAG_FREE))

/* ASM helpers prototypes */
void* __insert_random_tag(void* p); //Returns p with a hardware-generated random tag inserted
void __store_tag(void* p);   //Stores tag from p into the granule pointed to
void __store_tag_2(void* p); //Stores tag from p into two granules pointed to
void __store_tag_zeroing(void* p);  //Stores tag from p and zeroes out a granule    
void __store_tag_zeroing_2(void* p);//Stores tag from p and zeroes out two granules
size_t __get_pointer_tag(void* p);  //Returns the tag stored in p
void* __set_pointer_tag(void* p, size_t tag); //Returns p with its tag replaced by provided tag
void* __get_next_granule(void* p);  //Returns a pointer to the granule located right after the one pointed to by p in memory
void* __get_next_dgranule(void* p); //Returns a pointer to the granule located two granules after the one pointed to by p in memory

/* Prototypes for exported functions (needed by -Werror,-Wmissingprototypes) */
void* get_random_tagged_pointer(void* ptr);
void* get_tagged_pointer(void* ptr, size_t previous_tag, size_t adjacent_tag_1, size_t adjacent_tag_2);
size_t get_pointer_tag(void* ptr);
void* set_pointer_tag(void* ptr, size_t tag);
void zero_and_tag_area(void* ptr, size_t size);
void zero_and_tag_area_free(void* ptr, size_t size);

void* get_random_tagged_pointer(void* ptr) {
    void* p;
    do {
        p = __insert_random_tag(ptr);
    } while (__get_pointer_tag(p) == MEM_TAG_FREE);
    return p;
}

void* get_tagged_pointer(void* ptr, size_t previous_tag, size_t adjacent_tag_1, size_t adjacent_tag_2) {
    if (previous_tag == MEM_TAG_FREE) { //Never tagged before, generate random tag for first use
        void* tagged;
        size_t tag;
        do {
            tagged = get_random_tagged_pointer(ptr); //Call this to avoid getting MEM_TAG_FREE
            tag = __get_pointer_tag(tagged);
        } while (!TAGS_DIFFERENT_IGNFREE(tag, adjacent_tag_1) && !TAGS_DIFFERENT_IGNFREE(tag, adjacent_tag_2));

        return tagged;
    } else {
        size_t tag = (previous_tag + 1) % MEM_TAG_FREE;
        while (!TAGS_DIFFERENT_IGNFREE(tag, adjacent_tag_1) && !TAGS_DIFFERENT_IGNFREE(tag, adjacent_tag_2)) //Runs at most 2 times, no need to check that we haven't looped around
            tag = (tag + 1) % MEM_TAG_FREE;

        return __set_pointer_tag(ptr, tag);
    }
}

size_t get_pointer_tag(void* ptr) {
    return __get_pointer_tag(ptr);
}

void* set_pointer_tag(void* ptr, size_t tag) {
    return __set_pointer_tag(ptr, tag);
}

void zero_and_tag_area(void* ptr, size_t size) {
    if (unlikely((size % GRANULE_SIZE) != 0)) { //Check granule alignement
        fatal_error("tried to tag misaligned area");
    }
    while ((size % DGRANULE_SIZE) != 0) { //Zero-out and tag double-granule-sized areas
        size -= DGRANULE_SIZE;
        __store_tag_zeroing_2(ptr);
        ptr = __get_next_dgranule(ptr);
    }
    if ((size % GRANULE_SIZE) != 0) //Zero-out last granule, if needed (there can only be 1 - two granules would be included in double-granule-sized area)
        __store_tag_zeroing(ptr);
    return;
}

void zero_and_tag_area_free(void* ptr, size_t size) {
    void* p = __set_pointer_tag(ptr, MEM_TAG_FREE);
    return zero_and_tag_area(p, size);
}