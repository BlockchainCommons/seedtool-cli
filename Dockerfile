FROM ubuntu:20.04 as build-stage
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y
# Install build dependencies
RUN apt-get install -y git
RUN apt-get install -y build-essential
RUN apt-get install -y libc++-10-dev libc++abi-10-dev
RUN apt-get install -y lsb-release wget software-properties-common
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 10  # version 10
# Build seedtool-cli
ENV CC="clang-10"
ENV CXX="clang++-10"
COPY . /seedtool-cli
WORKDIR /seedtool-cli
RUN git submodule update --init --recursive
RUN CC="clang-10" CXX="clang++-10" ./build.sh

# Export built executable to a minimal runtime image and run as an unprivileged
# user.
FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
# Install runtime dependencies
RUN apt-get update -y
RUN apt-get install -y libc++1 libc++abi1
# Install binary
COPY --from=build-stage /seedtool-cli/src/seedtool /usr/local/bin/seedtool
# Create an unprivileged user
RUN useradd --create-home --user-group user
USER user
ENTRYPOINT ["seedtool"]
