AC_PREREQ(2.61)
AC_INIT(MongoBlog, 0.01, michael181189@hotmail.com)
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_LIBTOOL
AC_C_INLINE
#checks for cgicc
AC_ARG_WITH(libcgicc,AC_HELP_STRING([--with-libcgicc=DIR],[specifies where to find libm]),[libcgicc_dir="$withval"],[libcgicc_dir=""])
if test -d "$libcgicc_dir"; then
        CPPFLAGS="${CPPFLAGS} -I$libcgicc_dir/include"
        LDFLAGS="${LDFLAGS} -L$libcgicc_dir/lib"
        fi
AX_CXX_CHECK_LIB(cgicc, [cgicc::HTTPCookie])

AC_CONFIG_FILES([
Makefile 
bin/Makefile 
libs/Makefile 
libs/Template/Makefile
])
AC_OUTPUT
