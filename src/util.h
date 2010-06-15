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

/* returns the head */
void *ll_head(linked_list_t *);

/* pops off the head */
void *ll_pop(linked_list_t *);

/* returns the nth element */
void *ll_nth(linked_list_t *,int);

/* removes the nth element (returning the value) */
void *ll_del(linked_list_t *,int);

/* append an element */
void ll_append(linked_list_t *,void *);

/* prepend an element */
void ll_prepend(linked_list_t *,void *);
/* convenience for people who actually wanted a stack */
#define ll_push ll_prepend

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


/*
  Hashtables
*/

/* a key-value pair for use in hash tables */
typedef struct {
    void *key;
    void *value;
} kvpair_t;

/* hash table */
typedef struct {
    int size; /* the size of the hashtable */
    int *rsize; /* an array of sizes of rows */
    int *ruse; /* an array of usages of rows */
    kvpair_t ***table; /* an array of arrays of pointers to kvpairs */
    int (*hash)(void *); /* takes a key */
    char (*equal)(void *,void *); /* takes two keys */
} hashtable_t;

/* create a hashtable */
hashtable_t *make_hashtable(int,int (*)(void *),char (*)(void *,void *));

/* add data to a hashtable */
int hashtable_add(hashtable_t *,void *,void *); /* table, key, value */

/* modify data in a hashtable */
int hashtable_mod(hashtable_t *,void *,void *);

/* get data from a hashtable */
void *hashtable_retrieve(hashtable_t *,void *);

/* iterate through a hashtable */
int hashtable_foreach(hashtable_t *,int (*)(hashtable_t *,void *,void *),void *);

/* remove an item from a hashtable */
void *hashtable_remove(hashtable_t *,void *); /* table,key->value */

/* free all memory used by a hashtable */
int cleanup_hashtable(hashtable_t *);

/* print a C array representation of a hashtable */
void hashtable_print(hashtable_t *);


#endif /* !UTIL_H */
