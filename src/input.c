/*
  iv - the editor that Isn't VI
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
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "error.h"
#include "input.h"
#include "util.h"
#include "view.h"

/* maps of actions */
input_action_t actions[KEY_MAX];
input_action_t text_actions[KEY_MAX];

/* initialize the input module */
void input_init() {
    int i;
    for(i=0;i<KEY_MAX;i++) {
        actions[i]=unknown_action;
        text_actions[i]=unknown_action;
    }
}

/* do nothing */
void unknown_action() {
    /* just print the ? of doom */
    display_message("?");
}

/* just ignore if */
void ignore_action() {
}

/* move the cursor down */
void down_action() {
    cursor_down(current_view());
}

/* move the cursor left */
void left_action() {
    cursor_left(current_view());
}

/* move the cursor up */
void up_action() {
    cursor_up(current_view());
}

/* move the cursor right */
void right_action() {
    cursor_right(current_view());
}

/* save the file */
void write_action() {
    buffer_to_file(current_view()->buffer);
    display_message("wrote");    
}

/* delete char */
void backspace_action() {
    deletec(current_view());
}

/* writing text */
void text_action() {
    int c=getch(),err;
    /* start up a very similar input loop */
    while(c!=CTRL('D')) {
        /* clear displayed message */
        display_message("");
        view_flush();
        /* get and call the relevant action */
        if(c<KEY_MAX) { /* make sure the key is valid */
            /* print the key if no action assigned */
            if(text_actions[c]==unknown_action)
                insertc(current_view(),c);
            else
                /* catch any errors */
                if((err=error_catch(ERR_NONE,ERR_FATAL,text_actions[c],0))) {
                    /* some error */
                    display_message("error");
                    if(err & ERR_READONLY) /* readonly */
                        display_message("readonly");
                }
        }
	/* respond */
	view_flush();
        /* get the next character */
	c=getch();        
    }
}

/* push a character onto the queue */
void pushchar(char c) {
    /* use ncurses */
    ungetch(c);
}

/* run the input loop */
void input_loop() {
    int c=getch(),err;
    while(c!='q') {
        /* clear displayed message */
        display_message("");
        view_flush();
        /* get and call the relevant action */
        if(c<KEY_MAX) /* make sure the key is valid */
            /* catch any errors */
            if((err=error_catch(ERR_NONE,ERR_FATAL,actions[c],0))) {
                /* some error */
                display_message("error");
                if(err & ERR_READONLY) /* readonly */
                    display_message("readonly");
            }
	/* respond */
	view_flush();
        /* get the next character */
	c=getch();
    }
}

/* hash the given string */
int action_hash(void *vstr) {
    char *str=vstr;
    int len=strlen(str);
    return (len*str[0])%ACTION_HASH_SIZE;
}

/* hash the given equality */
char action_eql(void *a,void *b) {
    return strcmp((char *)a,(char *)b);
}

