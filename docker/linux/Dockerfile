FROM ubuntu:latest

RUN apt update -y && apt upgrade -y
RUN apt install vim \
                valgrind \
                build-essential \
                -y

EXPOSE 1234

WORKDIR /host/
