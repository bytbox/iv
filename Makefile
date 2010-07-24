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

# Meta-information
PNAME=iv
VERSION=0.1.0
DISTNAME=${PNAME}-${VERSION}

# directories
PREFIX=/usr/local
SHARE=${PREFIX}/share

#programs
CC=cc
CXX=c++
AR=ar
LD=ld
TAR=tar
INSTALL=/usr/bin/install -c
YACC=yacc
LEX=lex

#modules
COREMODULES=src/control.o src/buffer.o src/view.o src/util.o src/error.o \
	src/subprocess.o src/conf.o src/regex.o src/splash.o
TESTMODULES=tests/driver.o tests/suite.o tests/runner.o tests/flatrunner.o \
	tests/lightrunner.o tests/cursesrunner.o ${TESTSUITES} 
TESTSUITES=tests/suites/splash.o tests/suites/strings.o
MODULES=${COREMODULES} src/actionlist.o src/defaults.o src/keys.o

#meta-rules
.PHONY: all doc test sdist mostlyclean clean
.SUFFIXES: .c .cxx .o

#general rules
.cxx.o:
	${CXX} ${CXXFLAGS} -c -o $@ $?

.c.o:
	${CC} ${CFLAGS} -c -o $@ $?

#all means the executable and the documentation
all: iv doc

#################
# Documentation #
#################
doc: doc/iv.1 doc/README.html

doc/iv.1: doc/iv.xml
	if type xmlto 2> /dev/null > /dev/null;then \
	xmlto man -o doc doc/iv.xml; \
	else echo warning: xmlto not found - not generating manpages;\
	fi

doc/README.html: README.rst
	if type rst2html 2> /dev/null > /dev/null;then \
	rst2html "--title=About iv" README.rst $@; \
	else echo warning: docutils not found - not generating readme;\
	fi


############
# Cleaning #
############
mostlyclean:
	@echo TODO FIXME
	rm -rf ${DISTNAME}

clean: mostlyclean
	rm -f doc/iv.1 doc/README.html

iv:
	@echo TODO FIXME

################
# Installation #
################
install: all
	${INSTALL} iv ${PREFIX}/bin
	${INSTALL} -D doc/iv.1 ${SHARE}/man/man1/iv.1
	${INSTALL} -d ${SHARE}/doc/iv
	${INSTALL} doc/README.html ${SHARE}/doc/iv
	${INSTALL} README.rst ${SHARE}/doc/iv
	find etc -type d | sed "s/^/\\//" | grep -v ".svn" \
	  | xargs ${INSTALL} -d
	for f in `find etc -type f | grep -v ".svn"`;do \
	  ${INSTALL} -m 644 $$f "/$$f";done

#######################
# Source distribution #
#######################
sdist: ${DISTNAME}.tar.gz

${DISTNAME}.tar.gz ${DISTNAME}.tar.bz2: mostlyclean doc
	find . -type f | grep -v ".svn" | grep -v "MANIFEST" \
	  | sed "s/.\//${DISTNAME}\//" > MANIFEST
	ln -s . ${DISTNAME}
	${TAR} czf ${DISTNAME}.tar.gz -T MANIFEST
	${TAR} cjf ${DISTNAME}.tar.bz2 -T MANIFEST
	rm -rf ${DISTNAME} MANIFEST

###########
# Testing #
###########
test:
	@echo TODO FIXME
