@echo off
::                             ==================
::                             Project evaluation
::                             ==================
::
::
:: Author:  Lukas Kekely <ikekely@fit.vutbr.cz>
:: Date:  1.12.2014

echo|set /p ="Results evaluation                      "

set EVAL_OK=True
%EXE_DIR%\sim.exe -c -s %WORK_DIR%\sim\chk_sim_sig_basic.txt -k %WORK_DIR%\sim\chk_gen_code_basic.txt -o %WORK_DIR%\chk_sim_result_basic.txt >%LOG_DIR%\eval_basic.txt 2>%LOG_DIR%\eval.log
if %errorlevel% neq 0 set EVAL_OK=False
%EXE_DIR%\sim.exe -c -s %WORK_DIR%\sim\chk_sim_sig_advanced.txt -k %WORK_DIR%\sim\chk_gen_code_advanced.txt -o %WORK_DIR%\chk_sim_result_advanced.txt >%LOG_DIR%\eval_advanced.txt 2>>%LOG_DIR%\eval.log
if %errorlevel% neq 0 set EVAL_OK=False
%EXE_DIR%\eval.exe -s %WORK_DIR%\chk_sim_result_advanced.txt -b %WORK_DIR%\chk_sim_result_basic.txt -l %LOGIN% -o %WORK_DIR%\chk_sim_output.txt >%WORK_DIR%\chk_sim_result.txt 2>>%LOG_DIR%\eval.log
if %errorlevel% neq 0 set EVAL_OK=False

del %WORK_DIR%\chk_sim_result_basic.txt >NUL 2>&1
del %WORK_DIR%\chk_sim_result_advanced.txt >NUL 2>&1
echo. >>%WORK_DIR%\chk_sim_output.txt
echo ############################################################## >>%WORK_DIR%\chk_sim_output.txt
echo # BASIC CODES PROBLEMS                                       # >>%WORK_DIR%\chk_sim_output.txt
echo ############################################################## >>%WORK_DIR%\chk_sim_output.txt
echo. >>%WORK_DIR%\chk_sim_output.txt
if %EVAL_OK% == True (type %LOG_DIR%\eval_basic.txt >>%WORK_DIR%\chk_sim_output.txt) else (echo Simulation error >>%WORK_DIR%\chk_sim_output.txt)
echo. >>%WORK_DIR%\chk_sim_output.txt
echo ############################################################## >>%WORK_DIR%\chk_sim_output.txt
echo # ADVANCED CODES PROBLEMS                                    # >>%WORK_DIR%\chk_sim_output.txt
echo ############################################################## >>%WORK_DIR%\chk_sim_output.txt
echo. >>%WORK_DIR%\chk_sim_output.txt
if %EVAL_OK% == True (type %LOG_DIR%\eval_advanced.txt >>%WORK_DIR%\chk_sim_output.txt) else (echo Simulation error >>%WORK_DIR%\chk_sim_output.txt)

if %EVAL_OK% == True (
    echo [Done]

    echo.
    echo ###############################################################################
    echo Evaluation results:
    echo --^> student %LOGIN%
    for /f "tokens=*" %%l in ('type %WORK_DIR%\chk_sim_result.txt') do echo --^> %%l
    del %WORK_DIR%\chk_sim_result.txt >NUL 2>&1
    echo ###############################################################################
) else (
    echo [Failed]

    echo.
    echo ###############################################################################
    echo Evaluation results:
    echo --^> student %LOGIN%
    echo --^> basic codes                         [     0/48 Failed]
    echo --^> advanced codes                      [   0/1XXX Failed]
    echo ###############################################################################
)
