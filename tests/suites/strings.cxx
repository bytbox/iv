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

/* C++ Includes */
#include <iostream>
using namespace std;

/* C Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test System Includes */
#include "strings.hxx"

#include "util.h"

#define TEST_STRING_SIZE 10000

/* test that sizetoalloc() returns a large enough value */
bool sizetoalloc_more() {
    for(int i=0;i<TEST_STRING_SIZE;i++)
        assert_true(sizetoalloc(i)>i);
    return true;
}

/* test that strsize() returns a large enough value */
bool strsize_more() {
    for(int i=0;i<TEST_STRING_SIZE;i++) {
        char *str=new char[i+1];
        for(int j=0;j<i;j++)
            str[j]='a';
        str[i]='\0';
        assert_true(strsize(str)>i);
        delete[] str;
    }
    return true;
}

/* test that strsize() returns values that match sizetoalloc() */
bool strsize_matches_sizetoalloc() {
    for(int i=0;i<TEST_STRING_SIZE;i++) {
        char *str=new char[i+1];
        for(int j=0;j<i;j++)
            str[j]='a';
        str[i]='\0';
        assert_true(strsize(str)==sizetoalloc(strlen(str)));
        delete[] str;
    }
    return true;
}

/* test that sizetoalloc always returns a power of two */
bool sizetoalloc_pow2() {
    for(int i=0;i<TEST_STRING_SIZE;i++) {
        for(int j=sizetoalloc(i);j>0;j/=2)
            assert_true(j%2==0 || j==1);
    }
    return true;
}

/* test that sizetoalloc is never twice its argument */
bool sizetoalloc_double_less() {
    for(int i=0;i<TEST_STRING_SIZE;i++)
        assert_true(sizetoalloc(i)/2<=i+1);
    return true;
}

/* test that strexpand does not change the string */
bool strexpand_nochange1() {
    for(int i=0;i<TEST_STRING_SIZE;i++) {
        char *str=(char *)malloc(i+1);
        for(int j=0;j<i;j++)
            str[j]=(rand()%253)+1;
        str[i]='\0';
        char *ostr=(char *)malloc(i+2);
        sprintf(ostr,"%s",str);
        ostr=strexpand(ostr,i);
        assert_false(strcmp(ostr,str));
        free(str);
        free(ostr);
    }
    return true;
}

BEGIN_SUITE(strings)
ADD_CASE(sizetoalloc_more)
ADD_CASE(strsize_more)
ADD_CASE(strsize_matches_sizetoalloc)
ADD_CASE(sizetoalloc_pow2)
ADD_CASE(sizetoalloc_double_less)
ADD_CASE(strexpand_nochange1)
END_SUITE()
