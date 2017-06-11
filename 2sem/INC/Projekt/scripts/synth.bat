@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
::                    ====================================
::                    Synthesis and compilation of project
::                    ====================================
::
::
:: Author:  Lukas Kekely <ikekely@fit.vutbr.cz>
:: Date:  1.12.2014

echo|set /p ="Project synthesis                       "

md %WORK_DIR%\synth >NUL 2>&1
xcopy /s %SYNTH_DIR% %WORK_DIR%\synth >NUL

cd %WORK_DIR%\synth
md xst
md xst\projnav.tmp
xst -intstyle ise -ifn "fsm.xst" -ofn "fsm.syr" >..\..\%LOG_DIR%\synth.log 2>&1
cd ..\..

if not exist %WORK_DIR%\synth\fsm.ngc echo [Failed] & exit /b 1
echo [Done]
