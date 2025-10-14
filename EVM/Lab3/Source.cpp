#include <graphics.h>
#include <conio.h>
#include <stdio.h>

int main(void) {
	int graph_driver = DETECT;
	int graph_mode;
	int graph_error_code;
	initgraph(&graph_driver, &graph_mode, "C:\\TURBOC3\\BGI"); 
	graph_error_code = graphresult(); if( graph_error_code != grOk) {closegraph(); return graph_error_code;}
	
	int x = getmaxx();
	int y = getmaxy();
	
	viewporttype Win1;
	setviewport(5, 5, x-5, y/3*2, 1);
	getviewsettings(&Win1);
	
	viewporttype Win2;
	setviewport(5, y/3*2, x-5, y-5, 1);
	getviewsettings(&Win1);
	
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	graph_error_code = graphresult(); if( graph_error_code != grOk) {closegraph(); return graph_error_code;}
	
	setbkcolor(BLUE);
	setcolor(WHITE);
	
	outtext("sin^3(x)*cos^2(x)");
	
	
	
	getch();
	
	closegraph();
	return 0;
}
