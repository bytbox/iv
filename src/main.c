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

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conf.h"
#include "config.h"
#include "error.h"
#include "input.h"
#include "util.h"
#include "view.h"

void show_help();
void show_version();

void prepare_signal_handler();
void signal_handler(int);

/* stuff that happens inside error handling */
void main1(void *);

/* variables for trading between stages */
int opt;
short openany;
int argc_;
char **argv_;

int main(int argc,char *argv[]) {
    argc_=argc;
    argv_=argv;
    /* option parsing */
    while((opt=getopt(argc,argv,"hV"))!=-1)
        switch(opt) {
        case 'h':
            show_help();
            return 0;
        case 'V':
            show_version();
            return 0;
        case '?':
            exit(EXIT_FAILURE);
        default:
            exit(EXIT_FAILURE);
        }

    /* prepare error handling */
    error_init();
    int err;
    /* catch all errors, and call next stage */
    if((err=error_catch(ERR_NONE,ERR_NONE,main1,0))) {
        /* there was some sort of error */
        if(err & ERR_SEE_ERRNO)
            perror("uncaught error");
        return 1;
    }
    return 0;
}

/* inside error handling */
void main1(void *data) {
    UNUSED(data);
    /* extract stuff */
    int argc=argc_;
    char **argv=argv_;
    /* figure out what the configuration directory should be */
    char *confdir=getenv("IV_CONFDIR");
    if(!confdir) /* if it's not set, use default */
        confdir="~/.iv.d";

    /* start up the editor */
    prepare_signal_handler();
    view_init();
    conf_init();
    /* get ready for input */
    input_init();
    /* now read the configuration */
    read_configuration("/etc/iv"); /* global configuration */
    read_configuration(confdir); /* local configuration */

    /* display startup message */
    char *msg=malloc(500);
    sprintf(msg,"iv v%s - Copyright (C) Scott Lawrence 2010",VERSION);
    display_message(msg);
    
    /* look for list of files to open */
    opt=optind;
    while(opt<argc) {
        char *fname=argv[opt++];
        openany=1;
        add_buffer(buffer_from_file(fname));
    }

    if(!openany) {
        /* create a blank buffer */
        add_buffer(make_blank_buffer());
    }

    /* make sure everything is displayed */
    view_flush();

    /* run the input loop */
    input_loop();

    /* close up the editor */
    view_close();
}

void prepare_signal_handler() {
    signal(SIGINT,signal_handler);
    signal(SIGTERM,signal_handler);
    signal(SIGABRT,signal_handler);
    signal(SIGQUIT,signal_handler);
    signal(SIGALRM,signal_handler);
    signal(SIGHUP,signal_handler);
}

void signal_handler(int signal) {
    switch(signal) {
    case SIGINT:
        pushchar(CTRL('C'));
        break;
    default:
        /* uncaught signal - don't fret */
        break;
    }
    prepare_signal_handler(); /* refresh signals */
}

void show_help() {
    puts("usage: iv [OPTIONS] [file1 ...]");
    puts("Run iv - the editor that Isn't Vi");
    puts("");
    puts(" -h             display this screen and exit");
    puts(" -V             print version information and exit");
}

void show_version() {
    printf("iv version %s\n",VERSION);
    puts("Copyright (C) 2010 Scott Lawrence <bytbox@gmail.com>");
    puts("This is free software: you are free to change and redistribute it.");
    puts("There is NO WARRANTY, to the extent permitted by law.");
    puts("See COPYING for details.");
}
