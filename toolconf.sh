#!/bin/bash
rm aclocal.m4
libtoolize --force
aclocal -I aclocal
automake --add-missing
autoconf
