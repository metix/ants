#ifndef __ANTS_H__
#define	__ANTS_H__
#include "config.h"

#define ANT_STATE_SEARCH	1
#define ANT_STATE_FOOD		2
#define ANT_STATE_WAIT		3

#define FOOD_STATE_LYING	1
#define FOOD_STATE_BUSY		2

typedef struct point_s {
	int x;
	int y;
} point_t;

typedef struct food_s {
	point_t position;
	int state;
} food_t;

typedef struct brain_s {
	int wait_for_next_food;
} brain_t;

typedef struct ant_s {
	point_t position;
	point_t destination;
	int state;
	brain_t brain;
	food_t *inventar;
} ant_t;

/* see ants.c */
extern void init_ants();
extern void next_step();

extern ant_t *ants;
extern food_t *food;
extern int ants_count;
extern int food_count;
extern int field_width;
extern int field_height;

/* see graphics.c */
extern void init_graphics();
extern void draw_field();

extern float period;
extern char stop;
extern char debug;

/* see control.c */
extern void init_keycontrol();
extern void keycontrol(char key);

#endif

