FROM ubuntu:xenial

RUN apt-get update

RUN apt-get install -y \
    cmake \
    g++ \
    libasound2-dev \
    libcurl4-openssl-dev \
    libfreetype6-dev \
    libgl1-mesa-dev \
    libgtk-3-dev \
    libwebkit2gtk-4.0-dev \
    libxcursor-dev \
    libxinerama-dev \
    libxrandr-dev \
    pkg-config
