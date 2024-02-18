@echo off


pushd build
cl ..\output.c -W4 -wd 4244 -Zi -Od -nologo -link ..\raylib.lib
popd
build\output.exe