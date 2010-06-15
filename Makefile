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
VERSION=0.0.2
DISTNAME=${PNAME}-${VERSION}

CFLAGS=-g -O0 -Wall -Wextra -ansi -DVERSION=\"${VERSION}\"
LFLAGS=-lncurses

PREFIX=/usr/local

CC=cc
AR=ar
LD=ld
TAR=tar
INSTALL=/usr/bin/install -c


COREMODULES=src/view.o src/input.o src/buffer.o src/util.o src/error.o \
	src/subprocess.o src/conf.o src/regex.o src/splash.o
MODULES=${COREMODULES} src/actions.o

all: iv doc

doc: doc/iv.1

doc/iv.1: doc/iv.xml
	xmlto man -o doc doc/iv.xml

src/splash.c: src/splash.txt
	scripts/text2c src/splash.txt $@ splash

mostlyclean:
	rm -f ${MODULES} src/main.o src/actiongen.o iv MANIFEST src/splash.c \
	      ${DISTNAME}.tar.gz ${DISTNAME}.tar.bz2 src/actions.c
	rm -rf ${DISTNAME}

clean: mostlyclean
	rm -f doc/iv.1

install: all
	${INSTALL} iv ${PREFIX}/bin
	${INSTALL} -D doc/iv.1 ${PREFIX}/share/man/man1/iv.1
	find etc -type d | sed "s/^/\\//" | grep -v ".svn" \
	  | xargs ${INSTALL} -d
	for f in `find etc -type f | grep -v ".svn"`;do \
	  ${INSTALL} -m 644 $$f "/$$f";done

iv: ${MODULES} src/main.o
	${CC} -o iv src/main.o ${MODULES} ${LFLAGS}

src/actions.c: iv-actiongen
	./iv-actiongen < src/actions.txt > src/actions.c

iv-actiongen: ${COREMODULES} src/actiongen.o
	${CC} -o $@ src/actiongen.o ${COREMODULES} ${LFLAGS}

sdist: ${DISTNAME}.tar.gz

${DISTNAME}.tar.gz ${DISTNAME}.tar.bz2: mostlyclean doc
	find . -type f | grep -v ".svn" | grep -v "MANIFEST" \
	  | sed "s/.\//${DISTNAME}\//" > MANIFEST
	ln -s . ${DISTNAME}
	${TAR} czf ${DISTNAME}.tar.gz -T MANIFEST
	${TAR} cjf ${DISTNAME}.tar.bz2 -T MANIFEST
	rm -rf ${DISTNAME} MANIFEST

test:

.PHONY: doc
