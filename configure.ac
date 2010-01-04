AC_PREREQ(2.61)
AC_INIT(MongoBlog, 0.01, michael181189@hotmail.com)
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_LIBTOOL
AC_C_INLINE
#checks for cgicc
AC_ARG_WITH(libcgicc,AC_HELP_STRING([--with-libcgicc=DIR],[specifies where to find libcgicc]),[libcgicc_dir="$withval"],[libcgicc_dir=""])
if test -d "$libcgicc_dir"; then
        CPPFLAGS="${CPPFLAGS} -I$libcgicc_dir/include"
        LDFLAGS="${LDFLAGS} -L$libcgicc_dir/lib"
        fi
AX_CXX_CHECK_LIB(cgicc, [cgicc::HTTPCookie])
#checks for boost
#can't test for this one
LIBS="-lboost_filesystem -lboost_system-mt $LIBS"
AC_ARG_WITH(boost,AC_HELP_STRING([--with-boost=DIR],[specifies where to find boost]),[boost_dir="$withval"],[boost_dir=""])
if test -d "$boost_dir"; then
        CPPFLAGS="${CPPFLAGS} -I$boost_dir/include"
        LDFLAGS="${LDFLAGS} -L$boost_dir/lib"
        fi
AX_CXX_CHECK_LIB(boost_thread, [boost::thread::join()])
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
libs/Template/Makefile
])
AC_OUTPUT
