#!/bin/bash

gcc -o phoenix src/*.c -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./phoenix
