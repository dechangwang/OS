
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               tty.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

#define TTY_FIRST	(tty_table)
#define TTY_END		(tty_table + NR_CONSOLES)

PRIVATE void init_tty(TTY* p_tty);
PRIVATE void tty_do_read(TTY* p_tty);
PRIVATE void tty_do_write(TTY* p_tty);
PRIVATE void put_key(TTY* p_tty, u32 key);
PRIVATE int num = 0;
PUBLIC  char INPUT[0xE];//原先是想用数组 最初为 PUBLIC  char INPUT[10]
PUBLIC char INPUT_a='0',INPUT_b='0',INPUT_c='0',INPUT_d='0',INPUT_e='0',INPUT_f='0',
INPUT_g='0',INPUT_h='0',INPUT_i='0';//全局变量用来存输入的值 
PUBLIC extern int SUM =0 ;

/*======================================================================*
                           task_tty
 *======================================================================*/
PUBLIC void task_tty()
{

	TTY*	p_tty;

	init_keyboard();

	for (p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++) {
		init_tty(p_tty);

	}
	select_console(0);
	while (1) {
		for (p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++) {
			tty_do_read(p_tty);
			tty_do_write(p_tty);
num++;
		}
	}
}

/*======================================================================*
			   init_tty
 *======================================================================*/
PRIVATE void init_tty(TTY* p_tty)
{

	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

	init_screen(p_tty);
}

/*======================================================================*
				in_process
 *======================================================================*/
PUBLIC void in_process(TTY* p_tty, u32 key)
{

        char output[2] = {'\0', '\0'};

        if (!(key & FLAG_EXT)) {
		put_key(p_tty, key);
        }
        else {
                int raw_code = key & MASK_RAW;
                switch(raw_code) {
                case ENTER:
			put_key(p_tty, '\n');
			break;
                case BACKSPACE:
			put_key(p_tty, '\b');
			break;
                case UP:
                        if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				scroll_screen(p_tty->p_console, SCR_DN);
                        }
			break;
		case DOWN:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				scroll_screen(p_tty->p_console, SCR_UP);
			}
			break;
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case F11:
		case F12:
			/* Alt + F1~F12 */
			if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R)) {
				select_console(raw_code - F1);
			}
			break;
                default:
                        break;
                }
        }
}

/*======================================================================*
			      put_key
*======================================================================*/
PRIVATE void put_key(TTY* p_tty, u32 key)
{

	if (p_tty->inbuf_count < TTY_IN_BYTES) {
		*(p_tty->p_inbuf_head) = key;
		p_tty->p_inbuf_head++;
		if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_head = p_tty->in_buf;
		}
		p_tty->inbuf_count++;
	}
}


/*======================================================================*
			      tty_do_read
 *======================================================================*/
PRIVATE void tty_do_read(TTY* p_tty)
{
	if (is_current_console(p_tty->p_console)) {
		keyboard_read(p_tty);
	}
}


/*======================================================================*
			      tty_do_write
 *======================================================================*/
PRIVATE void tty_do_write(TTY* p_tty)
{

int num = 0;
	if (p_tty->inbuf_count) {
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;

if(SUM % 3 == 0)out_char(p_tty->p_console, '\n');
/*disp_int(SUM % 5);*/
/*以下是将输入的数据保存*/
if(INPUT_a == '0' && ch !='g' && ch !='o'){
INPUT_a = ch;

}else if(INPUT_b == '0' && ch !='g' && ch !='o'){
INPUT_b = ch;

}else if(INPUT_c == '0' && ch !='g' && ch !='o'){
INPUT_c = ch;

}else if(INPUT_d == '0' && ch !='g' && ch !='o'){
INPUT_d = ch;

}else if(INPUT_e == '0' && ch !='g' && ch !='o'){
INPUT_e = ch;

}else if(INPUT_f == '0'&& ch !='g' && ch !='o'){
INPUT_f = ch;

}else if(INPUT_g == '0'&& ch !='g' && ch !='o'){
INPUT_g = ch;

}else if(INPUT_h == '0' && ch !='g' && ch !='o'){
INPUT_h = ch;
}else if(INPUT_i == '0'&& ch !='g' && ch !='o'){
INPUT_i = ch;
}


out_char(p_tty->p_console, ch);
out_char(p_tty->p_console, '\0');
if(INPUT_i != '0'){
/* */
int result1 = INPUT_a+INPUT_b+INPUT_c;
int result2 = INPUT_d+INPUT_e+INPUT_f;
 if(result1 == result2){
   int result3 = INPUT_g+INPUT_h+INPUT_i;
   if(result3 == result2){
    int column1 = INPUT_a+INPUT_d+INPUT_g;
    if(column1 == result3){
       int column2 = INPUT_b+INPUT_e+INPUT_h;
       if(column2 == column1){
         int column3 = INPUT_c+INPUT_f+INPUT_i;
	  if(column3 == column2)
	  {
	    int diagonal = INPUT_a+INPUT_e+INPUT_i;
	    int counter_diagonal = INPUT_c+INPUT_e+INPUT_g;
 	    if(diagonal == column3 && counter_diagonal == column3)
		{
                    disp_color_str("the result you input is right",0x74);
		out_char(p_tty->p_console, 'R');
		out_char(p_tty->p_console, 'i');
		out_char(p_tty->p_console, 'g');
		out_char(p_tty->p_console, 'h');
		out_char(p_tty->p_console, 't');
	        }else{
		   out_char(p_tty->p_console, 'w');
		    out_char(p_tty->p_console, 'r');
		    out_char(p_tty->p_console, 'o');
		    out_char(p_tty->p_console, 'n');
		    out_char(p_tty->p_console, 'g');
                     }	
	   }else{
		   out_char(p_tty->p_console, 'w');
		    out_char(p_tty->p_console, 'r');
		    out_char(p_tty->p_console, 'o');
		    out_char(p_tty->p_console, 'n');
		    out_char(p_tty->p_console, 'g');
                     }	
        }else{
		   out_char(p_tty->p_console, 'w');
		    out_char(p_tty->p_console, 'r');
		    out_char(p_tty->p_console, 'o');
		    out_char(p_tty->p_console, 'n');
		    out_char(p_tty->p_console, 'g');
              }
    }else{
		   out_char(p_tty->p_console, 'w');
		    out_char(p_tty->p_console, 'r');
		    out_char(p_tty->p_console, 'o');
		    out_char(p_tty->p_console, 'n');
		    out_char(p_tty->p_console, 'g');
           }
   }else{
    out_char(p_tty->p_console, 'w');
    out_char(p_tty->p_console, 'r');
    out_char(p_tty->p_console, 'o');
    out_char(p_tty->p_console, 'n');
    out_char(p_tty->p_console, 'g');
   }


  }else{
  disp_color_str("the result you input is wrong",0x74);
  out_char(p_tty->p_console, 'w');
out_char(p_tty->p_console, 'r');
out_char(p_tty->p_console, 'o');
out_char(p_tty->p_console, 'n');
out_char(p_tty->p_console, 'g');
  }
out_char(p_tty->p_console, '\n');
out_char(p_tty->p_console, '\n');
out_char(p_tty->p_console, 'g');
out_char(p_tty->p_console, 'o');
INPUT_a='0',INPUT_b='0',INPUT_c='0',INPUT_d='0',INPUT_e='0',INPUT_f='0',
INPUT_g='0',INPUT_h='0',INPUT_i='0';
SUM = -1;
}
SUM =SUM+1;

}
}


