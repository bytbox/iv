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

# programs
GOFMT = gofmt -w

# flags
GCFLAGS = -Isrc
GCF = ${GC} ${GCFLAGS}

LDFLAGS = -Lsrc
LDF = ${LD} ${LDFLAGS}

# modules
CURSESMODULES = src/main.6 src/cursesview.6
QTMODULES = src/qtmain.6 src/qtview.6
COREMODULES = src/errors.6 src/buffer.6 src/display.6 src/conf.6

#include the system-specific configuration
include ${GOROOT}/src/Make.${GOARCH}

# meta-rules
.PHONY: all doc test sdist mostlyclean clean qt \
	curses cursesmods coremods qtmods
.SUFFIXES: .c .cxx .o .go .${O}

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
	rm -f iv ${COREMODULES} ${CURSESMODULES} ${QTMODULES}
	rm -rf ${DISTNAME}

clean: mostlyclean
	rm -f doc/iv.1 doc/README.html

###################
# Actual building #
###################

coremods:
	${GCF} -o src/errors.6 src/errors.go
	${GCF} -o src/buffer.6 src/buffer.go
	${GCF} -o src/display.6 src/display.go
	${GCF} -o src/conf.6 src/conf.go
	${GCF} -o src/actions.6 src/actions.go

cursesmods:
	${GCF} -o src/cursesview.6 src/cursesview.go
	${GCF} -o src/main.6 src/main.go src/view.go

curses:iv
iv: coremods cursesmods
	${LDF} -o $@ src/main.6

qtmods:
	${GCF} -o src/qtview.6 src/qtview.go
	${GCF} -o src/qtmain.6 src/qtmain.go

qt: iv-qt
iv-qt: coremods qtmods
	${LDF} -o $@ src/qtmain.6

.go.${O}:
	${GCF} -o $@ $?

format: src/*.go
	${GOFMT} src/*.go


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
	find . -type f | grep -v "MANIFEST" \
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
