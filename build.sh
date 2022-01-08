#!/bin/sh
set -e

#--------------------------------------------------------------------------------------------------
# Settings
#--------------------------------------------------------------------------------------------------

external="$PWD/../3rdparty"

#--------------------------------------------------------------------------------------------------

Qt4_version="4.8.7"
Qt5_version="5.15.2"
Qt6_version="6.2.1"

MinGW_version="8.1.0"

#--------------------------------------------------------------------------------------------------

make_arguments="-j 4"

#--------------------------------------------------------------------------------------------------
# Syntax
#--------------------------------------------------------------------------------------------------

if [ $# != 2 ] \
   || \
   [ $1 != "qt4" -a $1 != "qt5" -a $1 != "qt6" -a $1 != "clean" ] \
   || \
   [ $2 != "win32" -a $2 != "win64" ]; then

    echo "Usage: build <qt4 | qt5 | qt6 | clean> <win32 | win64>"

    exit 1
fi

#--------------------------------------------------------------------------------------------------
# Configuration
#--------------------------------------------------------------------------------------------------

external="$external/$2"

MinGW="$external/MinGW/$MinGW_version/bin"

if [ $1 = "qt4" ]; then

    Qt="$external/Qt/$Qt4_version"

elif [ $1 = "qt5" ]; then

    Qt="$external/Qt/$Qt5_version"
else
    Qt="$external/Qt/$Qt6_version"
fi

qmake="$Qt/bin/qmake.exe"

#--------------------------------------------------------------------------------------------------
# Clean
#--------------------------------------------------------------------------------------------------

if [ $1 = "clean" ]; then

    echo "CLEANING"

    # NOTE: We have to remove the folder to delete .qmake.stash.
    rm -rf build
    mkdir  build
    touch  build/.gitignore

    exit 0
fi

#--------------------------------------------------------------------------------------------------
# Build QuickWindow
#--------------------------------------------------------------------------------------------------

echo "BUILDING QuickWindow"
echo "--------------------"

export QT_SELECT="$1"

if [ $1 = "qt4" ]; then

    config="CONFIG+=release"
else
    config="CONFIG+=release qtquickcompiler"
fi

spec=win32-g++

PATH="$Qt/bin:$MinGW:$PATH"

$qmake --version
echo ""

cd build

$qmake -r -spec $spec "$config" ..

mingw32-make $make_arguments

echo "--------------------"
