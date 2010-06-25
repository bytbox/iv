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

#include <ctype.h>
#include <curses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>

#include "actions.h"
#include "conf.h"
#include "error.h"
#include "keys.h"

/* get ready for configuration operations */
void conf_init() {
    int i;
    for(i=0;i<KEY_MAX;i++)
        actions[i]=text_actions[i]=unknown_action;
}

/* convert a string to its key number */
int str2key(char *keystr) {
    if(strlen(keystr)==0)
        return 0; /* invalid key - ignore line */
    if(strlen(keystr)==1)
        /* just use the first character */
        return keystr[0];
    if(!strcmp(keystr,"space"))
        return ' ';
    if(strlen(keystr)==2 && keystr[0]=='^')
        /* it's a control character */
        return CTRL(keystr[1]);
    if(keystr[0]>='0' && keystr[0]<='9') 
        /* it's a number */
        return atoi(keystr);
    /* ensure that everything in keystr is upper case */
    unsigned i;
    for(i=0;i<strlen(keystr);i++)
        keystr[i]=toupper(keystr[i]);
    /* assume it's a curses key */
    return curses_strtokey(keystr);
}

/* read and apply the keymap */
void read_keymap(char *filename) {
    FILE *f=fopen(filename,"r");
    /* fixed malloc is safe - we're using fscanf */
    char *keystr=malloc(22);char *_k=keystr;
    char *action=malloc(102);char *_a=action;
    if(!f) return; /* ignore the error */
    fscanf(f,"%20s%100s",keystr,action);
    while(!feof(f)) {
        /* is this in normal mode or text mode? */
        input_action_t *table;
        if(keystr[0]=='+')
            table=text_actions,keystr++;
        else
            table=actions;
        /* convert the keystring to a key number */
        int key=str2key(keystr);
        /* look up the action */
        /* quick hack - look through the assoc list */
        int i;
        for(i=0;i<action_count;i++)
            if(!strcmp(action_table[i].name,action))
                /* assign action_table[i].action to keystr */
                table[key]=action_table[i].action;
        fscanf(f,"%20s%100s",keystr,action);
    }
    fclose(f);
    free(_k);
    free(_a);
}

/* read and apply the configuration from the specified directory */
void read_configuration(char *dirname) {
    /* read the configuration in full. In places, this may involve contacting
       other modules. */
    char *keymap=malloc(strlen(dirname)+20);
    sprintf(keymap,"%s/keymap",dirname);
    read_keymap(keymap);
    free(keymap);
}
