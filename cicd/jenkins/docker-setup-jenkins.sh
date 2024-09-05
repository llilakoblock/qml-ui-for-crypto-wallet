#!/bin/bash

echo "1/2: Build image - jenkins-freedom ..."

docker image rm -f jenkins-freedom:lts
docker build -t jenkins-freedom:lts -f Dockerfile.jenkins .

echo "2/2: Run container - jenkins-freedom ..."

docker run \
    --name jenkins-freedom \
    --rm \
    --detach \
    --privileged \
    --volume /var/run/docker.sock:/var/run/docker.sock \
    --volume jenkins-data:/var/jenkins_home \
    jenkins-freedom:lts
