@echo off


pushd build
cl ..\output.c -W4 -wd 4244 -nologo -link ..\raylib.lib
popd
build\output.exe