FROM ubuntu:latest

SHELL ["/bin/bash", "-c"]

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ="America/Salt Lake City"
 
RUN apt-get update \
  && apt-get install -y python3-pip python3-dev \
  && cd /usr/local/bin \
  && ln -s /usr/bin/python3 python \
  && pip3 install --upgrade pip 

# INSTALL DEPENDENCIES
RUN apt-get install -y curl unzip openjdk-8-jre-headless xvfb libxi6 libgconf-2-4 

# INSTALL CHROME
RUN curl -sS -o - https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add \
    && echo "deb http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google-chrome.list \
    && apt-get -y update \
    && apt-get -y install google-chrome-stable

# INSTALL CHROMEDRIVER
RUN wget -N https://chromedriver.storage.googleapis.com/86.0.4240.22/chromedriver_linux64.zip -P ~/ \
    && unzip ~/chromedriver_linux64.zip -d ~/ \
    && rm ~/chromedriver_linux64.zip \
    && mv -f ~/chromedriver /usr/local/bin/chromedriver \
    && chown root:root /usr/local/bin/chromedriver \
    && chmod 0755 /usr/local/bin/chromedriver

# INSTALL CURL UTILS AND FFMPEG LIBS
RUN apt-get -y update && apt-get -y install \
  apt-utils \
  g++ \
  curl \
  libcurl4-openssl-dev \
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

# INSTALL SELENIUM
RUN pip install selenium


#
# Make ffmpeg inside image
RUN mkdir /cs3505 \
    ;cd /cs3505 \  
    ;git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg \ 
    ;cd ffmpeg \
    ;git reset --hard  f7fd205f11d5299d6a16b0ff0ae85fddb32772f2  \
    ;./configure  --prefix=/ \
    ;make   \
    ;make install 
#
# Have additional steps appear after this

COPY ./pyfiles /pyfiles
COPY ./textImageGenerators /textImageGenerators