FROM ubuntu:18.04
RUN apt-get update && \
    apt-get install -y software-properties-common apt-transport-https \
    ca-certificates gnupg software-properties-common wget 
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add -
RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install g++-9 valgrind build-essential git cmake -y
RUN rm -rf /var/lib/apt/lists/*
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 100

COPY ./ /app/
WORKDIR /app

RUN  cmake -H. -Bbuild 
RUN cd build &&  cmake -DUSE_CPPM_PATH=ON -DCMAKE_BUILD_TYPE=Debug .. && cmake --build . --config Debug
RUN cd build/Debug && ./server 

