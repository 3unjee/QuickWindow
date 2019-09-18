#!/bin/sh
set -e

#--------------------------------------------------------------------------------------------------
# Settings
#--------------------------------------------------------------------------------------------------

QuickWindow="../.."

external="$PWD/../3rdparty"

#--------------------------------------------------------------------------------------------------

Qt4_version="4.8.7"
Qt5_version="5.12.3"

MinGW_version="7.3.0"

#--------------------------------------------------------------------------------------------------

make_arguments="-j 4"

#--------------------------------------------------------------------------------------------------
# Syntax
#--------------------------------------------------------------------------------------------------

if [ $# != 2 ] \
   || \
   [ $1 != "qt4" -a $1 != "qt5" -a $1 != "clean" ] \
   || \
   [ $2 != "win32" -a $2 != "win64" -a $2 != "macOS" -a $2 != "linux" ]; then

    echo "Usage: build <qt4 | qt5 | clean> <win32 | win64>"

    exit 1
fi

#--------------------------------------------------------------------------------------------------
# Configuration
#--------------------------------------------------------------------------------------------------

external="$external/$2"

MinGW="$external/MinGW/$MinGW_version/bin"

if [ $1 = "qt4" ]; then

    Qt="$external/Qt/$Qt4_version"
else
    Qt="$external/Qt/$Qt5_version"
fi

qmake="$Qt/bin/qmake.exe"

#--------------------------------------------------------------------------------------------------
# Clean
#--------------------------------------------------------------------------------------------------

if [ $1 = "clean" ]; then

    echo "CLEANING"

    rm -rf build/qt4/*
    rm -rf build/qt5/*

    touch build/qt4/.gitignore
    touch build/qt5/.gitignore

    exit 0
fi

#--------------------------------------------------------------------------------------------------
# Building QuickWindow
#--------------------------------------------------------------------------------------------------

echo "BUILDING MotionBox"
echo "------------------"

if [ $1 = "qt4" ]; then

    QT_SELECT=qt4

    build="build/qt4"

    config="CONFIG += release"
else
    QT_SELECT=qt5

    build="build/qt5"

    config="CONFIG += release qtquickcompiler"
fi

spec=win32-g++

PATH="$Qt/bin:$MinGW:$PATH"

$qmake --version
echo ""

cd $build

$qmake -r -spec $spec "$config" $QuickWindow

mingw32-make $make_arguments

echo "------------------"
