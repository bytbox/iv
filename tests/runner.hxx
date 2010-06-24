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

#ifndef RUNNER_HXX
#define RUNNER_HXX

#define SUITE_MAX 1000

#include "suite.hxx"

class TestRunner {
public:
    TestRunner();
    virtual ~TestRunner();

    /* running the tests */
    virtual void run_tests();
    virtual void run_suite(TestSuite *)=0;

    /* suite management */
    virtual void add_suite(TestSuite *);
protected:
    /* the suites */
    TestSuite *suites[SUITE_MAX];
    int suite_count;
private:
};

class FlatRunner : public TestRunner {
public:
    FlatRunner();
    virtual ~FlatRunner();

    /* running a suite */
    virtual void run_suite(TestSuite *);
protected:
private:
};

class LightRunner : public TestRunner {
public:
    LightRunner();
    virtual ~LightRunner();

    /* running a suite */
    virtual void run_suite(TestSuite *);
protected:
private:
};

class CursesRunner : public TestRunner {
public:
    virtual ~CursesRunner();
protected:
private:
};

#endif /* !RUNNER_HXX */
