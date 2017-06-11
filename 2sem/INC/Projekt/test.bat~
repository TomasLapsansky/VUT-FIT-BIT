@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
:: Main project testing script
::
:: Author: Lukas Kekely <ikekely@fit.vutbr.cz>
:: Date: 1.12.2014

set LOGIN=xadamc04



set WORK_DIR=work
set LOG_DIR=%WORK_DIR%\log
set EXE_DIR=bin
set KIT_DIR=fitkit
set SYNTH_DIR=synth
set TB_DIR=vhdl
set MCU_DIR=app



if [%1] == [] (
    call scripts\unpack.bat
    call scripts\synth.bat
    call scripts\simulate.bat
    call scripts\runkit.bat
    call scripts\eval.bat
)

if [%1] == [clean] (
    rd /s /q work >NUL 2>NUL
)
