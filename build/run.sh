cmake ../ && make -s -j 8 && ./src/a.out
rm -f -r ./CMakeFiles
rm -f -r ./src
rm ./cmake_install.cmake
rm ./CMakeCache.txt
rm ./Makefile