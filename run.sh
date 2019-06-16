#!/bin/bash

PARAM=$1

pushd .

cd bin

if [[ "$PARAM" == "debug" ]] ; then
	cgdb ./spaceGame.bin
else
	./spaceGame.bin
fi

popd

