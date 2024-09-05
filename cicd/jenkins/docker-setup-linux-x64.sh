#!/bin/bash

echo "Build image - linux-x64-freedom ..."

cd ../../
rm -rf ./.build/*
git submodule update --init

docker image rm linux-x64-freedom:latest
docker build -t linux-x64-freedom:latest -f cicd/jenkins/Dockerfile.linux-x64 .

