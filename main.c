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
    * The names of its contributors may be used to endorse or promote
      products derived from this software without specific prior
      written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.  
*/

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "view.h"

#define VERSION "0.0.1"

void show_help();
void show_version();

void prepare_signal_handler();
void signal_handler(int);

int main(int argc,char *argv[]) {
    /* option parsing */
    int opt;
    char openany=0;
    while((opt=getopt(argc,argv,"hV"))!=-1)
	switch(opt) {
	case 'h':
	    show_help();
	    return 0;
	case 'V':
	    show_version();
	    return 0;
	case '?':
	    exit(1);
	default:
	    puts("iv: strange error");
	    exit(10);
	}

    /* start up the editor */
    prepare_signal_handler();
    view_init();
    
    /* look for list of files to open */
    opt=optind;
    while(opt<argc) {
	char *fname=argv[opt++];
	openany=1;
    }

    if(!openany) {
	/* create a blank buffer */
    }

    /* close up the editor */
    view_close();
    return 0;
}

void prepare_signal_handler() {
    signal(SIGINT,signal_handler);
}

void signal_handler(int signal) {
    switch(signal) {
    case SIGINT:
	/* don't actually exit, just print message */
	break;
    default:
	/* uncaught signal - don't fret */
	break;
    }
}

void show_help() {
    puts("usage: iv [OPTIONS]");
}

void show_version() {
    printf("iv version %s\n",VERSION);
    puts("Copyright (C) 2010 Scott Lawrence <bytbox@gmail.com>");
    puts("Licensed under the GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.");
    puts("This is free software: you are free to change and redistribute it.");
    puts("There is NO WARRANTY, to the extent permitted by law.");
}
