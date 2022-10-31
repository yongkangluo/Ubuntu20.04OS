ENTRY(start_)

/*
    FUTURE: Use disk reader
    A bit of messy here.
    We will pull our higher half kernel out of this shit
      and load it separately once we have our disk reader.
*/

SECTIONS {
    . = 0x100000;
    /* 好了，我们的内核…… */
    .text BLOCK(4K) : {
        * (.multiboot)
        * (.text)
    }
    .bss BLOCK(4K) : {
        * (COMMON)
        * (.bss)
    }
    .data BLOCK(4K) : {
        * (.data)
    }
    .rodata BLOCK(4K) :{
        * (.rodata)
    }

}
