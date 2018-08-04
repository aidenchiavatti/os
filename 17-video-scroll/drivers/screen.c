#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

int print_char(char character, int row, int col, char attribute_byte);
int get_screen_offset(int row, int col);
int get_cursor();
void set_cursor(int offset);
int get_offset_row(int offset);
int get_offset_col(int offset);
int handle_scrolling(int cursor_offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/
void kprint_at(char* message, int row, int col) {
    int offset;
    //check if negative. set row and col based on cursor offset
    if(row < 0 | col < 0) {
        offset = get_cursor();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    } else {
        offset = get_screen_offset(row, col);
    }

    int i = 0;
    while(message[i]) {
        offset = print_char(message[i++], row, col, 0);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char* message) {
    kprint_at(message, -1, -1);
}

void clear_screen() {
    char* screen = VIDEO_ADDRESS;
    int screen_size = MAX_ROWS * MAX_COLS;
    int i;
    for(i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor(get_screen_offset(0, 0));
}

/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char character, int row, int col, char attribute_byte) {
    char *vidmem = VIDEO_ADDRESS;

    if(!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

    if(row >= MAX_ROWS || col >= MAX_COLS) {
        vidmem[2*MAX_ROWS*MAX_COLS - 2] = 'E';
        vidmem[2*MAX_ROWS*MAX_COLS - 1] = 0x06;
        return get_screen_offset(row, col);
    }

    int offset;
    if(row >= 0 & col >= 0) {
        offset = get_screen_offset(row, col);
    } else {
        offset = get_cursor();
    }

    if(character == '\n') {
        offset = get_screen_offset(row+1, 0);
    } else {
        vidmem[offset] = character;
        vidmem[offset+1] = attribute_byte;
        offset += 2;
    }

    set_cursor(offset);
    handle_scrolling(offset);
    return offset;
}

int get_screen_offset(int row, int col) {
    return (row*MAX_COLS + col) * 2;
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL , 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL , 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL , 14);
    port_byte_out(REG_SCREEN_DATA , (unsigned  char)( offset  >> 8));
    port_byte_out(REG_SCREEN_CTRL , 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int get_offset_row(int offset) {
    return (offset / 2) / MAX_COLS;
}

int get_offset_col(int offset) {
    return (offset / 2) % MAX_COLS;
}

int handle_scrolling(int cursor_offset) {
    //if cursor is within screen, do nothing
    if(cursor_offset < MAX_ROWS*MAX_COLS*2) {
        return cursor_offset;
    }

    //shift each line up by 1
    int i;
    for(i = 1; i < MAX_ROWS; i++) {
        memory_copy(get_screen_offset(i, 0) + VIDEO_ADDRESS,
                    get_screen_offset(i - 1, 0) + VIDEO_ADDRESS,
                    MAX_COLS*2
        );
    }

    //blank out last line
    char* last_line = get_screen_offset(MAX_ROWS - 1, 0) + VIDEO_ADDRESS;
    for(i = 0; i < MAX_COLS*2; i++) {
        //*(last_line + i) = 0;
        if(i % 2 == 0) {
            last_line[i] = 0;
        } else {
            last_line[i] = WHITE_ON_BLACK;
        }
    }

    //move mouse up one line
    cursor_offset -= 2*MAX_COLS;
    set_cursor(cursor_offset);
}