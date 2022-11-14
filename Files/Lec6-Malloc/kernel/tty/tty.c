#include<lunaix/tty/tty.h>
#include<stdint.h>

#define TTY_WIDTH 80
#define TTY_HEIGHT 25

vga_attributes *buffer = 0xB8000;
 
vga_attributes theme_color = VGA_COLOR_BLACK;

uint32_t TTY_COLUMN = 0;
uint16_t TTY_ROW = 0;


void
tty_init(void* vga_buf)
{

    buffer = (vga_attribute*)vga_buf;
    tty_clear();
    
}

void tty_set_theme(vga_attributes fg, vga_attributes bg){
    theme_color = (bg << 4 | fg) << 8;
}

void tty_put_char(char chr){
    if(chr == '\n'){
        TTY_COLUMN = 0;
        TTY_ROW++;
    }
    else if(chr == '\r'){
        TTY_COLUMN = 0;
    }
    else{
        *(buffer + TTY_COLUMN + TTY_ROW * TTY_WIDTH) = (theme_color | chr);
        ++TTY_COLUMN;
        if(TTY_COLUMN >= TTY_WIDTH){
            TTY_COLUMN = 0;
            TTY_ROW++;
        }
    }
    if(TTY_ROW >= TTY_HEIGHT){
        tty_scroll_up();
        TTY_ROW--;
    }



}
void tty_put_str(char* str){
    while(*str != '\0'){
        tty_put_char(*str);
        ++str;
    }
}
void tty_scroll_up(){

}
void
tty_clear()
{
    asm volatile("rep stosw" ::"D"(buffer),
                 "c"(TTY_HEIGHT * TTY_WIDTH),
                 "a"(theme_color)
                 : "memory");
}
void
tty_set_buffer(void* vga_buf)
{
    buffer = (vga_attribute*)vga_buf;
}