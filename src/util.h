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

#ifndef UTIL_H
#define UTIL_H

/*
  utility macros
*/

/* mark as ignored (suppressing unused warnings) */
#define UNUSED(x) (x=x)

/*
  linked lists
*/

/* a node in a linked list */
struct llnode;
typedef struct llnode {
    struct llnode *next; /* pointer to next node */
    void *data; /* the data */
} llnode_t;

/* a linked list */
typedef struct {
    llnode_t *head;
} linked_list_t;

/* creates an empty linked list */
linked_list_t *make_linked_list();

/* returns the nth element */
void *ll_nth(linked_list_t *,int);

/* removes the nth element (returning the value) */
void *ll_del(linked_list_t *,int);

/* append an element */
void ll_append(linked_list_t *,void *);

/* returns the length of the list */
int ll_len(linked_list_t *);

/* frees a linked list */
void ll_free(linked_list_t *);


/*
  String management.

  The rule is, when expanding a string, allocate the next highest
  multiple of 2.
*/

/* returns the size that should be allocated for the given size
   string */
int sizetoalloc(int);

/* returns the allocated size of the string, assuming it was allocated
   with these functions. */
int strsize(char *);

/* expands the string to be able to fit the specified size */
char *strexpand(char *,int);


#endif /* !UTIL_H */

