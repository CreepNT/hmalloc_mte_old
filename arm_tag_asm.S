/* Helper rountines for ARMv8.5+ memory tagging
Should be replaced with compiler intrinsics, when they become available*/
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
    lsr x0, x0, #56
    and x0, x0, #0xF
    ret

    .global __set_pointer_tag
    movn x2, #0x0F00, LSL #48 //x2 = 0xF0F...FF
    and x0, x0, x2 //Clear existing tag
    lsl x1, x1, #56
    orr x0, x0, x1
    ret

    .global __get_next_granule
    addg x0, x0, #1, #0
    ret
    
    .global __get_next_dgranule
    addg x0, x0, #2, #0
    ret