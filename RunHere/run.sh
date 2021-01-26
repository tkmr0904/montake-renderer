cmake ../ && make -s -j 8 && ./Source/a.out
rm -f -r ./CMakeFiles
rm -f -r ./Source
rm ./cmake_install.cmake
rm ./CMakeCache.txt
rm ./Makefile