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

#ifndef BUFFER_H
#define BUFFER_H

typedef struct {
    /* the content of the buffer - an array of all lines */
    char **lines;
    
    /* the number of lines in the buffer */
    int line_count;

    /* the name of the associated file */
    char *filename;

    /* is the buffer readonly? */
    char readonly;

    /* has the buffer been modified? */
    char modified;

    /* the default view for this buffer. This is kinda messy - the
       default view isn't changed in any clearly intuitive way. */
    void *default_view;
} buffer_t;


/* sets up the buffers */
void buffer_init();

/* cleans up the buffers */
void buffer_cleanup();


/* adds a buffer to the list */
void add_buffer(buffer_t *);

/* creates a blank buffer */
buffer_t *make_blank_buffer();

/* creates a buffer from a file */
buffer_t *buffer_from_file(char *);

#endif /* !BUFFER_H */
