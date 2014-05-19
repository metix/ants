#include "ants.h"
#include <stdio.h>
#include <stdlib.h>

#define KEYCODE_ENTER	13
#define BUFF_SIZE	20

int state = 0;
char buffer[BUFF_SIZE];
int bi = 0;

int write_buffer(char key)
{
	if (bi >= BUFF_SIZE - 1) {
		return 1;
	}
	buffer[bi++] = key;
	return 0;
}

void halt()
{
	stop = 1;
}

void play()
{
	stop = 0;
}

void short_key(char key)
{
	switch (key) {
	case 'd':
		if (debug)
			debug = 0;
		else
			debug = 1;
		break;
	case 's':
		period += 10;
		break;
	case 'w':
		if ((period - 10) < 0 || (period - 10) == 0)
			period = 1;
		else
			period -= 10;
		break;
	case 'q':
		exit(0);
		break;
	case 'e':
		period = 0.1;
		break;
	case 'r':
		init_ants();
		period = TICK_MS;
		break;
	case 'h':
		if (!stop)
			halt();
		else
			play();
		break;
	case 'p':
		play();
		break;
	}
}

void keycontrol(char key)
{
	switch (state) {
	case 0:
		play();
		switch (key) {
		case 'c': state = 1;
			halt();
			break;
		default:
			short_key(key);
			break;
		}
		break;
	case 1:
		bi = 0;
		switch (key) {
		case 'a': state = 2;
			break;
		case 'f': state = 4;
			break;
		case 's': state = 6;
			break;
		case 'p': state = 9;
			break;
		default: state = 0;
			play();
			break;
		}
		break;
	case 2:
		switch (key) {
		case 'n': state = 3;
			break;
		default:
			state = 0;
			break;
		}
		break;
	case 3:
		switch (key) {
		case KEYCODE_ENTER:
		{

			buffer[bi] = 0;
			int tmp;
			if (sscanf(buffer, "%d", &tmp) == 1) {
				ants_count = tmp;
				init_ants();
				printf("ants_count: %d\n", tmp);
			}
			state = 1;
		}
			break;
		default:
			if (write_buffer(key))
				state = 0;
			break;
		}
		break;
	case 4:
		switch (key) {
		case 'n': state = 5;
			break;
		default:
			state = 0;
			break;
		}
		break;
	case 5:
		switch (key) {
		case KEYCODE_ENTER:
		{

			buffer[bi] = 0;
			int tmp;
			if (sscanf(buffer, "%d", &tmp) == 1) {
				food_count = tmp;
				init_ants();
				printf("food_count: %d\n", tmp);
			}
			state = 1;
		}
			break;
		default:
			if (write_buffer(key))
				state = 0;
			break;
		}
		break;
	case 6:
		switch (key) {
		case 'x': state = 7;
			break;
		case 'y': state = 8;
			break;
		default:
			state = 0;
			break;
		}
		break;
	case 7:
		switch (key) {
		case KEYCODE_ENTER:
		{
			buffer[bi] = 0;
			int tmp;
			if (sscanf(buffer, "%d", &tmp) == 1) {
				field_width = tmp;
				init_ants();
				printf("field_width: %d\n", tmp);
			}
			state = 1;
		}
			break;
		default:
			if (write_buffer(key))
				state = 0;
			break;
		}
		break;
	case 8:
		switch (key) {
		case KEYCODE_ENTER:
		{

			buffer[bi] = 0;
			int tmp;
			if (sscanf(buffer, "%d", &tmp) == 1) {
				field_height = tmp;
				init_ants();
				printf("field_height: %d\n", tmp);
			}
			state = 1;
		}
			break;
		default:
			if (write_buffer(key))
				state = 0;
			break;
		}
		break;
	case 9:
		switch (key) {
		case KEYCODE_ENTER:
		{

			buffer[bi] = 0;
			float tmp;
			if (sscanf(buffer, "%f", &tmp) == 1) {
				if (tmp != 0 && tmp > 0) {
					period = tmp;
					printf("period: %f\n", tmp);
				}
			}
			state = 0;
			play();
		}
			break;
		default:
			if (write_buffer(key))
				state = 0;
			break;
		}
		break;
	}
}

void init_keycontrol()
{
	state = 0;
	bi = 0;
}