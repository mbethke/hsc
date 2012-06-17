# Copyright 1999-2005 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

DESCRIPTION="An HTML preprocessor using ML syntax"
HOMEPAGE="http://www.linguistik.uni-erlangen.de/~msbethke/software.html"
SRC_URI="http://www.linguistik.uni-erlangen.de/~msbethke/binaries/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~x86 ~ppc ~amd64"

IUSE=""

DEPEND="virtual/libc"

src_compile() {
	./configure --prefix=${D}/usr/ || die
	make || die
}

src_install() {
	dodir /usr/bin
	dodir /usr/lib
	dodir /usr/share
	#This makefile uses INSTDIR instead of DESTDIR
	make install || die
}

pkg_postinst() {
	einfo Documentation and examples for HSC are available in
	einfo /usr/share/doc/${P}/
}
