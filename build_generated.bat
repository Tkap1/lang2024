@echo off


pushd build
cl ..\output.c -W4 -nologo -link ..\raylibdll.lib
popd
build\output.exe