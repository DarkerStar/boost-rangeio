#
# Copyright (c) Mark A. Gibbs, 2015.
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
# 

tests_src := detail_formatting_saver.cpp \
             detail_write_all.cpp \
             detail_write_all_delim.cpp \
             write_iterator_range_imm.cpp \
             write_iterator_range_delim_imm.cpp

# Important settings for portability
SHELL := /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .hpp .o

# Add the working include directory to the include search path
CPPFLAGS := $(CPPFLAGS) -I../include

# Test lists
tests := $(patsubst %.cpp, %, $(tests_src))

runtests := $(addprefix run_, ${tests})

# Default make target (only makes all tests)
all : $(tests)

.PHONY : all

# Make 'check' target (makes and runs all tests)
${runtests}: run_% : %
	-./$*

check : ${runtests}

.PHONY : check ${runtests}

# Make 'clean' target
clean :
	-@rm -f *.o
	-@rm -f $(tests)

.PHONY : clean
