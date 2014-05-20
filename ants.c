#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "ants.h"
#include <stdio.h>
#include <string.h>

int ants_count = ANTS_COUNT;
int food_count = FOOD_COUNT;

int field_count_x = FIELD_WIDTH;
int field_count_y = FIELD_HEIGHT;

ant_t *ants = NULL;
food_t *food = NULL;

#define FIELD_EMPTY	0
#define FIELD_ANT	(1<<0)
#define FIELD_FOOD	(1<<1)

typedef struct field_info_s {
	ant_t *ant;
	food_t *food;
} field_t;

field_t **fast_field;

#define DELTA_FIELDS	9
point_t delta_pos[DELTA_FIELDS];

point_t random_pos()
{
	point_t p;
	p.x = rand() % field_count_x;
	p.y = rand() % field_count_y;
	return p;
}

point_t random_destination()
{
	int side = rand() % 4;
	point_t dest;

	switch (side) {
	case 0:
		dest.x = -1;
		dest.y = rand() % field_count_y;
		break;
	case 1:
		dest.x = field_count_x;
		dest.y = rand() % field_count_y;
		break;
	case 2:
		dest.x = rand() % field_count_x;
		dest.y = -1;
		break;
	case 3:
		dest.x = rand() % field_count_x;
		dest.y = field_count_y;
		break;
	}
	return dest;
}

ant_t *check_ant(point_t pos)
{
	if (fast_field[pos.x][pos.y].ant != NULL)
		return fast_field[pos.x][pos.y].ant;
	return NULL;
}

food_t *check_food(point_t pos)
{
	if (fast_field[pos.x][pos.y].food != NULL)
		return fast_field[pos.x][pos.y].food;
	return NULL;
}

int find_free(point_t pos, point_t *free)
{
	int cnt;
	point_t p;
	int i = rand() % DELTA_FIELDS;

	for (cnt = 0; cnt < DELTA_FIELDS; cnt++) {
		if (i >= DELTA_FIELDS) {
			i = 0;
		}

		p.x = pos.x + delta_pos[i].x;
		p.y = pos.y + delta_pos[i].y;

		if (p.x < 0 || p.y < 0 || p.x >= field_count_x || p.y >= field_count_y) {
			i++;
			continue;
		}

		if (check_food(p) == NULL) {
			*free = p;
			return 0;
		}

		i++;
	}
	return -1;
}

void test(int x, int y)
{
	point_t pos = {.x = x, .y = y};
	int i;
	for (i = 0; i < ants_count; i++)
		ants[i].destination = pos;
}

void move_ant(ant_t *ant)
{
	food_t *f;
	point_t next;

	if (ant->state == ANT_STATE_WAIT)
		if (ant->brain.wait_for_next_food-- <= 0)
			ant->state = ANT_STATE_SEARCH;

	if (ant->state == ANT_STATE_SEARCH &&
		((f = check_food(ant->position)))) {
		f->state = FOOD_STATE_BUSY;
		fast_field[ant->position.x][ant->position.y].food = NULL;
		ant->inventar = f;
		ant->state = ANT_STATE_FOOD;
		ant->destination = random_destination();
	}

	if (ant->state == ANT_STATE_FOOD && check_food(ant->position)) {
		point_t free;
		if (find_free(ant->position, &free) == -1) {
			// ant is caged :D
			return;
		}
		f = ant->inventar;
		f->position = free;
		fast_field[free.x][free.y].food = f;
		f->state = FOOD_STATE_LYING;
		ant->brain.wait_for_next_food = WAIT_TIME;
		ant->state = ANT_STATE_WAIT;
		ant->destination = random_destination();
		return;
	}

new_position:
	next = ant->position;

	if (ant->destination.x > ant->position.x)
		next.x++;
	else if (ant->destination.x < ant->position.x)
		next.x--;

	if (ant->destination.y > ant->position.y)
		next.y++;
	else if (ant->destination.y < ant->position.y)
		next.y--;

	if (next.y >= field_count_y || next.x >= field_count_x ||
		next.y < 0 || next.x < 0) {
		ant->destination = random_destination();
		goto new_position;
	}

	// check if next position is already taken by an ant
	if (check_ant(next)) {
		ant->destination = random_destination();
		return;
	}
	fast_field[ant->position.x][ant->position.y].ant = NULL;
	fast_field[next.x][next.y].ant = ant;
	ant->position = next;
}

void next_step()
{
	int i;
	for (i = 0; i < ants_count; i++) {
		move_ant(&ants[i]);
	}
}

void init_ants()
{
	srand(clock());

	int i, b, n;

	if (ants != NULL)
		free(ants);
	if (food != NULL)
		free(food);
	if (fast_field != NULL)
		free(fast_field);

	// allocate dynamic stuff
	ants = malloc(sizeof(ant_t) * ants_count);
	food = malloc(sizeof(food_t) * food_count);

	fast_field = malloc(sizeof(int*) * field_count_x);

	for (i = 0; i < field_count_x; i++)
		fast_field[i] = malloc(sizeof(field_t) * field_count_y);

	// initialize delta-position array used for random food placement
	i = 0;
	for (b = -1; b <= 1; b++) {
		for (n = -1; n <= 1; n++) {
			point_t p = {.x = b, .y = n};
			delta_pos[i++] = p;
		}
	}

	// init fast_access field
	for (i = 0; i < field_count_x; i++) {
		for (b = 0; b < field_count_y; b++) {
			fast_field[i][b].ant = NULL;
			fast_field[i][b].food = NULL;
		}
	}

	// init ants
	for (i = 0; i < ants_count; i++) {
		ant_t new_ant = {
			.destination = random_destination(),
			.state = ANT_STATE_SEARCH,
			.brain.wait_for_next_food = 0
		};

		ants[i] = new_ant;
		n = 0;

new_ant_position:

		if (n++ == field_count_x * field_count_y) {
			printf("no place for ants :(\n");
			exit(1);
		}
		ants[i].position = random_pos();
		if (fast_field[ants[i].position.x][ants[i].position.y].ant)
			goto new_ant_position;
		fast_field[ants[i].position.x][ants[i].position.y].ant = &ants[i];
	}

	// init food
	for (i = 0; i < food_count; i++) {
		food[i].state = FOOD_STATE_LYING;
		n = 0;
new_food_pos:

		if (n++ == field_count_x * field_count_y) {
			printf("no place for food :(\n");
			exit(1);
		}
		food[i].position = random_pos();
		if (fast_field[food[i].position.x][food[i].position.y].food)
			goto new_food_pos;
		fast_field[food[i].position.x][food[i].position.y].food = &food[i];
	}
}
