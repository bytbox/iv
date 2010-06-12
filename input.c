/*
  iv - an editor
  Copyright (c) 2010, Scott Lawrence <bytbox@gmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The names of its contributors may not be used to endorse or promote
      products derived from this software without specific prior
      written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.
*/

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "input.h"
#include "view.h"

#define CTRL(x) (x-'A')

void pushchar(char c) {
    /* use ncurses */
    ungetch(c);
}

/* run the input loop */
void input_loop() {
    int c=getch();
    while(c!='q') {
        display_message("");
        view_flush();
	switch(c) {
            /* stuff to ignore, because it just happens */
        case CTRL('c'):
        case -1:
            /* ignore */
            break;
        case 'a':
            /* add text */
            /* read characters until a stop character is
               encountered */
            display_message("m:a");
            view_flush();
            c=getch();
            while(c!=CTRL('C')) {
                if(c==KEY_ENTER || c==KEY_IL || c=='\n' || c=='\r') {
                    insertlb(current_view());
                } else
                    insertc(current_view(),c);
                view_flush();
                c=getch();
            }
            break;
        case 'w':
            /* write the file */
            /* FIXME TODO what if the filename is blank? */
            buffer_to_file(current_view()->buffer);
            break;
	case 'h':
        case KEY_LEFT:
	    /* move left */
	    cursor_left(current_view());
	    break;
	case 'j':
        case KEY_DOWN:
	    /* move down */
	    cursor_down(current_view());
	    break;
	case 'k':
        case KEY_UP:
	    /* move up */
	    cursor_up(current_view());
	    break;
	case 'l':
        case KEY_RIGHT:
	    /* move right */
	    cursor_right(current_view());
	    break;
	default:
	    /* unknown character */
            {
#ifdef DEBUG
                char *msg=malloc(10);
                sprintf(msg,"? %d",c);
                display_message(msg);
#else
                display_message("?");
#endif
            }
	}
	/* respond */
	view_flush();
	c=getch();
    }
}
