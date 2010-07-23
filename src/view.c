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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "control.h"
#include "error.h"
#include "util.h"
#include "view.h"

#define MAX_INPUT_SIZE 800

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
    start_color(); /* enable the use of color */
    cbreak(); /* don't wait for RETURN or ENTER to get input */
    noecho(); /* don't immediately echo characters */
    nonl(); /* don't display newlines as newlines */
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
    attron(A_BOLD);
    for(y=0;y<getmaxy(stdscr)-1;y++)
        mvaddch(y,0,'~'); /* display blank lines */
    attroff(A_BOLD);
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
    int i,ax,actual_x=view->cursor_x;
    for(i=view->topline;i<b->line_count && i<h+view->topline;i++) {
        ax=0; /* to keep track of where we actually need to put the cursor */
        /* for each character we can display */
        int j,len=strlen(b->lines[i]),__;
        move(y+i-view->topline,x);
        for(j=0;j<len && j<w;j++) {
            /* display the character */
            addch(b->lines[i][j]);
            if(i==view->cursor_line) {
                /* pay attention to the x position */
                ax++;
                if(ax==view->cursor_x)
                    getyx(stdscr,__,actual_x);
            }
        }
        /* fill the rest of the line with blanks */
        getyx(stdscr,__,j);
        for(;j<w;j++)
            mvaddch(y+(i-view->topline),x+j,' ');
    }
    move(view->cursor_line-view->topline,actual_x);
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

/* set the split mode, and the second view to be used. (The first view to be
   used is the active one. */
void create_split(int type, view_t *view) {

}

/* get rid of any existing splits */
void kill_split() {

}

/* display the given message */
void display_message(char *msg) {
    message=msg;
}

/* returns the currently displayed message */
char *displayed_message() {
    return message;
}

