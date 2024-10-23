FROM alpine

RUN apk update
RUN apk add wget unzip git
RUN apk add build-base cmake gdb pkgconfig
RUN apk add asio
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/Crow-1.2.0.zip
RUN unzip Crow-1.2.0.zip
RUN rm Crow-1.2.0.zip
RUN mv Crow-1.2.0-Darwin/lib/* /usr/local/lib
RUN rm -rf Crow-1.2.0-Darwin
RUN wget https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.11.0/mongo-cxx-driver-r3.11.0.tar.gz
RUN tar -xzf mongo-cxx-driver-r3.11.0.tar.gz
WORKDIR /mongo-cxx-driver-r3.11.0/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DMONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX=OFF
RUN cmake --build . && cmake --build . --target install
WORKDIR /
RUN rm -rf mongo-cxx-driver-r3.11.0
RUN rm mongo-cxx-driver-r3.11.0.tar.gz

WORKDIR /root
COPY /build .
ENV PORT=80
WORKDIR /root/hello_crow
CMD ["./hello"]