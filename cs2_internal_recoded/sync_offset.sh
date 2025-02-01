#!/bin/bash

cd offset/cs2_dumper

git pull origin main

if [ -d "output" ]; then
    cp output/* . 2>/dev/null
fi

cd ../..