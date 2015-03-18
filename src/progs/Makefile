# The following applies to this software package and all subparts therein
#
# Cognosco Copyright (C) 2015 Philip J. Uren
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

###############################################################################
#                           locations of sub-modules                          #
###############################################################################
IO_MODULE_DIR = ../io
UI_MODULE_DIR = ../ui
CORE_MODULE_DIR = ../core
UTIL_MODULE_DIR = ../util
CLUSTERING_MODULE_DIR = ../clustering
CLASSIFICATION_MODULE_DIR = ../classification


###############################################################################
#          PROGRAMS LIST -- THESE ARE THE THINGS THAT WILL BE BUILT           #
###############################################################################
PROGS = Classify Cluster


###############################################################################
#                                COMPILER FLAGS                               #
###############################################################################
CXX = g++
CFLAGS = -Wall -fPIC -fmessage-length=50 -std=c++11
OPTFLAGS = -O3
DEBUGFLAGS = -g
#LIBS = -lgsl -lgslcblas @bamlibdir@ @bamlib@

INCLUDEDIRS = $(IO_MODULE_DIR) $(CORE_MODULE_DIR) $(UI_MODULE_DIR) \
              $(CLASSIFICATION_MODULE_DIR) $(CLUSTERING_MODULE_DIR) \
              $(UTIL_MODULE_DIR)
INCLUDEARGS = $(addprefix -I,$(INCLUDEDIRS))

ifdef DEBUG
CFLAGS += $(DEBUGFLAGS)
endif

ifdef OPT
CFLAGS += $(OPTFLAGS)
endif


###############################################################################
#                          GENERAL COMPILATION RULES                          #
###############################################################################

%.o: %.cpp %.hpp
	$(CXX) $(CFLAGS) -c -o $@ $< $(INCLUDEARGS)

%.o: %.cpp %.h
	$(CXX) $(CFLAGS) -c -o $@ $< $(INCLUDEARGS)

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDEARGS) $(LIBS)


###############################################################################
#                      DEPENDENCIES FOR INDIVIDUAL PROGS                      #
###############################################################################

Classify: $(addprefix $(CORE_MODULE_DIR)/, Dataset.o Attribute.o Instance.o \
                                           MisclassificationCostMatrix.o) \
          $(addprefix $(IO_MODULE_DIR)/, CSVLoader.o) \
          $(addprefix $(UTIL_MODULE_DIR)/, StringUtils.o) \
          $(addprefix $(CLASSIFICATION_MODULE_DIR)/, NaiveBayes.o \
                                                     KMedoidsClassifier.o \
                                                     DecisionStump.o \
                                                     Random.o) \
          $(addprefix $(UI_MODULE_DIR)/, CLI.o) \
          $(addprefix $(CLUSTERING_MODULE_DIR)/, KMedoids.o)

Cluster:  $(addprefix $(IO_MODULE_DIR)/, PairwiseDistanceLoader.o) \
          $(addprefix $(UTIL_MODULE_DIR)/, StringUtils.o) \
          $(addprefix $(CLUSTERING_MODULE_DIR)/, KMedoids.o)


###############################################################################
#                                PHONY TARGETS                                #
###############################################################################

all: $(PROGS)
.PHONY: all

install: all
	@mkdir -p ../../bin
	@install -m 755 $(PROGS) ../../bin
.PHONY: install

clean:
	@-rm -f $(PROGS) *.o *.so *.a *~
.PHONY: clean

distclean : clean
	@rm Makefile
.PHONY: distclean

test:
	make -C ../test test;
.PHONY: test
