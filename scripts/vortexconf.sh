#!/bin/bash -ev

mkdir -p ~/.vortex
echo "rpcuser=username" >>~/.vortex/vortex.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.vortex/vortex.conf

