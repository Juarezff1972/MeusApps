@echo off

rem also try:
rem set ISEMPTY=readme.txt
rem to check a file that is not a null length file

set ISEMPTY=demo0.txt
isempty
@if ERRORLEVEL==255 goto notfound
@if ERRORLEVEL==254 goto iserror
@if ERRORLEVEL==1 goto fileisnull
@if ERRORLEVEL==0 goto fileisbig

:fileisbig
echo File is NOT a null sized file
goto end

:fileisnull
echo File is a null sized file
goto end

:notfound
echo Environment Var not found
goto end

:iserror
echo There was an error (file not found)

:end
rem The following line clears our environment var
set ISEMPTY=
