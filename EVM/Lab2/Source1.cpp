#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

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




int main(void)
{
    int x1 = 25, y1 = 10, x2 = 55, y2 = 20;
    double T = 0.2;
    int S = 3;

    clrscr();
    window(x1, y1, x2, y2);

    //scroll(ENTIRE, 0, y1-1, x1-1, y2+1, x2+1, get_color_attr(15, 15));




    for(int fg = 0; fg < 16; fg++) {
        for(int bg = 0; bg < 16; bg++) {
            if(fg == bg) continue;
            scroll(ENTIRE, 0, y1-2, x1-2, y2, x2, fg << 4);

            scroll(DOWN, 0, y1-1, x1-1, y2-1, x2-1, bg << 4);

            textbackground(bg);
            textcolor(fg);
            cprintf("\rBG:%2d FG:%2d", bg, fg);
            delay(T*360);
        }
    }


    getch();

    return 0;
}
