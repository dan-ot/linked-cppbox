FROM alpine

RUN apk update
RUN apk add wget unzip
RUN apk add build-base cmake
RUN apk add asio-dev
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/Crow-1.2.0.zip
RUN unzip Crow-1.2.0.zip
RUN mkdir /usr/local/include
RUN mv Crow-1.2.0-Darwin/include/* /usr/local/include
RUN mv Crow-1.2.0-Darwin/lib/* /usr/local/lib