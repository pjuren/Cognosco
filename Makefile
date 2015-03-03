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

package = Cognosco
version = 0.0.1
tarname = $(package)
distdir = $(tarname)-$(version)

BINDIR = ./bin

all:
	@make -C src OPT=1 install

install:
	@make -C src OPT=1 install

test:
	@make -C src OPT=1 test
.PHONY: test

clean:
	@rm -rf $(BINDIR)
	@make -C src clean
.PHONY: clean
