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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"
#include "view.h"

/* the message being displayed */
char *message;

/* the layout of the view */
struct {
    char hsplit; /* is there a horizontal split? */
    char vsplit; /* is there a vertical split? */
    int v1size; /* the size of the first view */
    view_t *view1; /* the first view */
    view_t *view2; /* the second view */
    view_t *active; /* the active view */
} view_layout;

/* draw a view to the specified position on the screen */
void draw_view(view_t *,int,int,int,int);

void view_init() {
    buffer_init(); /* initialize the buffers */
    /* init the actual view */
    initscr(); /* start ncurses */
    cbreak();
    noecho(); /* don't immediately echo characters */
    nonl();
    intrflush(stdscr,0);
    keypad(stdscr,1); /* enable the keypad */
    scrollok(stdscr,0); /* don't scroll off the bottom */
    view_layout.hsplit=0; /* no split */
    view_layout.vsplit=0;
    view_layout.view1=0;
    view_layout.view2=0;
    view_layout.active=0;
}

void view_hide() {

}

void view_flush() {
    /* fill the screen with blanks */
    int x,y;
    for(y=0;y<getmaxy(stdscr);y++)
        for(x=0;x<getmaxx(stdscr);x++)
            mvaddch(y,x,' '); /* empty the screen */
    for(y=0;y<getmaxy(stdscr)-1;y++)
        mvaddch(y,0,'~'); /* display blank lines */
    /* display the message at the bottom of the screen */
    mvaddstr(getmaxy(stdscr)-1,0,displayed_message());
    if(view_layout.hsplit) {
        /* paint a line down the center */
        attrset(A_REVERSE);
        for(y=0;y<getmaxy(stdscr)-1;y++)
            mvaddch(y,view_layout.v1size,'|');
        attrset(A_NORMAL);
        /* display a bottom row with info */
        
        /* buffer 1 info */

        /* buffer 2 info */
    } else if(view_layout.vsplit) {
        /* display a line through the middle */
        attrset(A_REVERSE);

        attrset(A_NORMAL);
    } else {
        /* just draaw the view */
        draw_view(view_layout.active,0,0,getmaxy(stdscr)-1,getmaxx(stdscr));
    }
    /* update and refresh the screen (curses stuff) */
    doupdate();
    refresh();
}

void draw_view(view_t *view,int y,int x,int h,int w) {
    /* is this a valid view? */
    if(!view)
        return; /* don't draw anything */
    /* make sure the view's size is right */
    view->height=h;
    view->width=w;
    /* adjust the cursor if necessary FIXME TODO */
    buffer_t *b=view->buffer;
    /* for each line on the screen */
    int i;
    for(i=view->topline;i<b->line_count && i<h+view->topline;i++) {
        /* for each character we can display */
        int j,len=strlen(b->lines[i]);
        for(j=0;j<len && j<w;j++) {
            /* display the character */
            mvaddch(y+i-view->topline,x+j,b->lines[i][j]);
        }
        /* fill the rest of the line with blanks */
        for(;j<w;j++)
            mvaddch(y+(i-view->topline),x+j,' ');
    }
    move(view->cursor_line-view->topline,view->cursor_x);
}

void view_close() {
    endwin();
    buffer_cleanup(); /* cleanup buffers */
}

view_t *current_view() {
    return view_layout.active;
}

view_t *create_view(buffer_t *buffer) {
    view_t *view=malloc(sizeof(view_t));
    view->buffer=buffer;
    /* start at the beginning of the buffer */
    view->topline=0;
    view->cursor_line=0;
    view->cursor_x=0;
    view->pref_x=0;
    /* we don't know the height and width, so guess */
    view->height=2;
    view->width=2;
    return view;
}

void activate_view(view_t *view) {
    /* which space is being used? */
    if(view_layout.active==view_layout.view1)
        view_layout.active=view_layout.view1=view;
    else
        view_layout.active=view_layout.view2=view;
}

void display_message(char *msg) {
    message=msg;
}

char *displayed_message() {
    return message;
}

/*
  cursor manipulation and scrolling
*/

/* clean the X cursor position by relocating the cursor */
void clean_cursor_x(view_t *view) {
    view->cursor_x=view->pref_x;
    if(view->cursor_x<0)
        view->cursor_x=0;
    int len=strlen(view->buffer->lines[view->cursor_line]);
    if(view->cursor_x>=len)
        view->cursor_x=len;
}

/* move the cursor to the left */
void cursor_left(view_t *view) {
    view->pref_x--;
    clean_cursor_x(view);
    view->pref_x=view->cursor_x;
}

/* move the cursor down */
void cursor_down(view_t *view) {
    view->cursor_line++;
    if(view->cursor_line>=view->buffer->line_count)
        view->cursor_line--;
    clean_cursor_x(view);
    /* scroll down if necessary */
    if((view->cursor_line-view->topline) >= view->height)
        view->topline+=(view->height+1)/2;
}

/* move the cursor up */
void cursor_up(view_t *view) {
    if(view->cursor_line>0)
        view->cursor_line--;
    clean_cursor_x(view);
    /* scroll up if necessary */
    if((view->cursor_line-view->topline) <0)
        view->topline-=(view->height+1)/2;
}

/* move the cursor right */
void cursor_right(view_t *view) {
    view->pref_x++;
    clean_cursor_x(view);
    view->pref_x=view->cursor_x;
}

/* inserts a character at the cursor */
void insertc(view_t *view,char c) {
    char *line=view->buffer->lines[view->cursor_line];
    
}
