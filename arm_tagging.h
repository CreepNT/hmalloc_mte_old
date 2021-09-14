#define TAG_SHIFT 56
#define MEMORY_TAG_MAX 15
#define MEM_TAG_FREE MEMORY_TAG_MAX //Use the highest memory tag for free slots

//Returns a tagged pointer, where the tag is different from MEM_TAG_FREE, adjacent_tag_1 and adjacent_tag_2 
//Set the latter two of those to MEM_TAG_FREE if no value is to be excluded
//If previous_tag is MEM_TAG_FREE, a random tag is generated with the hardware IRG instruction
//Else, tag is the closest increment (modulo MEM_TAG_FREE) of previous_tag that meets the above criteria
void* get_tagged_pointer(void* ptr, size_t previous_tag, size_t adjacent_tag_1, size_t adjacent_tag_2);

//Returns a tagged pointer, where tag is different from MEM_TAG_FREE
void* get_random_tagged_pointer(void* ptr);

size_t get_pointer_tag(void* ptr);

void* get_pointer_with_tag(void* ptr, size_t tag);

//Tags memory at ptr with tag inside ptr
void tag_memory_from_pointer(void* ptr);

//Tags memory at ptr with MEM_TAG_FREE
void set_free_tag_on_address(void* ptr);