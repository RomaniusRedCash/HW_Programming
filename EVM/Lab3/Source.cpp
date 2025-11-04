#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>

double Max = 0, Min = 0;
char buffer[50];


double fun (double x){
	return pow(sin(x), 2)-pow(cos(x), 3);
}

void Draw(const viewporttype& W, double cfrom, double cto){

	int width = W.right - W.left;
	int height = W.bottom - W.top;
	double CenterY = height / 2;

	double* A = new double[width];
	double CpPX = (cto - cfrom) / width;
	double Pi = (double) width / (cto - cfrom) * M_PI;

	int i = 0;
	for(i = 0; i < width; i++) {
		A[i] = fun(cfrom + CpPX * i);
		if (A[i] < Min) Min = A[i];
		if (A[i] > Max) Max = A[i];
	}
	double k = 0; i = 0;

	settextstyle(0, VERT_DIR, 1);


	for (k=0; k < width; k+=Pi){
		sprintf(buffer, "%d%s", ++i, "pi/2");
		outtextxy(k + W.left + 10, CenterY-50, buffer);
		line(k + W.left, 0, k + W.left, W.bottom);
	}

	double CpPY = (height - 100)/ (Max - Min);

	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	for(i = 0; i < width - 1; i++) {
		double y1 = CenterY - A[i] * CpPY;
		double y2 = CenterY - A[i + 1] * CpPY;
		double x1 = W.left + i;
		double x2 = W.left + i + 1;

		line(x1, y1, x2, y2);
		delay(5);
		// putpixel(x1,y1,WHITE);
	}
	setlinestyle(DASHED_LINE, 0, NORM_WIDTH);


	settextstyle(0, HORIZ_DIR, 1);


	sprintf(buffer, "%.2f", Max);
	line(W.left, CenterY - Max*CpPY, W.right, CenterY - Max*CpPY);
	outtextxy(W.left, CenterY - Max*CpPY, buffer);

	sprintf(buffer, "%.2f", Min);
	line(W.left, CenterY - Min*CpPY, W.right, CenterY - Min*CpPY);
	outtextxy(W.left, CenterY - Min*CpPY, buffer);


	delete [] A;
}





int main(void) {
	int graph_driver = DETECT;
	int graph_mode;
	int graph_error_code;
	initgraph(&graph_driver, &graph_mode, "C:\\BGI");
	graph_error_code = graphresult(); if( graph_error_code != grOk) {closegraph(); return graph_error_code;}

	int x = getmaxx();
	int y = getmaxy();



	setbkcolor(BLUE);
	setcolor(WHITE);

	viewporttype Win1;
	setviewport(5, 5, x-5, y/3*2, 1);
	getviewsettings(&Win1);

	int CenterY = Win1.bottom/2, CenterX = Win1.right/2;


	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);


	setlinestyle(DASHED_LINE, 0, NORM_WIDTH);
	line(0, Win1.bottom/2, Win1.right, Win1.bottom/2);
	outtextxy(Win1.right-15, CenterY-15, ">");
	outtextxy(Win1.right-15, CenterY+15, "X");



	line(5, 0, 5, Win1.bottom);
	outtextxy(0, 0, "^");
	outtextxy(10, 10, "Y");



	settextstyle(0, HORIZ_DIR, 1);
	outtextxy(0,CenterY+10, "pi/2");
	outtextxy(Win1.right-30, CenterY+10, "5pi");
	Draw(Win1, M_PI/2, 5*M_PI);





	viewporttype Win2;
	setviewport(5, y/3*2, x-5, y-5, 1);
	getviewsettings(&Win1);

	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	graph_error_code = graphresult(); if( graph_error_code != grOk) {closegraph(); return graph_error_code;}


	outtextxy(10, 10, "y = sin^2(x)-cos^3(x)");

	sprintf(buffer, "%s%.2f%s%.2f", "Max = ", Max, "Min = ", Min);

	outtextxy(10,40,buffer);


	getch();

	closegraph();
	return 0;
}
