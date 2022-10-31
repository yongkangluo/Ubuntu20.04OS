#include <lunaix/tty/tty.h>
void _kernel_init(){
    //tode
}

void _kernel_main(void* info_table){
    //Todo
    tty_set_theme(VGA_COLOR_GREEN, VGA_COLOR_BLUE);
    tty_put_str("hello kernel world!\nThis is second.");
}
