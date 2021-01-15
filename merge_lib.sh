#!/bin/sh
if [ $# -gt 2 ]
    then
    echo "\n" # print 2 empty lines at the start

    out=$1 # copy output file name to variable
    shift # shift arguments by 1 to get rid of output file name

    mkdir -p __tmp # make temporary directory
    cd __tmp # change to temporary directory

#        for var in ${@:2}
#        do
#            #echo "argument in for loop: " $var
#            ar -x ../$var
#        done
        while [ $# -gt 0 ] # while there are libraries to process
        do
            echo "extracting library $1" # argv[1] is the library that should be extracted, once it is handled, shift gets rid of the argument
            ar -x ../$1 # extracting the current library
            shift # getting rid of extracted library
        done
        echo "combining libraries into $out"
        ar -rc ../$out *.o
    cd .. # leave temporary directory

    rm -r __tmp # remove temporary directory with content
    echo "\n" # print 2 empty lines at the end
else
    echo "usage:" $0 "<lib (out)>" "<lib (in)>" "<lib (in)> ..."
fi
