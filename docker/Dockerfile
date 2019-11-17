# Stage 1
FROM debian:stretch-slim AS builder

# install dependencies
RUN buildDeps='git ca-certificates build-essential cmake xorg-dev libfreetype6-dev pkg-config' \
    && set -x \
    && apt-get update && apt-get install -y $buildDeps --no-install-recommends

# build FRUT
ARG CMAKE_BUILD_TYPE="Release"
RUN git clone --depth 1 https://github.com/WeAreROLI/JUCE.git \
    && git clone --depth 1 https://github.com/McMartin/FRUT.git \
    && mkdir ./FRUT/build && cd ./FRUT/build \
    && cmake .. -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=./ -DJUCE_ROOT=../../JUCE \
    && cmake --build . --target install

# Stage 2
FROM debian:stretch-slim

COPY --from=builder /FRUT/build/FRUT/bin/Jucer2Reprojucer /usr/local/bin
COPY --from=builder /FRUT/build/FRUT/cmake/Reprojucer.cmake /

COPY entrypoint.sh ./
RUN chmod +x ./entrypoint.sh
ENTRYPOINT ["./entrypoint.sh"]
CMD []