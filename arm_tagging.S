@Memory tagging routines for ARMv8.5+
    .equ TAG_SHIFT 56
    .equ MEM_TAG_FREE 15
    .equ POINTER_ADDRESS_MASK 0x00FFFFFFFFFFFFFF

@void* get_tagged_pointer(void* ptr, size_t previous_tag, size_t adjacent_tag_1, size_t adjacent_tag_2)
    .global get_tagged_pointer
    cmp x1, #15
    b.eq Lgenerate_new_tag
Lincrement_tag:
    add x1, #1
    cmp x1, MEM_TAG_FREE
    csel x1, x1, xzr, eq @x1 = ((x1 == 15) ? 0 : x1)
    cmp x2, MEM_TAG_FREE
    b.eq Lit_check_adj2
    cmp x1, x2
    b.eq Lincrement_tag
Lit_check_adj2:
    cmp x3, MEM_TAG_FREE
    b.eq Lit_return
    cmp x1, x3
    b.eq Lincrement_tag
Lit_return:
    lsl x8, x1, TAG_SHIFT
    orr x0, x0, x8
    ret

@x8 stores tagged pointer
Lgenerate_new_tag:
    irg x8, x0
    lsr x9, x8, TAG_SHIFT
    cmp x2, MEM_TAG_FREE
    b.eq Lgnt_check_adj2
    cmp x9, x2
    b.eq Lgenerate_new_tag
Lgnt_check_adj2:
    cmp x2, MEM_TAG_FREE
    b.eq Lgnt_return
    cmp x9, x2
    b.eq Lgenerate_new_tag
Lgnt_return:
    mov x0, x8
    ret

@void* get_random_tagged_pointer(void* ptr)
    .global get_random_tagged_pointer
    mov x1, x0
Lget_new_tag:
    irg x0, x1
    lsr x2, x0, TAG_SHIFT
    cmp x2, MEM_TAG_FREE
    b.eq Lget_new_tag
    ret

@viod* get_pointer_with_tag(void* ptr, size_t tag)
    .global get_pointer_with_tag
    mov x2, POINTER_ADDRESS_MASK
    and x0, x0, x2
    lsl x1, x1, TAG_SHIFT
    orr x0, x0, x1
    ret

@size_t get_pointer_tag(void* ptr)
    .global get_pointer_tag
    lsl x0, x0, TAG_SHIFT
    ret

@void tag_memory_from_pointer(void* ptr)
    .global tag_memory_from_pointer
    stg x0, [x0]
    ret

@void set_free_tag_on_address(void* ptr)
    .global set_free_tag_on_address
    mov x0, MEM_TAG_FREE
    lsl x0, TAG_SHIFT
    stg x0, [x1]