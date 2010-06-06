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

/* buffer info structure */
struct {
    linked_list_t *buffers; /* list of all buffers */
    char display_mode; /* the current split type */
    int buff1; /* the first buffer (if there is one) */
    int buff2; /* the second buffer (if there is one) */
    int buff1_size; /* the size of the first buffer (ignored if only
                       one buffer is in use) */
    char active; /* the position of the active buffer */
} binf;

/* the currently displayed message */
char *bt_msg;

/* low-level functions */
/* write the buffer to the screen, putting the cursor in the right
   place */
void write_buffer(buffer_t *,int,int,int,int);/* (buffer,y,x,h,w) */

/* initialize the view */
void view_init() {
    /* allocate space for buffers */
    binf.buffers=make_linked_list();
    binf.display_mode=BUFFER_NONE;
    binf.buff1=-1;
    binf.buff2=-1;
    binf.active=1;

    /* init the actual view */
    initscr(); /* start ncurses */
    cbreak();
    noecho(); /* don't immediately echo characters */
    nonl();
    intrflush(stdscr,0);
    keypad(stdscr,1); /* enable the keypad */
    scrollok(stdscr,0); /* don't scroll off the bottom */
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
    /* fill the screen with blanks */
    int x,y;
    for(y=0;y<getmaxy(stdscr);y++)
        for(x=0;x<getmaxx(stdscr);x++)
            mvaddch(y,x,' '); /* empty the screen */
    for(y=0;y<getmaxy(stdscr)-1;y++)
        mvaddch(y,0,'~'); /* display blank lines */
    /* display the message at the bottom of the screen */
    mvaddstr(getmaxy(stdscr)-1,0,get_displayed_message());
    /* display the buffers */
    switch(binf.display_mode) {
    case BUFFER_NONE:
        /* display an intro screen */
        break;
    case BUFFER_FULLSCREEN:
        write_buffer(get_buffer(current_buffer()),
                     0,0,getmaxy(stdscr)-1,getmaxx(stdscr));
        break;
    case BUFFER_VSPLIT: 
        /* display a vertical split binf.buff1_size to the right */
        attrset(A_REVERSE);
        for(y=0;y<getmaxy(stdscr)-1;y++)
            mvaddch(y,binf.buff1_size,'|');
        attrset(A_NORMAL);
        /* display a bottom row with info */
        
        /* buffer 1 info */

        /* buffer 2 info */
        break;
    case BUFFER_HSPLIT:
        attrset(A_REVERSE);
        /* display a row with buffer 1 info binf.buff1_size down */     

        /* display a row with buffer 2 info at the bottom */
        attrset(A_NORMAL);
        break;
    default:
        /* error */
        view_close();
        fprintf(stderr,"error: invalid display mode (%s: %d)\n",__FILE__,__LINE__);
        exit(2);
    }
    /* update and refresh the screen */
    doupdate();
    refresh();
}


/* message display */
void display_message(char *msg) {
    bt_msg=msg;
}

/* return the currently displayed message */
const char *get_displayed_message() {
    return bt_msg;
}


/* buffer management */

/* creating a blank buffer */
buffer_t *make_blank_buffer() {
    buffer_t *b=malloc(sizeof(buffer_t));
    char *content=malloc(2);
    content[0]='\n';
    content[1]=0;
    b->content=content;
    b->filename="";
    b->mode=BUFFER_MODE_ALL;
    b->pos=0;
    b->cursor_pos=0;
    b->pref_x=1;
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
    b->pref_x=1;
    return b;
}

int add_buffer(buffer_t *b) {
    ll_append(binf.buffers,b);
    if(binf.display_mode==BUFFER_NONE)
        binf.display_mode=BUFFER_FULLSCREEN;
    set_buffer(ll_len(binf.buffers)-1);
    return ll_len(binf.buffers)-1;
}

void remove_buffer(int b) {
    ll_del(binf.buffers,b);
}

char current_buffer_pos() {
    return binf.active;
}

char buffer_layout() {
    return binf.display_mode;
}

void set_buffer(int i) {
    switch(current_buffer_pos()) {
    case 1:
        binf.buff1=i;
        break;
    case 2:
        binf.buff2=i;
        break;
    default:
        break;
    }
}

