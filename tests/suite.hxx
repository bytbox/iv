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

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SUITE_HXX
#define SUITE_HXX

#define CASE_MAX 1000

#include <string>
using namespace std;

typedef bool (* test_case_t) (void);

class TestSuite {
public:
    TestSuite();
    virtual ~TestSuite();

    /* running test cases */
    /* returns the number of test cases, total */
    virtual int case_count();
    /* returns the number of the current test case */
    virtual int case_current();
    /* returns the name of the current test case */
    virtual string name_current();
    /* runs the current test case, and advance */
    virtual bool run_case();
    /* true if we have gone through all of the cases, false otherwise */
    virtual bool done();
    /* returns the name of the test suite */
    virtual string namestr()=0;

protected:
    int tc_cnt;
    int cur;
    test_case_t cases[CASE_MAX];
    string names[CASE_MAX];
private:
};

/* macros for defining suites */

/* for header files - just one line! */
#define SUITE(name) class __##name##Suite : public TestSuite { \
public: \
    __##name##Suite(); \
    virtual string namestr(); \
    }; \


/* for source files - after all the functions have been defined */
#define BEGIN_SUITE(name) string __##name##Suite::namestr() { \
    return #name; \
    } \
    __##name##Suite::__##name##Suite() {  \
    tc_cnt=0;

/* add a test case */
#define ADD_CASE(fname) names[tc_cnt]=#fname; \
    cases[tc_cnt++]=fname;

/* done with suite */
#define END_SUITE() }


/* assertion macros */
#define assert_true(x) if(!(x)) return false;
#define assert_false(x) if((x)) return false;
#define assert_equal(x,y) if((x)!=(y)) return false;
#define assert_notequal(x,y) if((x)==(y)) return false;


#endif /* !SUITE_HXX */
