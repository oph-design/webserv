#!/bin/bash

docker_destination="/goinfre/$USER/docker"

pkill Docker

if [ -d "$docker_destination" ]
then
  read -n1 -p "Folder $docker_destination already exists, \
                do you want to reset it? [y/N] " input
  echo ""
  if [ -n "$input" ] && [ "$input" = "y" ]; then
    rm -rf "$docker_destination"/{com.docker.{docker,helper},.docker} \
        &>/dev/null
  fi
fi

unlink ~/Library/Containers/com.docker.docker &>/dev/null
unlink ~/Library/Containers/com.docker.helper &>/dev/null
unlink ~/.docker &>/dev/null

rm -rf ~/Library/Containers/com.docker.{docker,helper} ~/.docker &>/dev/null

mkdir -p "$docker_destination"/{com.docker.{docker,helper},.docker}

ln -sf "$docker_destination"/com.docker.docker \
          ~/Library/Containers/com.docker.docker
ln -sf "$docker_destination"/com.docker.helper \
          ~/Library/Containers/com.docker.helper
ln -sf "$docker_destination"/.docker ~/.docker

echo "Docker now successfully is in goinfree"

open -g -a Docker
