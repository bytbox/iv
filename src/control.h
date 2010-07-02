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
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.  
*/

#ifndef INPUT_H
#define INPUT_H

#define CTRL(x) (x-'A'+1)

#define ACTION_HASH_SIZE 200

/* an action */
typedef void (* input_action_t) ();

/* actions in standard mode (whatever the current mode is) */
extern input_action_t actions[];

/* actions in text mode (whatever the current mode is) */
extern input_action_t text_actions[];

/* initialize the input system */
void input_init();

/* push a character onto the queue */
void pushchar(char);

/* run the input loop */
void input_loop();


/*
  managing actions
*/

/* set the action for a given key combination */
void set_action(int,input_action_t);

/* get the action for a given key combination */
input_action_t get_action(int);

/*
  stuff for hash maps
*/

/* hash the given string */
int action_hash(void *);

/* hash the given equality */
char action_eql(void *,void *);

/*
  The list of actions. 
*/

void unknown_action();
void ignore_action();
void goto_action();
void down_action();
void left_action();
void right_action();
void up_action();
void write_action();
void text_action();
void backspace_action();
void newline_action();
void open_action();
void hsplit_action();
void vsplit_action();
void qsplit_action();

#endif /* !INPUT_H */

