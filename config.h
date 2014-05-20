#ifndef __CONFIG_H__
#define	__CONFIG_H__
/***************************** CONFIG *****************************************/
#define ANTS_COUNT	50	/* how many ants */
#define FOOD_COUNT	200	/* how much food */
#define WAIT_TIME	5	/* how long wait an ant before it 
					searches for new food */

#define FIELD_WIDTH	50	/* width of the simulation field */
#define FIELD_HEIGHT	50	/* height of the simulation field */

#define TICK_MS		500	/* how long takes simulation step */
#define FPS		60	/* stable fps of the monitor */

#define WINDOW_WIDTH	640	/* width of the window */
#define WINDOW_HEIGHT	480	/* height of the window */
/******************************************************************************/
/* keycontrol - controlling the simulation		| state 0
 * (c) config						| state 1
 *	(a) ants					| state 2
 *		(n) count				| state 3
 *			(number number of ants)
 *			(ENTER) -> return to state 1
 *	(f) food					| state 4
 *		(n) count				| state 5
 *			(number number of food)
 *			(ENTER) -> return to state 1
 *	(s) size of field				| state 6
 *		(x) width				| state 7
 *			(width)				
 *			(ENTER) -> return to state 1
 *		(y) height				| state 8
 *			(height)
 *			(ENTER) -> return to state 1
 *	(p) period					| state 9
 *		(period)
 *		(ENTER) -> return to state 0
 * 
 * (w) faster -10ms
 * (s) slower +10ms
 * (e) very fast
 * (q) quit
 * (h) halt on/off
 * (p) play
 * (r) reset
 * (d) debug on/off
 */

#endif

