#include "ants.h"
#include <GL/glut.h>
#include <stdio.h>

#define RGB_GRAY	0.5f, 0.5f, 0.5f
#define RGB_MOREGRAY	0.7f, 0.7f, 0.7f
#define RGB_GREEN	0.0f, 1.0f, 0.0f
#define RGB_LIGHTRED	0.9f, 0.5f, 0.5f
#define RGB_RED		0.9f, 0.0f, 0.0f
#define RGB_LIGHTGRAY	0.94f, 0.94f, 0.94f

float period = TICK_MS;
float steps_per_frame;
char debug = 1, stop;
float fps, font_height, field_size;
int win_height, win_width;

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

		x = ants[i].position.x * field_size;
		y = ants[i].position.y * field_size;

		glRectf(x, y, x + field_size, y + field_size);
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

		x = food[i].position.x * field_size;
		y = food[i].position.y * field_size;

		glRectf(x, y, x + field_size, y + field_size);
	}
}

void draw_grid()
{
	int i;
	glBegin(GL_LINES);
	glColor3f(RGB_LIGHTGRAY);
	for (i = 0; i < field_count_x + 1; i++) {
		glVertex2d(i*field_size, 0);
		glVertex2d(i*field_size, field_count_y * field_size);
	}
	for (i = 0; i < field_count_y + 1; i++) {
		glVertex2d(0, i * field_size);
		glVertex2d(field_count_x*field_size, i * field_size);
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

void draw_debug()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	if (fps < (float) FPS * 0.8f)
		glColor3f(RGB_RED);
	snprintf(text_buffer, BUFF_SIZE, "fps: %f (%d)", fps, FPS);
	draw_text(1, 6 * font_height, text_buffer);
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

	snprintf(text_buffer, BUFF_SIZE, "steps per frame: %f",
		steps_per_frame);
	draw_text(1, 5 * font_height, text_buffer);

	snprintf(text_buffer, BUFF_SIZE, "interval: %f ms (%.2f Hz)", period
		, (float) (1.0f / (period / 1000.0f)));
	draw_text(1, 4 * font_height, text_buffer);

	snprintf(text_buffer, BUFF_SIZE, "food_count: %d", food_count);
	draw_text(1, 3 * font_height, text_buffer);

	snprintf(text_buffer, BUFF_SIZE, "ants_count: %d", ants_count);
	draw_text(1, 2 * font_height, text_buffer);

	snprintf(text_buffer, BUFF_SIZE, "field(width: %d, height: %d)",
		field_count_x, field_count_y);
	draw_text(1, 1 * font_height, text_buffer);
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

void calculate_field_size()
{
	// calculate size of one field
	if ((win_width / field_count_x) > win_height / field_count_y)
		field_size = (float) win_height / (float) field_count_y;
	else
		field_size = (float) win_width / (float) field_count_x;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);

	calculate_field_size();

	draw_food();
	draw_ants();
	//draw_grid();
	calculate_fps();
	if (debug) {
		draw_debug();
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	win_width = w;
	win_height = h;

	calculate_field_size();

	float field_width = (field_size * field_count_x);
	float field_height = (field_size * field_count_y);
	float margin_left = 0, margin_bottom = 0;

	margin_left = (w - field_width) / 2;
	margin_bottom = (h - field_height) / 2;

	glViewport(margin_left, margin_bottom, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
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
	// maybe some graphical values have changed
	reshape(win_width, win_height);
}

void init_graphics()
{
	int argc = 0;
	fps = 60;
	stop = 0;
	font_height = 10.0f;

	win_width = WINDOW_WIDTH;
	win_height = WINDOW_HEIGHT;

	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("Emergent Ants");
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, win_width, win_height);
	gluOrtho2D(0, win_width, 0, win_height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// dont draw backface of polygons
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glutDisplayFunc(render);
	glutIdleFunc(simulate);
	glutKeyboardFunc(keys_input);
	glutReshapeFunc(reshape);
	glutMainLoop();
}