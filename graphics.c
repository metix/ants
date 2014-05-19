#include "ants.h"
#include <GL/glut.h>
#include <stdio.h>

#define WINDOW_HEIGHT	600
#define WINDOW_WIDTH	600

#define FIELD_SIZE_X	((float)WINDOW_HEIGHT/(float)field_height)
#define FIELD_SIZE_Y	((float)WINDOW_WIDTH/(float)field_width)

#define RGB_GRAY	0.5f, 0.5f, 0.5f
#define RGB_MOREGRAY	0.7f, 0.7f, 0.7f
#define RGB_GREEN	0.0f, 1.0f, 0.0f
#define RGB_LIGHTRED	0.9f, 0.5f, 0.5f
#define RGB_RED		0.9f, 0.0f, 0.0f
#define RGB_LIGHTGRAY	0.94f, 0.94f, 0.94f

float period = TICK_MS;
float steps_per_frame;
char debug = 1, stop;
float fps;

#define BUFF_SIZE	100
char text_buffer[BUFF_SIZE];

void draw_ants()
{
	int i;
	double x, y;

	for (i = 0; i < ants_count; i++) {
		switch (ants[i].state) {
		case ANT_STATE_WAIT:
			glColor3f(RGB_MOREGRAY);
			break;
		case ANT_STATE_SEARCH:
			glColor3f(RGB_GRAY);
			break;
		case ANT_STATE_FOOD:
			glColor3f(RGB_GREEN);
		}

		x = ants[i].position.x * FIELD_SIZE_X;
		y = ants[i].position.y * FIELD_SIZE_Y;

		glRectf(x, y, x + FIELD_SIZE_X, y + FIELD_SIZE_Y);
	}
}

void draw_food()
{
	int i;
	double x, y;

	for (i = 0; i < food_count; i++) {
		switch (food[i].state) {
		case FOOD_STATE_LYING:
			glColor3f(RGB_LIGHTRED);
			break;
		default:
			continue;
		}

		x = food[i].position.x * FIELD_SIZE_X;
		y = food[i].position.y * FIELD_SIZE_Y;

		glRectf(x, y, x + FIELD_SIZE_X, y + FIELD_SIZE_Y);
	}
}

void draw_grid()
{
	int i;
	glBegin(GL_LINES);
	glColor3f(RGB_LIGHTGRAY);
	for (i = 0; i < field_width; i++) {
		glVertex2d(i*FIELD_SIZE_X, 0);
		glVertex2d(i*FIELD_SIZE_Y, field_height * FIELD_SIZE_Y);
	}
	for (i = 0; i < field_height; i++) {
		glVertex2d(0, i * FIELD_SIZE_Y);
		glVertex2d(field_width*FIELD_SIZE_X, i * FIELD_SIZE_Y);
	}

	glEnd();
}

void draw_text(float x, float y, char *string)
{
	char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
	}
}

void calculate_fps()
{
	static int current_time, previous_time, frame_count;
	frame_count++;

	current_time = glutGet(GLUT_ELAPSED_TIME);

	int time_interval = current_time - previous_time;

	if (time_interval > 1000) {
		fps = frame_count / (time_interval / 1000.0f);

		previous_time = current_time;

		frame_count = 0;
	}
}

#define FONT_SIZE WINDOW_HEIGHT/60

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	draw_food();
	draw_ants();
	draw_grid();
	calculate_fps();
	if (debug) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (fps < (float) FPS * 0.8f)
			glColor3f(RGB_RED);
		snprintf(text_buffer, BUFF_SIZE, "fps: %f (%d)", fps, FPS);
		draw_text(1, 6 * FONT_SIZE, text_buffer);
		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

		snprintf(text_buffer, BUFF_SIZE, "steps per frame: %f",
			steps_per_frame);
		draw_text(1, 5 * FONT_SIZE, text_buffer);

		snprintf(text_buffer, BUFF_SIZE, "period: %f ms (%d Hz)", period
			, (int) (1.0f / (period / 1000.0f) + 0.5));
		draw_text(1, 4 * FONT_SIZE, text_buffer);

		snprintf(text_buffer, BUFF_SIZE, "food_count: %d", food_count);
		draw_text(1, 3 * FONT_SIZE, text_buffer);

		snprintf(text_buffer, BUFF_SIZE, "ants_count: %d", ants_count);
		draw_text(1, 2 * FONT_SIZE, text_buffer);

		snprintf(text_buffer, BUFF_SIZE, "field(width: %d, height: %d)",
			field_width, field_height);
		draw_text(1, 1 * FONT_SIZE, text_buffer);
	}

	glutSwapBuffers();
}

void simulate()
{
	static int current_time, previous_time;

	current_time = glutGet(GLUT_ELAPSED_TIME);

	int time_interval = current_time - previous_time;

	if (time_interval >= period && !stop) {
		int i;
		float mspf = (1.0f / FPS)*1000;

		steps_per_frame = (mspf / period);

		for (i = 0; i < (steps_per_frame > 0 ? steps_per_frame : 1); i++)
			next_step();
		previous_time = current_time;
	}
	render();
}

void keys_input(unsigned char key, int x, int y)
{
	keycontrol(key);
}

void init_graphics()
{
	int argc = 0;
	fps = 60;
	stop = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Emergent Ants");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glOrtho(0, WINDOW_HEIGHT, 0, WINDOW_WIDTH, 0, 128);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glutDisplayFunc(render);
	glutIdleFunc(simulate);
	glutKeyboardFunc(keys_input);

	glutMainLoop();
}
