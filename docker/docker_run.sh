#!/bin/bash

IMG_NAME=webserv
DOCKERFILE_DIR=.

start_docker()
{
	# start the docker daemon
	if ! [ "$(pgrep Docker)" ]
	then
		echo "${blue}Starting docker daemon...${reset}"
		open -j -g -a Docker
	fi

	# wait for the daemon to start
	until docker info > /dev/null 2>&1
	do
		sleep 1
	done
	echo "${blue}Docker daemon started${reset}"
}

build_minilinux()
{
	# build the image
	echo "${blue}Building image...${reset}"
	docker build -t $IMG_NAME $DOCKERFILE_DIR
}

run_minilinux()
{
	# create the container
	echo "${blue}Creating container...${reset}"
	docker run -it -v ..:/host/ --name $IMG_NAME -e DOCKER_CONTAINER_NAME=$IMG_NAME $IMG_NAME bash
	# docker run -it -v $PWD:/host/ --name $IMG_NAME -e DOCKER_CONTAINER_NAME=$IMG_NAME $IMG_NAME zsh
}

launch_minilinux()
{
	# check if the container already exists
	if [ "$(docker image ls | grep $IMG_NAME)" ]
	then
		run_minilinux
	else
		build_minilinux
		run_minilinux
	fi
}

start_docker
launch_minilinux