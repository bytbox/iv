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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "error.h"
#include "control.h"
#include "util.h"
#include "view.h"

#define ERRSTR_MAXLEN 500

/* maps of actions */
input_action_t actions[KEY_MAX];
input_action_t text_actions[KEY_MAX];

/* private error handling function */
void handle_error(int);

/* initialize the input module */
void input_init() {
    int i;
    for(i=0;i<KEY_MAX;i++) {
        actions[i]=unknown_action;
        text_actions[i]=unknown_action;
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
            if((err=error_catch(ERR_NONE,ERR_FATAL,actions[c],0)))
                handle_error(err);
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



/*
  The actions
*/


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
    display_message("w"); 
}

/* tab action */
void tab_action() {
    insertc(current_view(),'\t');
}

/* delete char */
void backspace_action() {
    deletec(current_view());
}

/* insert line break */
void newline_action() {
    insertlb(current_view());
}

/* open a file */
void open_action() {
    /* input the name of the file */
    char *fname=get_input("f:");
    activate_view(create_view(buffer_from_file(fname)));
}

/* goto a line */
void goto_action() {
    /* input the line number */
    char *lno=get_input("l:");
    errno=0;
    int a=strtol(lno,0,10);
    if(errno)
        error_throw(ERR_BADINPUT | ERR_SEE_ERRNO);
    goto_line(current_view(),a);
}

/* writing text */
void text_action() {
    /* clear displayed message to 't'*/
    display_message("t");
    view_flush();
    /* start input */
    int c=getch(),err;
    /* start up a very similar input loop */
    while(c!=CTRL('D')) {
        view_flush();
        /* get and call the relevant action */
        if(c<KEY_MAX) { /* make sure the key is valid */
            /* print the key if no action assigned */
            if(text_actions[c]==unknown_action)
                insertc(current_view(),c);
            else
                /* catch any errors */
                if((err=error_catch(ERR_NONE,ERR_FATAL,text_actions[c],0)))
                    handle_error(err);
        }
	/* respond */
	view_flush();
        /* get the next character */
	c=getch();        
        /* clear displayed message to 't'*/
        display_message("t");
    }
    /* clear displayed message */
    display_message("");
}

/* create a horizontal split - one window to the right of the other */
void hsplit_action() {
    /* create another view with the current buffer */
    view_t *view=create_view(current_view()->buffer);
    create_split(HORIZONTAL,view);
}

/* create a vertical split - one window above the other */
void vsplit_action() {
    /* create another view with the current buffer */
    view_t *view=create_view(current_view()->buffer);
    create_split(VERTICAL,view);
}

/* quit split mode */
void qsplit_action() {
    /* pass the kill on to the view module */
    kill_split();
}


/* handle errors */
void handle_error(int err) {
    /* some error */
    char *errstr=calloc(ERRSTR_MAXLEN,sizeof(char));
    if(err & ERR_READONLY) /* readonly */
        sprintf(errstr,"e:ro");
    else if(err & ERR_BADINPUT)
        sprintf(errstr,"e:bi");
    else
        sprintf(errstr,"e");

    /* if there is system error information available, show it */
    if(err & ERR_SEE_ERRNO)
        sprintf(errstr,"%s: %s",errstr,strerror(errno));
    /* display the message */
    display_message(errstr);
}
