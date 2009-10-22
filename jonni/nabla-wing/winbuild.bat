@ECHO OFF

REM BASEDIR voisi olla esim c:\programming tms.

set BASEDIR=d:

set SDLDIR=%BASEDIR%\SDL-1.2.13
set SDLTTFDIR=%BASEDIR%\SDL_ttf-2.0.9
set MINGWDIR=%BASEDIR%\CodeBlocks\MinGW
set CXX=g++

set PATH=%MINGWDIR%\bin;%PATH%

if /i not [%1] == [distclean] goto WORK0

rd /s/q obj bin
goto END

:WORK0

if not exist %SDLDIR%\bin\SDL.dll set SDLDIR=%1
if exist %SDLDIR%\bin\SDL.dll goto WORK1

 echo SDL directory not found. Please give your SDL directory
 echo as an argument as follows (or modify this script!):
 echo winbuild.bat c:\somewhere\SDL-1.2.13
 goto :END

:WORK1

set CXXFLAGS=-I. -I..\src -I%SDLDIR%\include\SDL -I%SDLTTFDIR%\include
set LDFLAGS=-L. -L%SDLDIR%\lib -lmingw32 -lSDLmain -lSDL -lenet -lws2_32 -lwinmm -L%SDLTTFDIR%\lib -lSDL_ttf
%CXX% -dumpversion
if NOT %ERRORLEVEL% == 9009 goto WORK2

 echo No compiler found. You need to have mingw binaries in path.
 goto :END

:WORK2

if exist obj rd /s/q obj
if exist bin rd /s/q bin
md bin obj

xcopy /e winlibs\* obj
xcopy %SDLDIR%\bin\SDL.dll bin\
xcopy %SDLTTFDIR%\lib\SDL_ttf.dll bin\
xcopy %SDLTTFDIR%\lib\zlib1.dll bin\
xcopy "%PROGRAMFILES%\GnuWin32\bin\freetype6.dll" bin\
xcopy forgotten.ttf bin\
move bin\freetype6.dll bin\libfreetype-6.dll

cd obj

%CXX% -c %CXXFLAGS% ../src/*.cpp
%CXX% -c %CXXFLAGS% ../src/net/*.cpp
%CXX% *.o -o ../bin/nablawing.exe %LDFLAGS%

cd ..

REM Enet k‰‰ntyy k‰sin n‰in mingw:ll‰, mutta lopputulos ei toimi ;)
REM	c++ -c -I. *.c
REM	ar rcs enet.lib *.o

:END
