# Copyright (C) 2004, 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 759 2006-07-07 03:07:08Z andreasw $

# Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

AUTOMAKE_OPTIONS = foreign

SUBDIRS = TMatrices

includeipoptdir = $(includedir)/ipopt
includeipopt_HEADERS = \
	IpMatrix.hpp \
	IpSymMatrix.hpp \
	IpVector.hpp

noinst_LTLIBRARIES = liblinalg.la

liblinalg_la_SOURCES = \
	IpBlas.cpp IpBlas.hpp \
	IpCompoundMatrix.cpp IpCompoundMatrix.hpp \
	IpCompoundSymMatrix.cpp IpCompoundSymMatrix.hpp \
	IpCompoundVector.cpp IpCompoundVector.hpp \
	IpDenseGenMatrix.cpp IpDenseGenMatrix.hpp \
	IpDenseSymMatrix.cpp IpDenseSymMatrix.hpp \
	IpDenseVector.cpp IpDenseVector.hpp \
	IpDiagMatrix.cpp IpDiagMatrix.hpp \
	IpExpansionMatrix.cpp IpExpansionMatrix.hpp \
	IpIdentityMatrix.cpp IpIdentityMatrix.hpp \
	IpLapack.cpp IpLapack.hpp \
	IpLowRankUpdateSymMatrix.cpp IpLowRankUpdateSymMatrix.hpp \
	IpMatrix.cpp IpMatrix.hpp \
	IpMultiVectorMatrix.cpp IpMultiVectorMatrix.hpp \
	IpScaledMatrix.cpp IpScaledMatrix.hpp \
	IpSumMatrix.cpp IpSumMatrix.hpp \
	IpSumSymMatrix.cpp IpSumSymMatrix.hpp \
	IpSymMatrix.cpp IpSymMatrix.hpp \
	IpSymScaledMatrix.cpp IpSymScaledMatrix.hpp \
	IpVector.cpp IpVector.hpp \
	IpZeroMatrix.cpp IpZeroMatrix.hpp

liblinalg_la_LDFLAGS = $(LT_LDFLAGS)

AM_CPPFLAGS = -I`$(CYGPATH_W) $(srcdir)/../Common`

# This line is necessary to allow VPATH compilation with MS compilers
# on Cygwin
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` -I$(top_builddir)/inc

# Astyle stuff

ASTYLE_FILES = IpBlas.cppbak IpBlas.hppbak \
	IpCompoundMatrix.cppbak IpCompoundMatrix.hppbak \
	IpCompoundSymMatrix.cppbak IpCompoundSymMatrix.hppbak \
	IpCompoundVector.cppbak IpCompoundVector.hppbak \
	IpDenseGenMatrix.cppbak IpDenseGenMatrix.hppbak \
	IpDenseSymMatrix.cppbak IpDenseSymMatrix.hppbak \
	IpDenseVector.cppbak IpDenseVector.hppbak \
	IpDiagMatrix.cppbak IpDiagMatrix.hppbak \
	IpExpansionMatrix.cppbak IpExpansionMatrix.hppbak \
	IpIdentityMatrix.cppbak IpIdentityMatrix.hppbak \
	IpLapack.cppbak IpLapack.hppbak \
	IpLowRankUpdateSymMatrix.cppbak IpLowRankUpdateSymMatrix.hppbak \
	IpMatrix.cppbak IpMatrix.hppbak \
	IpMultiVectorMatrix.cppbak IpMultiVectorMatrix.hppbak \
	IpScaledMatrix.cppbak IpScaledMatrix.hppbak \
	IpSumMatrix.cppbak IpSumMatrix.hppbak \
	IpSumSymMatrix.cppbak IpSumSymMatrix.hppbak \
	IpSymMatrix.cppbak IpSymMatrix.hppbak \
	IpSymScaledMatrix.cppbak IpSymScaledMatrix.hppbak \
	IpVector.cppbak IpVector.hppbak \
	IpZeroMatrix.cppbak IpZeroMatrix.hppbak

ASTYLE = @ASTYLE@
ASTYLEFLAGS = @ASTYLEFLAGS@

CLEANFILES = $(ASTYLE_FILES)

SUFFIXES = .cppbak .hppbak

astyle: $(ASTYLE_FILES)

.hpp.hppbak:
	mv $< $@
	$(ASTYLE) $(ASTYLEFLAGS) < $@ > $<
	touch $@

.cpp.cppbak:
	mv $< $@
	$(ASTYLE) $(ASTYLEFLAGS) < $@ > $<
	touch $@
