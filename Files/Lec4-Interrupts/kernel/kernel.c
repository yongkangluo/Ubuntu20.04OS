#include <lunaix/tty/tty.h>
#include <lunaix/arch/gdt.h>
#include <lunaix/arch/idt.h>

void _kernel_init(){
    //tode
    _init_gdt();
    _init_idt();

}

void _kernel_main(void* info_table){

    //Todo
    (void) info_table;
    tty_set_theme(VGA_COLOR_RED, VGA_COLOR_BLACK);
    // tty_put_str("hello kernel world!\nThis is second.");
    __asm__("int $0");
}
