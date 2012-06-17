# Initial spec file created by autospec ver. 0.7 with rpm 2.5 compatibility
Summary: Multiplatform (X)HTML preprocessor written in C
# The Summary: line should be expanded to about here -----^
Name: hsc
Version: 1.0
Release: 1
Group: Applications/Internet
Copyright: GPL
Source: http://www.linguistik.uni-erlangen.de/~msbethke/binaries/hsc-1.0.tar.bz2
# Following are optional fields
URL: http://www.linguistik.uni-erlangen.de/~msbethke/software.html
#Patch: source.patch
Prefix: /usr/local
#BuildArchitectures: noarch
#Requires: 
#Obsoletes: 

%description
HSC, or ``HTML Sucks Completely'' for long, is an HTML preprocessor featuring
include files, macros, expressions, conditionals and a basic syntax checker. It
is designed for experienced web authors and supports project-management tools
such as ``make'' and ``rcs''. HSC comes with an extensive library of macros
that support advanced features like interfaces to external programs,
semi-automatic table generation, SQL databases etc.

%prep
%setup -n hsc-1.0

%build
#CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr/local
make

%install
make install

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf "$RPM_BUILD_ROOT"

%files
%defattr(-,root,root)
/usr/local/bin/hsc
/usr/local/bin/hscdepp
/usr/local/bin/hscpitt

%config
/usr/local/lib/hsc.prefs

%doc
/usr/local/share/doc/hsc/README
/usr/local/share/doc/hsc/CHANGES 
/usr/local/share/doc/hsc/docs/options.html
/usr/local/share/doc/hsc/docs/envvar.html
/usr/local/share/doc/hsc/docs/image/nocopy.png
/usr/local/share/doc/hsc/docs/image/noprev.png
/usr/local/share/doc/hsc/docs/image/noback.png
/usr/local/share/doc/hsc/docs/image/nonext.png
/usr/local/share/doc/hsc/docs/image/niceguy.png
/usr/local/share/doc/hsc/docs/image/main.png
/usr/local/share/doc/hsc/docs/image/austria.png
/usr/local/share/doc/hsc/docs/image/teutsch.png
/usr/local/share/doc/hsc/docs/image/copy.png
/usr/local/share/doc/hsc/docs/image/index.png
/usr/local/share/doc/hsc/docs/image/english.png
/usr/local/share/doc/hsc/docs/image/prev.png
/usr/local/share/doc/hsc/docs/image/up.png
/usr/local/share/doc/hsc/docs/image/back.png
/usr/local/share/doc/hsc/docs/image/next.png
/usr/local/share/doc/hsc/docs/image/hsc.png
/usr/local/share/doc/hsc/docs/image/nomain.png
/usr/local/share/doc/hsc/docs/image/noindex.png
/usr/local/share/doc/hsc/docs/macro/attrib.html
/usr/local/share/doc/hsc/docs/macro/flag.html
/usr/local/share/doc/hsc/docs/macro/macros.html
/usr/local/share/doc/hsc/docs/future.html
/usr/local/share/doc/hsc/docs/others.html
/usr/local/share/doc/hsc/docs/examples.html
/usr/local/share/doc/hsc/docs/ports.html
/usr/local/share/doc/hsc/docs/updates.html
/usr/local/share/doc/hsc/docs/index.html
/usr/local/share/doc/hsc/docs/install.html
/usr/local/share/doc/hsc/docs/author.html
/usr/local/share/doc/hsc/docs/about.html
/usr/local/share/doc/hsc/docs/bugs.html
/usr/local/share/doc/hsc/docs/teutsch.html
/usr/local/share/doc/hsc/docs/undocumented.html
/usr/local/share/doc/hsc/docs/features/spcattr.html
/usr/local/share/doc/hsc/docs/features/expressions.html
/usr/local/share/doc/hsc/docs/features/prefs.html
/usr/local/share/doc/hsc/docs/features/checkuri.html
/usr/local/share/doc/hsc/docs/features/syntax.html
/usr/local/share/doc/hsc/docs/features/strip.html
/usr/local/share/doc/hsc/docs/features/rplcent.html
/usr/local/share/doc/hsc/docs/features/exec.html
/usr/local/share/doc/hsc/docs/features/spctags.html
/usr/local/share/doc/hsc/docs/features/assign.html
/usr/local/share/doc/hsc/docs/features/if.html
/usr/local/share/doc/hsc/docs/features/uris.html
/usr/local/share/doc/hsc/docs/features/getsize.html
/usr/local/share/doc/hsc/docs/source.html
/usr/local/share/doc/hsc/docs/usecases.html
/usr/local/share/doc/hsc/docs/copy.html
/usr/local/share/doc/hsc/docs/messages.html
/usr/local/share/doc/hsc/docs/distrib.html
/usr/local/share/doc/hsc/docs/related.html
/usr/local/share/doc/hsc/docs/fileargs.html
/usr/local/share/doc/hsc/docs/require.html
/usr/local/share/doc/hsc/docs/changes.html
/usr/local/share/doc/hsc/docs/questions.html
/usr/local/share/doc/hsc/docs/project/make.html
/usr/local/share/doc/hsc/docs/project/hscpaltrow.html
/usr/local/share/doc/hsc/docs/project/index.html
/usr/local/share/doc/hsc/docs/project/hscpitt.html
/usr/local/share/doc/hsc/docs/project/hscdepp.html
/usr/local/share/doc/hsc/docs/project/prjfile.html
/usr/local/share/doc/hsc/docs/project/makefile.html
/usr/local/share/doc/hsc/docs/message-list.html

%changelog
* Fri Nov 05 2004 Matthias Bethke <bethke@linguistik.uni-erlangen.de>
- Updated to V1.0
* Wed Feb 05 2004 Matthias Bethke <bethke@linguistik.uni-erlangen.de>
- Updated to V0.935
* Wed Feb 05 2004 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.934
* Wed Jan 07 2004 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.933
* Wed Dec 24 2003 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.932
* Wed Aug 09 2003 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.931
* Mon Apr 28 2003 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.930
* Wed Apr 03 2003 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.929
* Mon Nov 18 2002 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.928
- Changed installation prefix to /usr/local
* Wed Nov 06 2002 Matthias Bethke <mb@matthias-bethke.de>
- Updated to V0.927
* Wed Sep 25 2002 Matthias Bethke <mb@matthias-bethke.de>
- Initial spec file created by autospec ver. 0.7 with rpm 2.5 compatibility
