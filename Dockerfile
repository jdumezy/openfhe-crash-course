FROM ubuntu:24.04

ENV TZ=Europe/Paris

WORKDIR /usr/src/app

RUN apt-get update && apt-get install -y cmake git clang make libomp-dev vim # Also install vim to edit files

RUN git clone https://github.com/openfheorg/openfhe-development/

WORKDIR /usr/src/app/openfhe-development

RUN mkdir build && cd build && cmake .. \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ && make -j$(nproc) && make install

WORKDIR /usr/src/app/openfhe-development

COPY ./example/* ./src/pke/examples/

RUN cd build && cmake .. \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ && make -j$(nproc)

RUN ldconfig

WORKDIR /usr/src/app
