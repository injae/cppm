FROM ubuntu:bionic
MAINTAINER 8687lee@gmail.com
RUN apt-get update

RUN apt-get -y install git gcc g++ cmake

#RUN git clone https://github.com/INJAE/cppm.git
#
#WORKDIR cppm
#RUN mkdir build && cd build && cmake .. && make install


