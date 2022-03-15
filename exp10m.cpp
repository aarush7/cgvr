#include<stdio.h>
#include<gl/glut.h>

struct point {
	float x, y;
};

void draw_polynomial(point vertices[], int len) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < len; i++) {
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

typedef struct point point;

point window[4], output_vertices[4];
int size, outlen = 0;

void define_window() {
	window[0].x = 30; window[0].y = 30;
	window[1].x = 30; window[1].y = 70;
	window[2].x = 70; window[2].y = 70;
	window[3].x = 70; window[3].y = 30;
}

bool inside_test(point v1, int i) {
	if (i == 0 && v1.x > window[0].x) return true;
	if (i == 1 && v1.y < window[1].y) return true;
	if (i == 2 && v1.x < window[2].x) return true;
	if (i == 3 && v1.y > window[3].y) return true;
	return false;
}

void add_output(point v1) {
	output_vertices[(outlen++) % size] = v1;
	printf("out_index: %d, out[1]: %f\n", outlen, output_vertices[0].x);
}

point intersection(point v1, point v2, int window_edge) {
	point out;
	if (window_edge == 0 || window_edge == 2) { // vertical
		out.x = window[window_edge].x;
		out.y = v2.y - ((v2.y - v1.y) / (v2.x - v1.x)) * (v2.x - out.x);
	}
	else {										// horizontal
		out.y = window[window_edge].y;
		out.x = v2.x - ((v2.x - v1.x) / (v2.y - v1.y)) * (v2.y - out.y);
	}
	return out;
}

void sutherland_hodgeman(point* input_vertices) {
	point v1, v2;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < size; j++) {
			v1 = input_vertices[j];
			v2 = input_vertices[(j + 1) % size];
			if (inside_test(v1, i)) {
				if (inside_test(v2, i)) { add_output(v2); printf("in-in\n"); }
				else { add_output(intersection(v1, v2, i)); printf("in-out\n"); }
			}
			else {
				if (inside_test(v2, i)) {
					add_output(intersection(v1, v2, i));
					add_output(v2);
					printf("out-in\n");
				}
				else { printf("out-out\n"); }
			}
		}
		glColor3f(0, 0, 1);
		draw_polynomial(output_vertices, size);
		input_vertices = output_vertices;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	define_window();
	glColor3d(1, 0, 0);
	point input_vertices[4];
	size = 4;
	input_vertices[0].x = 20; input_vertices[0].y = 40;
	input_vertices[1].x = 20; input_vertices[1].y = 60;
	input_vertices[2].x = 50; input_vertices[2].y = 60;
	input_vertices[3].x = 50; input_vertices[3].y = 40;
	glColor3d(0, 1, 0);
	draw_polynomial(window, 4);
	glColor3d(1, 0, 0);
	draw_polynomial(input_vertices, size);
	sutherland_hodgeman(input_vertices);

	glFlush();
}

void myInit() {
	gluOrtho2D(0, 100, 0, 100);
	glClearColor(0, 0, 0, 0);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE);
	glutCreateWindow("Sutherland Hodgeman");
	glutDisplayFunc(display);
	myInit();
	glutMainLoop();
}
