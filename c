#!/bin/bash

gcc hash_table.c main.c -o hash_table.out

if [ $? -eq 0 ]; then
    ./hash_table.out
fi
