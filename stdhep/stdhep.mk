echo "build the stdhep library"
echo " be sure to setup stdhep and cern first"
# build stdhep library
 mkdir stdhep
 ypatchy - - stdinc tty :go
 ypatchy - stdhep.source stdhep tty :go
 cd stdhep
 fsplit ../stdhep.source

echo "compile stdhep"
 xlf -qextname -c -O *.f

echo "archive stdhep"
 ar cr $STDHEP_DIR/libstdhep.a *.o 

echo "make symbol list for the archive"
 ar ts $STDHEP_DIR/libstdhep.a

echo "clean out the subdirectory"
 cd ..
 rm -r stdhep
echo "exit procedure"
