#include <dos.h>
#include <conio.h>



enum { ENTIRE, UP, DOWN };

void scroll(int direction, char lines, char l_row, char l_col, char r_row, char r_col, char attr) {
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

void hide_cursor() {
    union REGS r;
    r.h.ah = 0x01;
    r.h.ch = 1<<5;
    int86(0x10, &r, &r);
}

int get_key(){
    union REGS r;
    r.h.ah = 0x00;
    int86(0x16, &r, &r);
    if  (r.h.al == 0)
        return r.h.ah;
    else return r.h.al;
}

int is_key(){
    union REGS r;
    r.h.ah = 0x01;
    int86(0x16, &r, &r);
    return !(r.x.flags & 1<<6);
}

enum {UP_MOVE,DOWN_MOVE,STOP_MOVE};

int main(){
    int x1 = 10, y1 = 12, x2 = 70, y2 = 23;
    clrscr();
    scroll(ENTIRE, 0, y1-2, x1-2, y2, x2, 15 << 4);
    scroll(ENTIRE, 0, y1-1, x1-1, y2-1, x2-1, 1 << 4);
    window(x1, y1, x2, y2);
    textbackground(1);
    textcolor(15);
    hide_cursor();
    int now_x = (x2-x1)/2, now_y = (y2-y1)/2;
    int max_x = x2-x1, max_y = y2-y1;
    int current_move = STOP_MOVE;
    int key = 0;
    while(1){
        if(is_key()){
            key = get_key();
            if (key == 80)
                current_move = DOWN_MOVE;
            else if (key == 72)
                current_move = UP_MOVE;
        }
        switch(current_move){
            case UP_MOVE:
                if (now_y - 1 > 0) now_y--;
                break;
            case DOWN_MOVE:
                if (now_y - 1 < max_y) now_y++;
                break;
        }
        if (key == 3) break;
        clrscr();
        gotoxy(now_x,now_y);
        putch('*');
        delay(25);
    }
    return 0;
}
