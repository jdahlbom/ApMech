@ECHO OFF

set SDLDIR=c:\programming\SDL-1.2.13
set CXX=g++

if not exist %SDLDIR%\bin\SDL.dll set SDLDIR=%1
if exist %SDLDIR%\bin\SDL.dll goto WORK1

 echo SDL directory not found. Please give your SDL directory
 echo as an argument as follows (or modify this script!):
 echo winbuild.bat c:\somewhere\SDL-1.2.13
 goto :END

:WORK1

set CXXFLAGS=-I. -I..\src -I%SDLDIR%\include\SDL
set LDFLAGS=-L. -L%SDLDIR%\lib -lmingw32 -lSDLmain -lSDL -lenet -lws2_32 -lwinmm
%CXX% -dumpversion
if %ERRORLEVEL% == 1 goto WORK2

 echo No compiler found. You need to have mingw binaries in path.
 goto :END

:WORK2

rd /s/q obj bin
md bin obj

xcopy /e winlibs\* obj
xcopy %SDLDIR%\bin\SDL.dll bin\

cd obj

%CXX% -c %CXXFLAGS% ../src/*.cpp
%CXX% *.o -o ../bin/netdata.exe %LDFLAGS%

cd ..

REM Enet k‰‰ntyy k‰sin n‰in mingw:ll‰, mutta lopputulos ei toimi ;)
REM	c++ -c -I. *.c
REM	ar rcs enet.lib *.o

:END