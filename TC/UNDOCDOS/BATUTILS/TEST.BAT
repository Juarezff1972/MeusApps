@echo off
rem ***WEEKDAY**** get the weekday
write Today is:  
weekday
IF ERRORLEVEL 0 IF NOT ERRORLEVEL 1 ECHO Sunday
IF ERRORLEVEL 1 IF NOT ERRORLEVEL 2 ECHO Monday
IF ERRORLEVEL 2 IF NOT ERRORLEVEL 3 ECHO Tuesday
IF ERRORLEVEL 3 IF NOT ERRORLEVEL 4 ECHO Wednesday
IF ERRORLEVEL 4 IF NOT ERRORLEVEL 5 ECHO Thursday
IF ERRORLEVEL 5 IF NOT ERRORLEVEL 6 ECHO Friday
IF ERRORLEVEL 6 ECHO Saturday

rem ***GETHOUR****  Get the current hour for a greeting message
gethour
IF ERRORLEVEL  0 IF NOT ERRORLEVEL  6 ECHO Man, you up late.  GO TO BED!!!
IF ERRORLEVEL  6 IF NOT ERRORLEVEL 12 ECHO Good Morning
IF ERRORLEVEL 12 IF NOT ERRORLEVEL 18 ECHO Good Afternoon
IF ERRORLEVEL 18 ECHO Good Evening

rem ***WRITE.COM**** This 'writes' a line to the screen
rem                   without moving to the next line
rem                   like ECHO does.
write This is a test line to print using WRITE.COM
rem                  We do however want to do a CRLF
rem                   to get to the next line in this case.
echo .
echo ÿ

rem ***CLRKYBRD.COM**** Clear the keyboard buffer
clrkybrd

rem ***GETYN.COM**** get a Y or a N for a YES/NO answer
write Please enter a 'Y' or 'N':  
getyn
IF ERRORLEVEL 89 GOTO YES
echo o
goto getyndone
:yes
echo es
:getyndone
echo ÿ

rem ***REBOOT**** reboot the system
rem we obviously won't reboot the system here, but this is
rem  how you would do it.
rem  REBOOT.COM

rem ***GETDAY**** gets the day of the month
rem  if it is the day we want, run a system test or something.
getday
IF ERRORLEVEL 15 IF NOT ERRORLEVEL 16 ECHO doing something on the 15th
echo ÿ

rem ***TOLOW**** Renames all Long Filenames to lowercase
rem  *needs a DOS window under Windoze 9x
rem  *won't work in True DOS
tolow

rem ***BEEP**** sounds the computers speaker
beep

rem ***DRVRDY****  Is the disk drive ready
drvrdy a:
IF ERRORLEVEL 0 IF NOT ERRORLEVEL 1 ECHO Drive ready
IF ERRORLEVEL 255                   ECHO Drive NOT ready

