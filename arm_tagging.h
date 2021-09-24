#include <stddef.h>

/* start of Linux headers */
//From arch/arm64/include/uapi/asm/mman.h
#define PROT_MTE	0x20		/* Normal Tagged mapping */

//From include/uapi/linux/prctl.h
#define PR_SET_TAGGED_ADDR_CTRL 55
#define PR_TAGGED_ADDR_ENABLE  (1UL << 0)
#define PR_MTE_TCF_SHIFT       1
#define PR_MTE_TCF_SYNC        (1UL << PR_MTE_TCF_SHIFT)
#define PR_MTE_TAG_SHIFT       3
/* end of Linux headers */


#define GRANULE_SIZE 16 //Size, in bytes, of a granule (smallest size the CPU can tag) 
#define DGRANULE_SIZE (2 * GRANULE_SIZE) //Size of two granules (Double GRANULE SIZE)

#define MEMORY_TAG_MAX 15 //Maximum value a memory tag can take
#define MEM_TAG_FREE MEMORY_TAG_MAX //Use the highest memory tag for free slots

//Returns a tagged pointer, where the tag is different from MEM_TAG_FREE, adjacent_tag_1 and adjacent_tag_2 
//Set the latter two of those to MEM_TAG_FREE if no value is to be excluded
//If previous_tag is MEM_TAG_FREE, a random tag is generated with the hardware IRG instruction
//Else, tag is the closest increment (modulo MEM_TAG_FREE) of previous_tag that meets the above criteria
void* get_tagged_pointer(void* ptr, size_t previous_tag, size_t adjacent_tag_1, size_t adjacent_tag_2);

//Returns a pointer with a random tag, different from MEM_TAG_FREE
void* get_random_tagged_pointer(void* ptr);

//Returns the tag of the pointer
size_t get_pointer_tag(void* ptr);

//Returns the provided pointer, with tag changed to 'tag'
void* get_pointer_with_tag(void* ptr, size_t tag);

/* For all the following functions, size must be GRANULE_SIZE aligned */
//Marks size bytes starting from ptr with the tag embedded in ptr
void tag_area(void* ptr, size_t size);

//Marks size bytes starting from ptr with the MEM_TAG_FREE tag
void tag_area_as_free(void* ptr, size_t size);

//Same as tag_area, but also zeroes out the memory
void zero_and_tag_area(void* ptr, size_t size);

//Same as tag_area_as_free, but also zeroes out the memory
void zero_and_tag_area_as_free(void* ptr, size_t size);

