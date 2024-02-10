@echo off


pushd build
cl ..\output.c -W4 -nologo
popd