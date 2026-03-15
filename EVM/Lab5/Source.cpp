#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>

void interrupt (*oldvect)(...);

clock_t  start_time;

void interrupt prev (...) {
	int r = rand() % ('Z' - 'A' + 1);
	clock_t  current_time;
	current_time = clock();
	double diff_seconds = (double)(current_time - start_time) / CLOCKS_PER_SEC;
	if (diff_seconds >= 0.5){
		putch(r + 'A');
		start_time = clock();
	}
	oldvect();
}

int main(void) {
	start_time = clock();
	randomize();
	oldvect = getvect(0x8); 
    setvect(0x8, prev);   
    getch();
    setvect(0x8, oldvect); 
	return 0;
}
