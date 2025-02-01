@echo off
cd offset\cs2_dumper

git pull origin main

if exist output\ (
    copy /Y output\*.hpp ..\ >nul 2>&1
) else (
    echo output/ directory not found.
)

cd ..\..

echo Offsets sync and file move completed.
pause