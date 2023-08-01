#!/bin/bash

IMG_NAME=webserv
DOCKERFILE_DIR=docker/
CMD=bash

start_docker()
{
  if ! [ "$(pgrep Docker)" ]
  then
    echo "Starting docker daemon..."
    open -j -g -a Docker
  fi

  until docker info > /dev/null 2>&1
  do
    sleep 1
  done
  echo "Docker daemon started"
}

build_minilinux()
{
  echo "Building image..."
  docker build -t $IMG_NAME $DOCKERFILE_DIR
}

run_minilinux()
{
  echo "Creating container..."
  docker run -it \
              -v $PWD:/host/ \
              --name $IMG_NAME \
              -e DOCKER_CONTAINER_NAME=$IMG_NAME $IMG_NAME \
              $CMD
}

launch_container()
{
  if [ "$(docker image ls | grep $IMG_NAME)" ]
  then
    run_minilinux
  else
    build_minilinux
    run_minilinux
  fi
}

clean_up()
{
  docker container rm $IMG_NAME
  echo "Cleaning up..."
  docker rmi $(docker images -f "dangling=true" -q) 2>/dev/null
}

start_docker
launch_container
clean_up
echo "Credits go to pfuchs, I just ripped it"
