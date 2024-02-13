@echo off


pushd build
cl ..\output.c -W4 -nologo -link ..\raylib.lib
popd
build\output.exe