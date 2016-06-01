#!/bin/bash

function refactor_depfile(filename) {
    if [ -f filename ]
    then
        sed -i "s/\\\/\//g" filepath
    fi
}

refactor_depfile "path"
