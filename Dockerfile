FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y && apt-get install -y --no-install-recommends apt-utils

RUN apt-get install -y build-essential
RUN apt-get install -y shunit2
RUN apt-get install -y libc++-10-dev libc++abi-10-dev
RUN apt-get install -y lsb-release wget software-properties-common
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 10  # version 10

ENV CC="clang-10"
ENV CXX="clang++-10"
COPY . /bc-seedtool-cli
WORKDIR /bc-seedtool-cli
RUN CC="clang-10" CXX="clang++-10" ./build.sh

ENTRYPOINT ["/bc-seedtool-cli/src/seedtool"]
