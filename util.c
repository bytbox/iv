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

#include <stdlib.h>
#include <string.h>

#include "util.h"

linked_list_t *make_linked_list() {
    linked_list_t *l=malloc(sizeof(linked_list_t));
    l->head=0;
    return l;
}

void *ll_nth(linked_list_t *l,int x) {
    llnode_t *n=l->head;
    int i=0;
    for(i=0;i<x;i++)
	if(!n) /* no such element */ 
	    return 0;
	else
	    n=n->next; /* advance */
    return n->data;
}

void *ll_del(linked_list_t *l,int x) {
    if(x==0) {
	/* special execution path for removing the head */
	if(l->head) {
	    void *d=l->head->data; /* save data */
	    l->head=l->head->next;
	    return d; /* d was removed */
	}
	return 0; /* nothing was removed */
    }
    llnode_t *n=l->head;
    int i=0;
    for(i=1;i<x;i++)
	if(!n) /* no such element */ 
	    return 0; /* nothing was removed */
	else
	    n=n->next; /* advance */
    if(!n->next)
	return 0; /* nothing to remove */
    void *d=n->next->data;
    n->next=n->next->next;
    return d;
}

void ll_append(linked_list_t *l,void *d) {
    llnode_t *newnode=malloc(sizeof(llnode_t));
    newnode->next=0;
    newnode->data=d;
    llnode_t *n=l->head;
    if(!n) {
	l->head=newnode;
	return;
    }
    while(n->next)
        n=n->next;
    n->next=newnode;
}

int ll_len(linked_list_t *l) {
    llnode_t *n=l->head;
    int len=0;
    while(n)
	len++,n=n->next;
    return len;
}

void ll_free(linked_list_t *l) {
    llnode_t *n=l->head,*tmp;
    while(n) { /* free all nodes */
	tmp=n->next;
	free(n);
	n=tmp;
    }
    /* free the list itself */
    free(l);
}

/*
  String management.

  The rule is, when expanding a string, allocate the next highest
  multiple of 2.
*/

/* returns the size that should be allocated for the given size
   string */
int sizetoalloc(int len) {
    int size=1;
    int i=0;
    while(size<=len+2) {
        i++;
        if(i>=size)
            i=0,size*=2;
    }
    return size;
}

/* returns the allocated size of the string, assuming it was allocated
   with these functions. */
int strsize(char *str) {
    return sizetoalloc(strlen(str));
}

/* expands the string to be able to fit the specified size */
char *strexpand(char *str,int newsize) {
    if(newsize+1>=strsize(str)) {
        /* figure out what the new size will be */
        int size=strsize(str);
        while(newsize+1>=size)
            size*=2; /* double size for each expansion */
        /* reallocate memory */
        return realloc(str,size);
    } else return str;
}

