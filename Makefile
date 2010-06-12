####################################################################################
# iv - an editor                                                                   #
# Copyright (c) 2010, Scott Lawrence <bytbox@gmail.com>                            #
# All rights reserved.                                                             #
#                                                                                  #
# Redistribution and use in source and binary forms, with or without               #
# modification, are permitted provided that the following conditions are met:      #
#     * Redistributions of source code must retain the above copyright             #
#       notice, this list of conditions and the following disclaimer.              #
#     * Redistributions in binary form must reproduce the above copyright          #
#       notice, this list of conditions and the following disclaimer in the        #
#       documentation and/or other materials provided with the distribution.       #
#     * The names of its contributors may not be used to endorse or promote        #
#       products derived from this software without specific prior                 #
#       written permission.                                                        #
#                                                                                  #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  #
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    #
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           #
# DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY                 #
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES       #
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;     #
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND      #
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       #
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS    #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     #
####################################################################################

PNAME=iv
VERSION=0.0.1
DISTNAME=${PNAME}-${VERSION}

CFLAGS=-g -O0 -Wall -Wextra -ansi -DVERSION=\"${VERSION}\"
LFLAGS=-lncurses

PREFIX=/usr/local

CC=cc
AR=ar
LD=ld
TAR=tar
INSTALL=/usr/bin/install -c

MODULES=main.o view.o input.o buffer.o util.o error.o subprocess.o conf.o regex.o

all: iv

clean:
	rm -f ${MODULES} iv
	rm -rf ${DISTNAME}

install:
	${INSTALL} iv ${PREFIX}/bin

iv: ${MODULES}
	${CC} -o iv ${MODULES} ${LFLAGS}

sdist: clean
	mkdir -p ${DISTNAME}

test:

