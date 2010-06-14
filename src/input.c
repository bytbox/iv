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
#include "util.h"
#include "view.h"

/* map of actions */
input_action_t actions[KEY_MAX];

/* initialize the input module */
void input_init() {
    int i;
    for(i=0;i<KEY_MAX;i++)
        actions[i]=unknown_action;
}

/* sets the action for a given character code */
void set_action(int c,input_action_t action) {
    actions[c]=action;
}

/* returns the action for a given character code */
input_action_t get_action(int c) {
    return actions[c];
}

/* do nothing */
void unknown_action(char c) {
#ifdef DEBUG
    /* print out the unknown character */
    char *msg=malloc(10);
    sprintf(msg,"? %d",c);
    display_message(msg);
#else
    /* just print the ? of doom */
    display_message("?");
#endif
}

/* just ignore if */
void ignore_action(char c) {
    UNUSED(c);
}

/* push a character onto the queue */
void pushchar(char c) {
    /* use ncurses */
    ungetch(c);
}

/* run the input loop */
void input_loop() {
    int c=getch();
    while(c!='q') {
        /* clear displayed message */
        display_message("");
        view_flush();
        /* get and call the relevant action */
        if(c<KEY_MAX) 
            actions[c](c);
	/* respond */
	view_flush();
        /* get the next character */
	c=getch();
    }
}
