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
    * The names of its contributors may be used to endorse or promote
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"
#include "view.h"

/* buffer info structure */
struct {
    linked_list_t *buffers; /* list of all buffers */
    char display_mode; /* the current split type */
    int buff1; /* the first buffer (if there is one) */
    int buff2; /* the second buffer (if there is one) */
} binf;

/* initialize the view */
void view_init() {
    /* allocate space for buffers */
    binf.buffers=make_linked_list();
    binf.display_mode=BUFFER_NONE;
    binf.buff1=-1;
    binf.buff2=-1;

    /* init the actual view */
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr,0);
    keypad(stdscr,1);
}

void view_hide() {

}

void view_close() {
    /* end curses */
    endwin();

    /* clean up all buffer stuff */
    ll_free(binf.buffers);
}

void view_flush() {
    /* display the message at the bottom of the screen */
    /* display the buffers */
    /* update and refresh the screen */
    doupdate();
    refresh();
}


/* message display */
void display_message(char *msg) {
    /* fixme */
}

/* return the currently displayed message */
char *get_displayed_message() {
    /* fixme */
    return 0;
}


/* buffer management */

/* creating a blank buffer */
buffer_t *make_blank_buffer() {
    buffer_t *b=malloc(sizeof(buffer_t));
    char *content=malloc(1);
    content[0]=0;
    b->content=content;
    b->filename="";
    b->mode=BUFFER_MODE_ALL;
    b->pos=0;
    b->cursor_pos=0;
    return b;
}

buffer_t *buffer_from_file(char *filename) {
    /* check mode information */
    FILE *f;
    buffer_t *b;
    if(access(filename,F_OK)) { /* exists? */
	/* attempt to create the file */
	f=fopen(filename,"w");
	if(!f) {
	    /* couldn't create the file - error out */
	    return make_blank_buffer();
	} else
	    fclose(f); /* done creating file */
	b=malloc(sizeof(buffer_t));
	b->mode=BUFFER_MODE_ALL;
	b->content=malloc(1);
	b->content[0]=0;
    } else {
	/* check that we have read permissions */
	if(access(filename,R_OK)) {
	    /* couldn't read the file - error out */
	    return make_blank_buffer();
	}
	/* can we write? */
	b=malloc(sizeof(buffer_t));
	if(access(filename,W_OK))
	    b->mode=BUFFER_MODE_READONLY;
	else b->mode=BUFFER_MODE_ALL;
	/* read the file */
	f=fopen(filename,"r");
	/* get the length of the file */
	fseek(f,0,SEEK_END);
	int len=ftell(f),i;
	/* read the file */
	fseek(f,0,SEEK_SET);
	b->content=malloc(len+1);
	for(i=0;i<len;i++)
	    b->content[i]=fgetc(f);
	b->content[len]=0; /* make sure there's a null byte */
	fclose(f);
    }

    /* stuff that's always the same */
    b->filename=filename;
    b->pos=0; /* start at the beginning */
    b->cursor_pos=0;    
    return b;
}

int add_buffer(buffer_t *b) {
    ll_append(binf.buffers,b);
    return ll_len(binf.buffers)-1;
}

void remove_buffer(int b) {
    ll_del(binf.buffers,b);
}
