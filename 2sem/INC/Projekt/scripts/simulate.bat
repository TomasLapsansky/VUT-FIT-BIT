@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
::                   ======================================
::                   Project simulation in ModelSim or ISim
::                   ======================================
::
::
:: Author:  Lukas Kekely <ikekely@fit.vutbr.cz>
:: Date:  1.12.2014

set SIMULATOR=ISim
::set SIMULATOR=ModelSim

echo Simulating project in %SIMULATOR%:

taskkill /im vlm.exe /f >NUL 2>&1
md %WORK_DIR%\sim >NUL 2>&1
xcopy /s %TB_DIR% %WORK_DIR%\sim >NUL

set "CODES=1234#,12345#"
for /f "tokens=*" %%i in ('%EXE_DIR%\gen-code.exe -l %LOGIN%') do set "CODES=%%i"



echo|set /p ="--> basic codes                         "

%EXE_DIR%\sim.exe -g -b -l %CODES% -k %WORK_DIR%\sim\chk_gen_code_basic.txt -p %WORK_DIR%\sim\testbench_pkg.vhd -s chk_sim_sig_basic.txt

cd %WORK_DIR%\sim
if [%SIMULATOR%] == [ModelSim] (
    vsim -c -do testbench.fdo >..\..\%LOG_DIR%\sim_basic.log 2>&1
) else (
    xtclsh testbench_sim.tcl >..\..\%LOG_DIR%\sim_basic.log 2>&1
)
cd ..\..

copy %WORK_DIR%\sim\testbench_pkg.vhd %WORK_DIR%\sim\testbench_pkg_basic.vhd >NUL 2>&1
del %WORK_DIR%\sim\testbench_pkg.vhd >NUL 2>&1
del %WORK_DIR%\sim\transcript >NUL 2>&1
del %WORK_DIR%\sim\vsim.wlf >NUL 2>&1
rd /s /q %WORK_DIR%\sim\work >NUL 2>&1
del %WORK_DIR%\sim\fuse* >NUL 2>&1
del %WORK_DIR%\sim\isim.log >NUL 2>&1
del %WORK_DIR%\sim\isim.wdb >NUL 2>&1
del %WORK_DIR%\sim\*.exe >NUL 2>&1
rd /s /q %WORK_DIR%\sim\isim >NUL 2>&1

if not exist %WORK_DIR%\sim\chk_sim_sig_basic.txt echo [Failed] & exit /b 1
echo [Done]



echo|set /p ="--> advanced codes                      "

%EXE_DIR%\sim.exe -g -l %CODES% -k %WORK_DIR%\sim\chk_gen_code_advanced.txt -p %WORK_DIR%\sim\testbench_pkg.vhd -s chk_sim_sig_advanced.txt

cd %WORK_DIR%\sim
if [%SIMULATOR%] == [ModelSim] (
    vsim -c -do testbench.fdo >..\..\%LOG_DIR%\sim_advanced.log 2>&1
) else (
    xtclsh testbench_sim.tcl >..\..\%LOG_DIR%\sim_advanced.log 2>&1
)
cd ..\..

copy %WORK_DIR%\sim\testbench_pkg.vhd %WORK_DIR%\sim\testbench_pkg_advanced.vhd >NUL 2>&1
del %WORK_DIR%\sim\testbench_pkg.vhd >NUL 2>&1
del %WORK_DIR%\sim\transcript >NUL 2>&1
del %WORK_DIR%\sim\vsim.wlf >NUL 2>&1
rd /s /q %WORK_DIR%\sim\work >NUL 2>&1
del %WORK_DIR%\sim\fuse* >NUL 2>&1
del %WORK_DIR%\sim\isim.log >NUL 2>&1
del %WORK_DIR%\sim\isim.wdb >NUL 2>&1
del %WORK_DIR%\sim\*.exe >NUL 2>&1
rd /s /q %WORK_DIR%\sim\isim >NUL 2>&1

if not exist %WORK_DIR%\sim\chk_sim_sig_advanced.txt echo [Failed] & exit /b 1
echo [Done]