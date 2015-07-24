
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;
	int i;
	for (i = 0; i < NR_TASKS; i++) {
		strcpy(p_proc->p_name, p_task->name);	// name of the process
		p_proc->pid = i;			// pid

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
		p_proc->regs.cs	= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.ds	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.es	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.fs	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.ss	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
			| SA_TIL | RPL_TASK;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK)
			| RPL_TASK;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	proc_table[0].ticks = proc_table[0].priority = 15;
	//proc_table[1].ticks = proc_table[1].priority = 10;
	//proc_table[2].ticks = proc_table[2].priority =  3;

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
    init_keyboard();

	restart();

	while(1){}
}

/*======================================================================*
                               Start
 *======================================================================*/
void Start()
{
	/*int ai;

	disp_pos = 0;

	for (ai = 0; ai < 80 * 25; ai++)
	{
		disp_str(" ");
	}

	disp_pos = 0;*/

	//memset(snake_map, 0, sizeof(char)*MAP_SIZE);

	/*int ai;

	for (ai = 0; ai < 80 * 25; ai++)
	{
		snake_map[ai] = ' ';
	}*/

	Reset();

	while (1){
		//proc_table[0].ticks = proc_table[0].priority = 150;
		milli_delay(3333);

		int hX, hY;
		int newHead;

		if (snake_direct == DIRECT_NULL) continue;

		hX = snake_path[snake_head] % MAP_LENGTH;
		hY = snake_path[snake_head] / MAP_LENGTH;
		switch (snake_direct)
		{
		case DIRECT_UP:
		{
			hY--;
			break;
		}
		case DIRECT_DOWN:{
			hY++;
			break;
		}
		case DIRECT_LEFT:{
			hX--;
			break;
		}
		case DIRECT_RIGHT:{
			hX++;
			break;
		}
		default:
			break;
		}
		
		newHead = hY*MAP_LENGTH + hX;

		if (newHead < 0 || newHead >= MAP_SIZE || hY >= MAP_HEIGHT || hY < 0 || hX < 0 || hX >= MAP_LENGTH){
			Lose();
			continue;
		}

		if (snake_map[newHead] == 'O' && newHead != snake_path[snake_tail]){
			Lose();
			continue;
		}

		if (snake_map[newHead] != '*'){
			disp_pos = snake_path[snake_tail] * 2;
			disp_str(" ");
			snake_map[snake_path[snake_tail]] = ' ';

			snake_tail++;
			if (snake_tail == MAP_SIZE) snake_tail = 0;
		}
		else{
			snake_length++;
		}

		disp_pos = snake_path[snake_head] * 2;
		disp_str("0");
		snake_map[snake_path[snake_head]] = 'O';
		
		snake_head++;
		if (snake_head == MAP_SIZE) snake_head = 0;
		snake_path[snake_head] = newHead;
		
		disp_pos = snake_path[snake_head] * 2;
		disp_str("@");
		snake_map[snake_path[snake_head]] = '@';

		if (snake_length == 6) {
			Win();
			continue;
		}
	}
}

void Reset(){
	int bi;

	for (bi = 0; bi < MAP_SIZE; bi++)
	{
		snake_map[bi] = ' ';
		if (bi % (MAP_LENGTH / 2) == 20){
			if (bi / MAP_LENGTH == 2 || bi / MAP_LENGTH == 24){
				snake_map[bi] = '*';
ii33333333			}
		}
	}

	7

	disp_pos = 0;
	disp_str(snake_map);

	snake_length = 2;
	snake_head = 1;
	snake_tail = 0;
	snake_path[snake_head] = 1;
	snake_path[snake_tail] = 0;
	snake_direct = DIRECT_RIGHT;

	disp_pos = snake_path[snake_tail] * 2;
	disp_str("O");
	disp_pos = snake_path[snake_head] * 2;
	disp_str("@");
	
	/*snake_killer1 = MAP_SIZE - 1;
	snake_killer2 = MAP_SIZE - 1;

	disp_pos = snake_killer1 * 2;
	disp_str("#");
	snake_map[snake_killer1] = '#';
	disp_pos = snake_killer2 * 2;
	disp_str("#");
	snake_map[snake_killer2] = '#';*/
}

void Lose(){
	disp_pos = (MAP_SIZE / 2 - 5) * 2;
	disp_str("You Lose");

	snake_direct = DIRECT_NULL;
}

void Win(){
	disp_pos = (MAP_SIZE / 2 - 5) * 2;
	disp_str("You Win");

	snake_direct = DIRECT_NULL;
}

/*
void SnakeKiller(){
	while (1){
		milli_delay(2000);

		if (snake_direct == DIRECT_NULL) continue;

		//int x, y, dx, dy, nx, ny, ax, ay;
		
		x = snake_killer1 % MAP_LENGTH;
		y = snake_killer1 / MAP_HEIGHT;

		ax = snake_path[snake_head] % MAP_LENGTH;
		ay = snake_path[snake_head] / MAP_HEIGHT;

		dx = (x > ax ? x - ax: ax - x);
		dy = (y > ay ? y - ay: ay - y);

		nx = x;
		ny = y;

		if (dx > dy){
			if (x < ax){
				nx++;
			}
			else{
				nx--;
			}
		}
		else{
			if (y < ay){
				ny++;
			}
			else{
				ny--;
			}
		}

		disp_pos = snake_killer1 * 2;
		disp_str(" ");
		snake_map[snake_killer1] = ' ';

		snake_killer1 = nx + ny*MAP_LENGTH;
		disp_pos = snake_killer1 * 2;
		disp_str("#");

		x = snake_killer2 % MAP_LENGTH;
		y = snake_killer2 / MAP_HEIGHT;

		ax = snake_path[snake_tail] % MAP_LENGTH;
		ay = snake_path[snake_tail] / MAP_HEIGHT;

		dx = (x > ax ? x - ax: ax - x);
		dy = (y > ay ? y - ay: ay - y);

		nx = x;
		ny = y;

		if (dx >= dy){
			if (x < ax){
				nx++;
			}
			else{
				nx--;
			}
		}
		else{
			if (y < ay){
				ny++;
			}
			else{
				ny--;
			}
		}
		disp_pos = 0;
		disp_int(nx);
		disp_pos = 80;
		disp_int(ny);

		disp_pos = snake_killer2 * 2;
		disp_str(" ");
		snake_map[snake_killer2] = ' ';

		snake_killer2 = nx + ny*MAP_LENGTH;
		disp_pos = snake_killer2 * 2;
		disp_str("#");

		if (snake_map[snake_killer1] == '@' || snake_map[snake_killer1] == 'O'
			|| snake_map[snake_killer2] == '@' || snake_map[snake_killer2] == 'O'){
			Lose();
		}

		snake_map[snake_killer1] = '#';
		snake_map[snake_killer2] = '#';
	}
}*/