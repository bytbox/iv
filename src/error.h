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

#ifndef ERROR_H
#define ERROR_H

/* definitions describing errors. These are very carefully designed to be
   completely (although not statistically) orthogonal. */
/* no error - although in practice, if this error is ever thrown, there /is/
   an error */
#define ERR_NONE              0x00000000
/* the user made a mistake */
#define ERR_USER              0x00000001
/* error reading configuration */
#define ERR_CONFIG            0x00000010
/* More information available in errno */
#define ERR_IO                0x10000000
/* internal error (buggy code? assert failed?) */
#define ERR_INTERNAL          0x40000000
/* fatal error */
#define ERR_FATAL             0x80000000
/* unknown error */
#define ERR_UNKNOWN           0x88888888

/* initialize the error handling module */
void error_init();

/* chaining function to catch certain types of errors. */
int error_catch(int base,int mask,void (*next)(void));

/* throws the error, and never returns (it jumps down several stack frames as
   specified by calls to error_catch, or else exits completely */
void error_throw(int error);

#endif /* !ERROR_H */
