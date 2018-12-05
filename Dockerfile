FROM ubuntu:bionic
MAINTAINER 8687lee@gmail.com
RUN apt-get update

RUN apt-get -y install git gcc g++ cmake

#ADD https://github.com/Kitware/CMake/releases/download/v3.13.1/cmake-3.13.1.tar.gz .
#RUN tar -zxvf cmake-3.13.1.tar.gz && cd cmake-3.13.1 && ./bootstrap && make install

#WORKDIR ../
RUN git clone https://github.com/INJAE/cppm.git

WORKDIR cppm
RUN gcc --version
RUN mkdir build && cd build && cmake .. && make install