/* long-running function to get input */
char *get_input(char *prefix) {
    view_t *view=current_view();
    /* write the prefix */
    mvaddstr(view->height,0,prefix);
    doupdate();
    refresh();
    /* read input */
    char *buffer=(char *)malloc(MAX_INPUT_SIZE);
    int c=getch(),i=0;
    while(c!='\n' && c!='\r') {
        if(c==CTRL('D'))
            return 0; /* no input */
        /* add the character to the screen */
        addch(c);
        doupdate(); /* and flush */
        refresh();
        /* add to our internal buffer */
        buffer[i++]=c;
        if(i==MAX_INPUT_SIZE)
            return 0; /* FIXME throw actual error */
        c=getch();
    }
    return buffer;
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

/* clean the preferred X cursor position */
void clean_pref_x(view_t *view) {
    int len=strlen(view->buffer->lines[view->cursor_line]);
    if(view->pref_x>=len)
        view->pref_x=len;
}

/* move the cursor to the left */
void cursor_left(view_t *view) {
    clean_pref_x(view);
    view->pref_x--;
    clean_cursor_x(view);
    view->pref_x=view->cursor_x;
    clean_pref_x(view);
}

/* move the cursor down */
void cursor_down(view_t *view) {
    view->cursor_line++;
    if(view->cursor_line>=view->buffer->line_count)
        view->cursor_line--;
    clean_cursor_x(view);
    /* scroll down if necessary */
    if((view->cursor_line-view->topline) >= view->height)
        view->topline+=1;
    clean_cursor_x(view);
}

/* move the cursor up */
void cursor_up(view_t *view) {
    if(view->cursor_line>0)
        view->cursor_line--;
    clean_cursor_x(view);
    /* scroll up if necessary */
    if((view->cursor_line-view->topline) <0)
        view->topline-=1;
    clean_cursor_x(view);
}

/* move the cursor right */
void cursor_right(view_t *view) {
    clean_pref_x(view);
    view->pref_x++;
    clean_cursor_x(view);
    view->pref_x=view->cursor_x;
    clean_pref_x(view);
}

/* inserts a character at the cursor */
void insertc(view_t *view,unsigned char c) {
    /* check readonly */
    if(view->buffer->readonly)
        error_throw(ERR_READONLY);
    /* check valid character */
    if((char)c<0) return;
    char *line=view->buffer->lines[view->cursor_line];
    /* make sure the line is long enough */
    view->buffer->lines[view->cursor_line]=strexpand(line,strlen(line)+2);
    line=view->buffer->lines[view->cursor_line];
    /* make a copy of the line */
    char *tln=malloc(strlen(line)+2);
    char *ln=tln;
    sprintf(ln,"%s",line);
    /* put the line back together */
    /* copy the first part back */
    line[0]='\0';
    strncat(line,ln,view->cursor_x);
    /* copy the character */
    line[strlen(line)+1]='\0';
    line[strlen(line)]=c;
    /* copy the second part back */
    ln+=view->cursor_x;
    strcat(line,ln);
    free(tln); /* free temporary buffer */
    /* advance the cursor */
    view->pref_x++;
    clean_cursor_x(view);
    /* we've been modified */
    view->buffer->modified=1;
}

/* inserts a line break at the cursor */
void insertlb(view_t *view) {
    buffer_t *b=view->buffer;
    if(b->readonly) 
        error_throw(ERR_READONLY);
    /* enlarge the line count */
    b->line_count++;
    /* allocate more space */
    b->lines=realloc(b->lines,sizeof(char *)*b->line_count);
    int nln=view->cursor_line; /* the number of the new line -1 */
    /* copy lines down */
    int l;
    for(l=b->line_count-2;l>=nln;l--)
        b->lines[l+1]=b->lines[l];
    /* create the new line */
    b->lines[nln]=malloc(view->cursor_x+2);
    b->lines[nln][0]='\0';
    /* copy the first part into the new line*/
    strncat(b->lines[nln],b->lines[nln+1],view->cursor_x);
    /* copy the second part out of the next line */
    char *sp=malloc(strsize(b->lines[nln+1]+view->cursor_x));
    sp[0]='\0';
    strcat(sp,b->lines[nln+1]+view->cursor_x);
    free(b->lines[nln+1]); /* free old stuff */
    b->lines[nln+1]=sp; /* use new stuff */
    view->pref_x=0; /* we're at the beginning of the line */
    /* move the cursor down */
    cursor_down(view);
    /* we've been modified */
    b->modified=1;
}

/* delete a character at the cursor */
void deletec(view_t *view) {
    /* exit if there's nothing to delete */
    if(view->cursor_x==0 && view->cursor_line==0)
        return;
    /* clean up our position */
    clean_cursor_x(view);
    clean_pref_x(view);
    /* should we merge lines? */
    if(view->cursor_x==0) {
        /* merge lines */
        char *line=view->buffer->lines[view->cursor_line];
        view->buffer->lines[view->cursor_line-1]=
            strexpand(view->buffer->lines[view->cursor_line-1],
                      strlen(view->buffer->lines[view->cursor_line-1])+
                      strlen(line));
        view->pref_x=strlen(view->buffer->lines[view->cursor_line-1]);
        strcat(view->buffer->lines[view->cursor_line-1],line);
        free(line); /* free the line */
        /* reduce line count */
        view->buffer->line_count--;
        /* scroll all future lines up */
        int i;
        for(i=view->cursor_line;i<view->buffer->line_count;i++)
            view->buffer->lines[i]=view->buffer->lines[i+1];
        /* move the cursor */
        cursor_up(view);
        return;
    }
    /* no line merging to be done */
    /* get the line */
    char *line=view->buffer->lines[view->cursor_line];
        /* make a copy of the line */
    char *tln=malloc(strlen(line)+2);
    char *ln=tln;
    sprintf(ln,"%s",line);
    /* copy the first part back */
    line[0]='\0';
    strncat(line,ln,view->cursor_x-1);
    /* copy the second part back */
    ln+=view->cursor_x;
    strcat(line,ln);
    free(tln); /* free temporary buffer */
    /* retreat the cursor */
    view->pref_x--;
    view->cursor_x--;
    /* we've been modified */
    view->buffer->modified=1;
}

/* goto the given line in the given view */
void goto_line(view_t *view,int lno) {
    if(lno<=0)
        error_throw(ERR_BADINPUT);
    /* set the cursor line and refresh */
    view->cursor_line=lno-1;
    /* move the screen */
    while(view->topline>lno-1)
        view->topline--;
    while(view->cursor_line-view->topline>=view->height)
        view->topline++;
    /* clean the cursor X coordinate */
    clean_cursor_x(view);
}
