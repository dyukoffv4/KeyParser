#!/usr/bin/bash
if [[ $# == 0 ]]; then exit 1; fi

if [[ $1 == "build" ]]; then F_BUILD=true
elif [[ $1 == "clean" ]]; then F_CLEAN=true
elif [[ $1 == "prune" ]]; then F_PRUNE=true; F_CLEAN=true
else exit 1; fi

LSRC=lib
LNAME=keyparser
PNAME=package

if [ $F_BUILD ]
then
    cd $LSRC
    bash build.sh $LNAME $PNAME
    if [[ $? == 0 ]]
    then
        if [[ $(dpkg -l | grep $LNAME) ]]; then sudo apt reinstall ./$PNAME.deb
        else sudo apt install ./$PNAME.deb; fi
    fi
    cd ..
elif [ $F_CLEAN ]
then
    echo "Clearing binary ..."
    rm -f $LSRC/bin/*.*o
    if [[ $F_PRUNE ]]
    then
        echo "Clearing packages ..."
        rm -f $LSRC/*.deb
        if [[ $(dpkg -l | grep $LNAME) ]]; then sudo apt remove $LNAME; fi
    fi
fi

# PERSONAL CODE
SRC=example
NAME=PE

if [ $F_BUILD ]; then echo "Example assembly ..."; make -C $SRC LNAME=$LNAME NAME=$NAME >&-; fi
if [ $F_CLEAN ]; then echo "Example clean ..."; make -C $SRC LNAME=$LNAME clean >&-; fi
if [ $F_PRUNE ]; then echo "Example prune ..."; rm -f ./$SRC/bin/$NAME; fi
