FROM alpine

RUN apk update
RUN apk add wget unzip
RUN apk add build-base
RUN apk add asio
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/Crow-1.2.0.zip
RUN unzip Crow-1.2.0.zip
RUN rm Crow-1.2.0.zip
RUN mv Crow-1.2.0-Darwin/lib/* /usr/local/lib
RUN rm -rf Crow-1.2.0-Darwin

WORKDIR /root
COPY /build .
ENV PORT=80
WORKDIR /root/hello_crow
CMD ["./hello"]