FROM alpine

RUN apk update
RUN apk add wget unzip git
RUN apk add build-base cmake gdb pkgconfig
RUN apk add asio-dev
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/Crow-1.2.0.zip
RUN unzip Crow-1.2.0.zip
RUN rm Crow-1.2.0.zip
RUN mkdir /usr/local/include
RUN mv Crow-1.2.0-Darwin/include/* /usr/local/include
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

ENV AZURE_COSMOS_CONNECTIONSTRING=mongodb://ot-learning-cpp:4kb8BOOlEd0IJeDosuaZW5RJ9VbwdJIgtbE5tIog21uLZhhkpd1jhcCd4zXxkKUHaXCWuxxvQAm7ACDbIWwH3Q==@ot-learning-cpp.mongo.cosmos.azure.com:10255/?ssl=true&retrywrites=false&replicaSet=globaldb&maxIdleTimeMS=120000&appName=@ot-learning-cpp@