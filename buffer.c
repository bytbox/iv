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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "buffer.h"
#include "util.h"
#include "view.h"

/* the list of buffers */
linked_list_t *buffer_list;

/* initialize the buffers */
void buffer_init() {
    buffer_list=make_linked_list();
}

void buffer_cleanup() {
    ll_free(buffer_list);
}

/* add the buffer to the list */
void add_buffer(buffer_t *b) {
    ll_append(buffer_list,b);
    activate_view(b->default_view);
}

/* create a blank buffer */
buffer_t *make_blank_buffer() {
    buffer_t *buffer=malloc(sizeof(buffer_t));
    /* blank content */
    buffer->lines=malloc(sizeof(char *));
    buffer->lines[0]=malloc(1);
    buffer->lines[0][0]='\0';
    /* only one line */
    buffer->line_count=1;
    /* no filename */
    buffer->filename=malloc(1);
    buffer->filename[0]='\0';
    buffer->readonly=0; /* we can write to this buffer */
    buffer->modified=0; /* even though we haven't been saved */
    /* create a view for our buffer */
    buffer->default_view=create_view(buffer);
    return buffer;
}

/* create a buffer from the filename */
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
        b->readonly=0;
        /* empty content */
        b->lines=malloc(sizeof(char *));
        b->lines[0]=malloc(1);
        b->lines[0][0]='\0';
        b->line_count=1; /* there is one line */
    } else {
        /* check that we have read permissions */
        if(access(filename,R_OK)) {
            /* couldn't read the file - error out */
            return make_blank_buffer();
        }
        /* can we write? */
        b=malloc(sizeof(buffer_t));
        if(access(filename,W_OK))
            b->readonly=1;
        else b->readonly=0;
        /* read the file */
        f=fopen(filename,"r");
        /* get the number of lines in the file */
        b->line_count=0;
        char c=fgetc(f),d='\0';
        while(!feof(f)) {
            if(c=='\n')
                b->line_count++;
            d=c;
            c=fgetc(f);
        }
        /* enforce a closing newline */
        if(d!='\n')
            b->line_count++;
        /* allocate lines */
        b->lines=calloc(sizetoalloc(b->line_count),sizeof(char *));
        /* for each line */
        int pos=0,len,lno=0;
        rewind(f);
        while(!feof(f)) {
            /* get the length of the line */
            pos=ftell(f);
            c=fgetc(f);
            while(c!='\n' && !feof(f)) 
                c=fgetc(f);
            len=ftell(f)-pos;
            /* allocate space for the line */
            b->lines[lno]=calloc(len+2,sizeof(char *));
            /* copy stuff into the line */
            fseek(f,pos,SEEK_SET);
            pos=0;
            c=fgetc(f);
            while(c!='\n' && !feof(f)) {
                b->lines[lno][pos++]=c;
                c=fgetc(f);
            }
            b->lines[lno][pos]='\0';
            /* next line */
            lno++;
        }
        fclose(f);
    }

    /* stuff that's always the same */
    b->filename=filename;
    b->modified=0;
    b->default_view=create_view(b);
    return b;
}

/* writes the buffer to the appropriate file */
void buffer_to_file(buffer_t *b) {
    /* open the file */
    FILE *f=fopen(b->filename,"w");
    /* for each line */
    int i;
    for(i=0;i<b->line_count;i++) {
        /* write the line to the file */
        fprintf(f,"%s\n",b->lines[i]);
    }
    fclose(f);
}
