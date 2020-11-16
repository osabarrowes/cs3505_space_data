#
# Have this image be based off of the ubuntu image
FROM ubuntu
#
#

#
# Set a default shell.
SHELL ["/bin/bash", "-c"]
#
#

#
# Set a timezone
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ="America/Salt Lake City"
#
#

#
# Grab necessary tools and librarys to make ffmpeg
RUN apt-get -y update && apt-get -y install \
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
   zlib1g-dev
#
#

#
# Make ffmpeg inside image
RUN mkdir /cs3505 \
    ;cd /cs3505 \  
    ;git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg \ 
    ;cd ffmpeg \
    ;git reset --hard  f7fd205f11d5299d6a16b0ff0ae85fddb32772f2  \
    ;./configure  --prefix=/ \
    ;make   \
    ;make install \
    ;cd .. \
    ;rm -r ffmpeg
#
# Have additional steps appear after this



