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

/* C++ includes */
#include <iostream>
#include <string>
using namespace std;

/* C includes */
#include <stdio.h>
#include <string.h>

/* Test System includes */
#include "runner.hxx"

LightRunner::LightRunner() {

}

LightRunner::~LightRunner() {

}

void LightRunner::run_suite(TestSuite *suite) {
    /* start output */
    cout << "Running suite '" << suite->namestr() << "'... ";
    int passed=0; /* the number of tests passed */
    char *progress=new char[100]; /* the current progress string being displayed */
    string faillist="";
    /* run the test suite */
    /* iterate through all of the cases */
    while(!suite->done()) {
        /* run a test case */
        /* generate information */
        sprintf(progress,"%d/%d",suite->case_current()+1,suite->case_count());
        /* print information */
        cout << progress << flush;
        bool result=suite->run_case(); /* run */
        /* were we successful? */
        if(result) 
            passed++; /* we passed one more */
        else
            /* update the fail list */
            faillist+=suite->name_current()+" ";
        for(unsigned i=0;i<strlen(progress);i++)
            cout << "\b";
    }
    /* print out the status */
    if(passed!=suite->case_count())
        cout << "FAILED " << faillist;
    else cout << "passed!";
    delete progress;
    cout << endl; /* next line for next suite */
}
