#!/bin/bash

if ! [[ -d "Calculator_v2/build" ]]; then
    make install
    tar -cvf $1 *
else
    tar -cvf $1 *
fi
