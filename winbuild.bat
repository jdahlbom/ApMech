@ECHO OFF

REM Work in progress, ei todellakaan toimi viel. -J

REM BASEDIR voisi olla esim c:\programming tms.

set BASEDIR=d:

set SDLDIR=%BASEDIR%\SDL-1.2.13
set SDLTTFDIR=%BASEDIR%\SDL_ttf-2.0.9
set OGREDIR=%BASEDIR%\OgreSDK
set MINGWDIR=%BASEDIR%\CodeBlocks\MinGW
set BOOSTDIR=%BASEDIR%\boost_1_40_0
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

set CXXFLAGS=-I. -I..\src -I%SDLDIR%\include\SDL -I%OGREDIR%\include -I%OGREDIR%\include\CEGUI -I%BOOSTDIR%
set LDFLAGS=-L. -L%SDLDIR%\lib -lmingw32 -lSDLmain -lSDL -lenet -lws2_32 -lwinmm
%CXX% -dumpversion
if NOT %ERRORLEVEL% == 9009 goto WORK2

 echo No compiler found. You need to have mingw binaries in path.
 goto :END

:WORK2

if exist obj rd /s/q obj
if exist bin rd /s/q bin
md bin obj

xcopy /e jonni\nabla-wing\winlibs\* obj
xcopy %SDLDIR%\bin\SDL.dll bin\

cd obj

echo ************************************* Compiling net:
%CXX% -c %CXXFLAGS% ../src/net/*.cpp
echo ************************************* Compiling ooinput:
%CXX% -c %CXXFLAGS% ../src/ooinput/*.cpp
echo ************************************* Compiling math:
%CXX% -c %CXXFLAGS% ../src/math/*.cpp
echo ************************************* Compiling server:
%CXX% -c %CXXFLAGS% ../src/server/*.cpp
echo ************************************* Compiling src:
%CXX% -c %CXXFLAGS% ../src/*.cpp
echo ************************************* Linking:
%CXX% *.o -o ../apmech.exe %LDFLAGS%

cd ..

:END