int current_buffer() {
    switch(current_buffer_pos()) {
    case 1:
        return binf.buff1;
    case 2:
        return binf.buff2;
    default:
        return -1;
    }
}

buffer_t *get_buffer(int i) {
    return ll_nth(binf.buffers,i);
}

void write_buffer(buffer_t *b,int y,int x,int h,int w) {
    /* locate the first character visible on screen */
    /* note: wrapping is /not/ supported, and shouldn't be */
    int i=0;
    int curx,cury; /* where the cursor will go */
    char *s=b->content; /* iterator */
    while(i<b->pos) {
        if(*s=='\n')
            i++;
        s++;
    }
    char *lb=malloc(w+2); /* line buffer */
    i=0; /* counter for how many lines we've displayed */
    int j=0;
    unsigned k=0; /* to find where to put the cursor */
    /* for each line */
    while(i<h && *s) {
        j=0;
        /* check the cursor */
        if(k==b->cursor_pos) cury=i,curx=j;
        /* copy the line */
        while(*s!='\n' && j<w && *s) {
            lb[j++]=*(s++),k++;
            /* check the cursor */
            if(k==b->cursor_pos) cury=i,curx=j;
        }
        if(*s!='\n' && *s) {
            lb[j-1]='$'; /* indicate that there's more */
            /* skip */
            while(*(s++)!='\n' && *s) {
                k++;
                /* check the cursor */
                /* FIXME implement horizontal scroll */
                if(k==b->cursor_pos) cury=i,curx=j;
            }
        } else
            s++,k++; /* get past newline */
        /* fill the rest with blanks */
        for(;j<w;j++)
            lb[j]=' ';
        lb[j]=0;
        /* write the line */
        mvaddstr(y+i,x,lb);
        i++;
    }
    /* check the cursor */
    if(k==b->cursor_pos) cury=i,curx=j;
    while(i<h) /* fill the rest of the lines with ~ */
        mvaddch(y+(i++),x,'~');
    move(cury,curx); /* position the cursor */
}

/* move cursor down */
void cursor_down(buffer_t *b) {
    char *s=b->content+b->cursor_pos;
    int x=b->pref_x-1;
    /* advance to next newline */
    while(*s && *s!='\n')
        s++;
    /* and skip the newline */
    if(*s=='\n') s++;
    /* advance by x, bit by bit - don't skip \n */
    while(x>0 && *s!='\n') {
        s++,x--;
    }
    if(s>(b->content+strlen(b->content)-1))
        s=b->content+strlen(b->content)-1;
    b->cursor_pos=s-b->content;
    /* we do not update pref_x - we move vertically */
}

/* move cursor up */
void cursor_up(buffer_t *b) {
    char *s=b->content+b->cursor_pos;
    int x=b->pref_x-1;
    if(*s=='\n') s--; /* skip newline, if any */
    /* retreat to last newline */
    while(s>b->content && *s!='\n')
        s--;
    if(*s=='\n') s--; /* skip newline */
    /* now do it again */
    while(s>b->content && *s!='\n')
        s--;
    /* skip the newline */
    if(*s=='\n') s++;
    /* advance by x, bit by bit - don't skip \n */
    while(x>0 && *s!='\n')
        s++,x--;
    if(s<b->content)
        s=b->content; /* bounds check */
    b->cursor_pos=s-b->content;
    /* we do not update pref_x - we move vertically */
}

/* calculates the current x value and caches it. This method should
   only be called when the user requests horizontal (or equivalent)
   movement. The cached variable is used to determine the effect of
   vertical movement. */
void update_pref_x(buffer_t *b) {
    char *s=b->content+b->cursor_pos;
    int x=0;
    if(*s=='\n') s--,x++;
    /* count back to the previous newline */
    while(s>=b->content && *s!='\n')
        x++,s--;
    b->pref_x=x?x:1;
}

/* move cursor left */
void cursor_left(buffer_t *b) {
    if(b->cursor_pos>0)
        b->cursor_pos--;
    if(b->content[b->cursor_pos]=='\n')
        b->cursor_pos++;
    /* calculate pref_x */
    update_pref_x(b);
}

/* move cursor right */
void cursor_right(buffer_t *b) {
    if(b->cursor_pos<strlen(b->content)-1 && b->content[b->cursor_pos]!='\n')
        b->cursor_pos++;
    /* calculate pref_x */
    update_pref_x(b);
}

