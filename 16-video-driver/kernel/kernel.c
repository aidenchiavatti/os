#include "../drivers/screen.h"

void main() {
    clear_screen();

    kprint("There is a line\n");
    kprint("There is a lineThere is a lineThere is a lineThere is a lineThere is a lineThere is a line\n");
    kprint_at("This line is specific to this location", 24, 2);
}
