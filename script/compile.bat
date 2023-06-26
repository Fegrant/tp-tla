@echo off
setlocal enabledelayedexpansion

set "ROOT=%~dp0"
echo Relocating script to '%ROOT%'...
cd /d "%ROOT%.."

type "%~1" | .\bin\Compiler.exe

python output.py
