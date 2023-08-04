FROM ubuntu:latest

RUN apt update -y && apt upgrade -y
RUN apt install vim \
                nginx \
                build-essential \
                -y


EXPOSE 6969


RUN apt install nginx -y


COPY ./nginx.conf /etc/nginx/conf.d/default.conf


CMD ["nginx", "-g", "daemon off;"]