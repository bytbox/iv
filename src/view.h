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

#ifndef VIEW_H
#define VIEW_H

#include "buffer.h"

#define VERTICAL    0
#define HORIZONTAL  1

typedef struct {
    /* the associated buffer */
    buffer_t *buffer;

    /* the topmost visible line */
    int topline;

    /* the line the cursor is on */
    int cursor_line;

    /* the current x value of the cursor */
    int cursor_x;

    /* the preferred x value of the cursor. This value is changed only
       as a result of deliberate horizontal motion. */
    int pref_x;

    /* the height of the view */
    int height;
    
    /* the width of the view */
    int width;
} view_t;

/*
  high-level view management
*/

/* sets up the screen */
void view_init();

/* hide the screen and allow terminal operations */
void view_hide();

/* refreshed the screen */
void view_flush();

/* close up shop */
void view_close();


/*
  doing stuff with actual views
*/

/* returns the currently open view */
view_t *current_view();

/* creates a view for the given buffer */
view_t *create_view(buffer_t *);

/* sets the given view to be the current view */
void activate_view(view_t *);


/*
  split modes
*/

/* creating splits */
void create_split(int, view_t *);

/* kill any existing split */
void kill_split();


/*
  displayed message management 
*/

/* set the displayed message */
void display_message(char *);

/* returns the displayed message */
char *displayed_message();

/* retreives a string from the user in the buffer, and returns it. The string
   passed as a parameter is used as a prefix. */
char *get_input(char *);

/*
  cursor manipulation and scrolling
*/

/* move the cursor to the left */
void cursor_left(view_t *);

/* move the cursor down */
void cursor_down(view_t *);

/* move the cursor up */
void cursor_up(view_t *);

/* move the cursor right */
void cursor_right(view_t *);

/* inserts a character at the cursor */
void insertc(view_t *,unsigned char);

/* inserts a string at the cursor */
void inserts(view_t *,char *);

/* deletes a character before the cursor */
void deletec(view_t *);

/* inserts a line break at the cursor */
void insertlb(view_t *);

/* goto a given line */
void goto_line(view_t *,int);

#endif /* !VIEW_H */
