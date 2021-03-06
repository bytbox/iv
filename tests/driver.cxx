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

/* C++ header files */
#include <iostream>
using namespace std;

/* C header files */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* iv header files */

/* iv-test header files */
#include "runner.hxx"
#include "suite.hxx"

/* suites */
#include "suites/splash.hxx"
#include "suites/strings.hxx"

/* options */
#define RUNNER LightRunner

int main(int argc,char *argv[]) {
    int opt;
    while((opt=getopt(argc,argv,"hV"))!=-1)
        switch(opt) {
        case 'h':
            return 0;
        case 'V':
            printf("iv %s\n",VERSION);
            printf("iv -V for more information\n");
            return 0;
        case '?':
            exit(EXIT_FAILURE);
        default:
            exit(EXIT_FAILURE);
        }

    /* create the runner */
    TestRunner *runner=new RUNNER();
    /* FIXME make this happen automatically */
    /* add the suites */
    runner->add_suite(new __splashSuite());
    runner->add_suite(new __stringsSuite());
    /* run the tests */
    runner->run_tests();
    delete runner;
    return 0;
}
