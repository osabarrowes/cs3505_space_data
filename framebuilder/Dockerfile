FROM ubuntu:latest

SHELL ["/bin/bash", "-c"]

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ="US/Mountain"

RUN apt-get -y update && apt-get -y install \
  apt-utils \
  autoconf \
  automake \
  build-essential \
  cmake \
  git-core \
  libass-dev \
  libfreetype6-dev \
  libgnutls28-dev \
  libtool \
  libvorbis-dev \
  pkg-config \
  texinfo \
  wget \
  yasm \
  zlib1g-dev \
  libfontconfig \
  g++ \ 
  gdb 

#
# Make ffmpeg inside image
RUN apt install -y \
    libswresample3 \
    libavcodec58 \
    libavformat58 \
    libavdevice58 \
    libavfilter7 \
    ffmpeg 
#
# Have additional steps appear after this

COPY ./framebuilder /framebuilder