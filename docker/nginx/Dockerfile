FROM ubuntu:latest

RUN apt update -y && apt upgrade -y
RUN apt install vim \
                nginx \
                build-essential \
                -y

EXPOSE 6969
EXPOSE 7070
EXPOSE 7171
EXPOSE 7272
EXPOSE 7373

RUN apt install nginx -y

COPY ./nginx.conf /etc/nginx/conf.d/default.conf

CMD ["nginx", "-g", "daemon off;"]
