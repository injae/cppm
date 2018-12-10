FROM ubuntu:bionic
MAINTAINER 8687lee@gmail.com
RUN apt-get update

RUN apt-get -y install git gcc g++ cmake
RUN apt-get -y install build-essential
RUN apt-get -y install libboost-all-dev
RUN git clone https://github.com/INJAE/cppm.git
WORKDIR cppm
RUN cmake -H. -Bbuild && cd build && cmake --build . --target install


