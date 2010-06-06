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

#ifndef VIEW_H
#define VIEW_H

#define BUFFER_MODE_ALL       0
#define BUFFER_MODE_READONLY  1

/* buffer display modes */
#define BUFFER_NONE           0x00
#define BUFFER_FULLSCREEN     0x01
#define BUFFER_VSPLIT         0x02
#define BUFFER_HSPLIT         0x04

/* the definition of a buffer */
typedef struct {
    /* the content */
    char *content;

    /* the filename of the buffer */
    char *filename;

    /* the mode of the buffer */
    char mode;

    /* the position (first visible line number, from 0) */
    int pos;

    /* the position of the cursor, in characters from the start of the
       buffer */
    unsigned int cursor_pos;


    /* stuff only used for nice/fast display */
    /* the preferred x position */
    unsigned pref_x;
} buffer_t;

/* (relatively) high-level view management abstractions */

/* create the view */
void view_init();

/* save the view and stop displaying it 
   (this passes control to normal i/o functions */
void view_hide();

/* close up the view */
void view_close();

/* force the screen buffer to be flushed */
void view_flush();

/* message displaying

   These control the text displayed on the bottom line of the
screen. There is no buffer-specific message.
*/

/* displays a message */
void display_message(char *);

/* get the currently displayed message */
const char *get_displayed_message();


/*
  Buffer layout allows only two visible buffers, split horizontally
  or vertically.
*/

/* buffer management */

/* create a blank buffer */
buffer_t *make_blank_buffer();

/* create a buffer from a filename */
buffer_t *buffer_from_file(char *);

/* add the buffer to the buffer list, returning the buffer id */
int add_buffer(buffer_t *);

/* removes the buffer id */
void remove_buffer(int);

/* returns the id of the current buffer */
int current_buffer();

/* gets the position (1 or 2) of the current buffer */
char current_buffer_pos();

/* gets the current buffer layout */
char buffer_layout();

/* sets the current buffer */
void set_buffer(int);

/* switches to the other open buffer, if another is open */
void switch_buffer();

/* creates a vertical split, or transforms a horizontal split to a
   vertical split */
void create_vsplit();

/* creates a horizontal split, or transforms a vertical split to a
   horizontal split */
void create_hsplit();

/* eradicates any split present */
void merge_split();

/* returns the buffer object for the given id */
buffer_t *get_buffer(int);

/* cursor movement */
/* move cursor down */
void cursor_down(buffer_t *);
/* move cursor up */
void cursor_up(buffer_t *);
/* move cursor left */
void cursor_left(buffer_t *);
/* move cursor right */
void cursor_right(buffer_t *);

/* buffer editing */

#endif /* !VIEW_H */
