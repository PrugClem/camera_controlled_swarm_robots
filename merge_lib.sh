#!/bin/sh
if [ $# -gt 2 ]
    then
    mkdir -p __tmp
    cd __tmp

        for var in ${@:2}
        do
            #echo "argument in for loop: " $var
            ar -x ../$var
        done

        ar -rc ../$1 *.o
    cd ..

    rm -r __tmp
else
    echo "usage:" $0 "<lib (out)>" "<lib (in)>" "<lib (in)> ..."
fi
