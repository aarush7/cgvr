#include<stdio.h>
#include<gl/glut.h>

int xwmin = 20, ywmin = 20;
int xwmax = 90, ywmax = 70;

int xvmin = 150, yvmin = 150;
int xvmax = 250, yvmax = 250;

void draw_line(double x0, double y0, double x1, double y1) {
	double x_operator = (xvmax - xvmin) / (xwmax - xwmin);
	double y_operator = (yvmax - yvmin) / (ywmax - ywmin);

	double xv0 = ((x0 - xwmin) * x_operator) + xvmin;
	double xv1 = ((x1 - xwmin) * x_operator) + xvmin;

	double yv0 = ((y0 - ywmin) * y_operator) + yvmin;
	double yv1 = ((y1 - ywmin) * y_operator) + yvmin;

	glColor3d(0, 0, 1);
	glVertex2f(xv0, yv0);
	glVertex2f(xv1, yv1);
	
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);

}


void liangBarsky(double x0, double y0, double x1, double y1) {
	double dx = x1 - x0, dy = y1 - y0;
	double p[] = { dx, -dx, dy, -dy };
	double q[] = {xwmax - x0, x0 - xwmin, ywmax - y0, y0 - ywmin};
	double u1 = 0,  u2 = 1, r;
	
	for (int i = 0; i < 4; i++) {
		
		if (p[i] == 0) {
			if (q[i] >= 0) draw_line(x0, y0, x1, y1); 
			return;
		}

		r = q[i] / p[i];
		if (p[i] > 0) if (r < u2) u2 = r;
		if (p[i] < 0) if (r > u1) u1 = r;
	}

	if (u1 > u2) return;

	double x0_clipped = x0 + u1 * dx;
	double y0_clipped = y0 + u1 * dy;

	double x1_clipped = x0 + u2 * dx;
	double y1_clipped = y0 + u2 * dy;

	draw_line(x0_clipped, y0_clipped, x1_clipped, y1_clipped);
}

void display() {
	double x0 = 10, y0 = 30;
	double x1 = 80, y1 = 90;
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3d(1, 0, 0);

	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(xvmin, yvmin);
	glVertex2f(xvmin, yvmax);
	glVertex2f(xvmax, yvmax);
	glVertex2f(xvmax, yvmin);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(xwmin, ywmin);
	glVertex2f(xwmin, ywmax);
	glVertex2f(xwmax, ywmax);
	glVertex2f(xwmax, ywmin);
	glEnd();

	glBegin(GL_LINES);
		liangBarsky(x0, y0, x1, y1);
	glEnd();
	glFlush();
}

void myInit() {
	glClearColor(1, 1, 1, 1);
	gluOrtho2D(0, 300, 0, 300);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Liang Barsky");
	glutDisplayFunc(display);
	myInit();
	glutMainLoop();
}
