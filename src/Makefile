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

app_subdirs=progs
module_subdirs=ui core classification clustering util io

all:
	@for i in $(app_subdirs); do \
	        make -C $${i} OPT=1; \
	done;

install:
	@for i in $(app_subdirs); do \
	        make -C $${i} OPT=1 install; \
	done;

test:
	@for i in $(app_subdirs); do \
	        make -C test; \
	done;
.PHONY: test

clean:
	@for i in $(app_subdirs); do \
	        make -C $${i} clean; \
	done
	@for i in $(module_subdirs); do \
	        make -C $${i} clean; \
	done;
