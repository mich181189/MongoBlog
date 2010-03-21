AC_PREREQ(2.59)
AC_INIT(MongoBlog, 0.01, michael181189@hotmail.com)
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_LIBTOOL
AC_C_INLINE
AX_PTHREAD
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

#checks for cgicc
AC_ARG_WITH(libcgicc,AC_HELP_STRING([--with-libcgicc=DIR],[specifies where to find libcgicc]),[libcgicc_dir="$withval"],[libcgicc_dir=""])
if test -d "$libcgicc_dir"; then
        CPPFLAGS="${CPPFLAGS} -I$libcgicc_dir/include"
        LDFLAGS="${LDFLAGS} -L$libcgicc_dir/lib"
        fi
AX_CXX_CHECK_LIB(cgicc, [cgicc::HTTPCookie])
#checks for boost
AX_BOOST_BASE([1.33.1])
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM
AX_BOOST_THREAD
AX_BOOST_PROGRAM_OPTIONS
CPPFLAGS="${CPPFLAGS} ${BOOST_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${BOOST_LDFLAGS}"
LIBS="${LIBS} $BOOST_THREAD_LIB $BOOST_FILESYSTEM_LIB $BOOST_SYSTEM_LIB $BOOST_PROGRAM_OPTIONS_LIB"
#checks for mongo client
AC_ARG_WITH(libmongoclient,AC_HELP_STRING([--with-libmongoclient=DIR],[specifies where to find libmongoclient]),[libcgicc_dir="$withval"],[libmongoclient_dir=""])
if test -d "$libmongoclient_dir"; then
        CPPFLAGS="${CPPFLAGS} -I$libmongoclient_dir/include"
        LDFLAGS="${LDFLAGS} -L$libmongoclient_dir/lib"
        fi
AX_CXX_CHECK_LIB(mongoclient, [mongo::Query::explain()])

AC_CONFIG_FILES([
Makefile 
bin/Makefile 
libs/Makefile 
])
AC_OUTPUT
