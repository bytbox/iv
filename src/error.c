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

#include <setjmp.h>
#include <stdlib.h>

#include "error.h"
#include "util.h"

linked_list_t *error_catch_stack;

/* a frame in the error_catch_stack */
struct error_catch_frame {
    /* the next function being called */
    void (*next)(void *);
    
    /* the base of requirements */
    int base;

    /* the mask (specifying which are actually required) */
    int mask;

    /* the place to jump */
    jmp_buf dest;
};

void error_init() {
    /* create the error catching stack */
    error_catch_stack=make_linked_list();
}

void error_cleanup() {
    ll_free(error_catch_stack);
}

int error_catch(int base,int mask,void (*next)(void *),void *data) {
    int r;
    /* create the new frame */
    struct error_catch_frame *frame=
        malloc(sizeof(struct error_catch_frame));
    frame->base=base; /* copy information */
    frame->mask=mask;
    frame->next=next;
    /* add the frame to the stack */
    ll_push(error_catch_stack,frame);
    /* call setjmp */
    if((r=setjmp(frame->dest)))
        return r;
    /* chain */
    next(data);
    /* there was no error if we reach this line */
    return 0;
}

/* test conformance of an error to a given base and mask */
int error_conforms(int error,int base,int mask) {
    int force0=(!base) & mask; /* if 1, error must be 0 */
    int force1=base & mask; /* if 1, error must be 1 */
    return ((force0 | error) == error) && 
            (((error ^ force1) & (error | force1)) == error);
}

/* this method throws an error, and doesn't return */
void error_throw(int error) {
    struct error_catch_frame *frame=ll_pop(error_catch_stack);
    while(!error_conforms(error,frame->base,frame->mask))
        frame=ll_pop(error_catch_stack);
    longjmp(frame->dest,error);
}
