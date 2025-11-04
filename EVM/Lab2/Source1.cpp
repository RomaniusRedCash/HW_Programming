#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ENTIRE, UP, DOWN };

void scroll(int direction, char lines, char l_row, char l_col, char r_row, char r_col, char attr)
{
    union REGS r;


    if (direction == DOWN)
        r.h.ah = 7;
    else
        r.h.ah = 6;

    r.h.al = lines;
    r.h.ch = l_row;
    r.h.cl = l_col;
    r.h.dh = r_row;
    r.h.dl = r_col;
    r.h.bh = attr;

    int86(0x10, &r, &r);
}

static char* get_color(const int& i){
    static char* colors[] = {"BLACK", "BLUE", "GREEN", "CYAN", "RED", "MAGENTA", "BROWN", "LIGHTGRAY", "DARKGRAY", "LIGHTBLUE", "LIGHTGREEN", "LIGHTCYAN", "LIGHTRED", "LIGHTMAGENTA", "YELLOW", "WHITE"};
    return colors[i];
}


int main(void)
{
    int x1 = 10, y1 = 12, x2 = 70, y2 = 23;
    double T = 0.3;
    int S = 3;

    clrscr();
    window(x1, y1, x2, y2);

    //scroll(ENTIRE, 0, y1-1, x1-1, y2+1, x2+1, get_color_attr(15, 15));



    int bg = 0, fg = 0;
    for(bg = 0; bg < 8; bg++) {
        for(fg = 0; fg < 16; fg++) {
            if(fg == bg) continue;
            scroll(ENTIRE, 0, y1-2, x1-2, y2, x2, fg << 4);

            scroll(UP, 0, y1-1, x1-1, y2-1, x2-1, bg << 4);

            textbackground(bg);
            textcolor(fg);
            cprintf("\rBG:%s FG:%s", get_color(bg), get_color(fg));
            delay(T*1000);
        }
    }



//     for(bg = 0; bg < 8; bg++) {
//         for(fg = 0; fg < 16; fg++) {
//             if(fg == bg) continue;
//             scroll(DOWN, S, y1-1, x1-1, y2-1, x2-1, bg << 4);
//
//             textbackground(bg);
//             textcolor(fg);
//             cprintf("\rBG:%s FG:%s", get_color(bg), get_color(fg));
//             //putch('\r');
//             //putch('Z');
//             delay(T*1000);
//         }
//     }


    getch();

    return 0;
}
