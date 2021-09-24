@Helper rountines for ARMv8.5+ memory tagging
@Should be replaced with compiler intrinsics, when they become available
    .equ TAG_SHIFT 56
    .equ POINTER_ADDRESS_MASK 0xF0FFFFFFFFFFFFFF @Bottom 4 bytes of MSB

    .global __insert_random_tag
    irg x0, x0
    ret

    .global __store_tag
    stg x0, [x0]
    ret

    .global __store_tag_2
    st2g x0, [x0]
    ret

    .global __store_tag_zeroing
    stzg x0, [x0]
    ret

    .global __store_tag_zeroing_2
    stz2g x0, [x0]
    ret

    .global __get_pointer_tag
    and x0, x0, POINTER_ADDRESS_MASK
    lsr x0, TAG_SHIFT
    ret

    .global __set_pointer_tag
    and x0, x0, POINTER_ADDRESS_MASK
    lsl x1, x1, TAG_SHIFT
    orr x0, x0, x1
    ret

    .global __get_next_granule
    addg x0, x0, #1, #0
    ret
    
    .global __get_next_dgranule
    addg x0, x0, #2, #0
    ret