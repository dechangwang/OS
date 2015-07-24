/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            keyboard.c
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
                            keyboard_handler
 *======================================================================*/
PUBLIC void keyboard_handler(int irq)
{
	/* disp_str("*"); */
	u8 scan_code = in_byte(0x60);
	//disp_int(scan_code);
	if (snake_direct != DIRECT_NULL){

		int Head2 = snake_head - 1;
		if (Head2 < 0) Head2 = MAP_SIZE - 1;
		
		int x = snake_path[snake_head] - snake_path[Head2];
		if (x < 0) x = -1 * x;

		if (scan_code == 0x48 && x == 1){
			snake_direct = DIRECT_UP;
		}
		else if (scan_code == 0x50 && x == 1){
			snake_direct = DIRECT_DOWN;
		}
		else if (scan_code == 0x4B && x > 1){
			snake_direct = DIRECT_LEFT;
		}
		else if (scan_code == 0x4D && x > 1){
			snake_direct = DIRECT_RIGHT;
		}
	}
	if (scan_code == 0x13){
		Reset();
	}
}


/*======================================================================*
                           init_keyboard
*======================================================================*/
PUBLIC void init_keyboard()
{
        put_irq_handler(KEYBOARD_IRQ, keyboard_handler);/*设定键盘中断处理程序*/
        enable_irq(KEYBOARD_IRQ);                       /*开键盘中断*/
}

