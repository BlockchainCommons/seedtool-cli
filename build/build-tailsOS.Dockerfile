FROM debian:latest as base
ARG DEBIAN_FRONTEND=noninteractive
WORKDIR /app
COPY . source
COPY .git source/.git

RUN apt update -y 
RUN apt install -y  \
    build-essential autogen libtool shtool     \
    clang llvm lld libstdc++6 libc++-dev libc++1 libc++abi1         \
    git-buildpackage    \
    git                 \
    coreutils binutils          \
    bash
WORKDIR source
FROM base
ARG DEBIAN_FRONTEND=noninteractive
SHELL ["/bin/bash", "-c"]
RUN chmod -x debian/clean
RUN ln -svf /usr/bin/lld /usr/bin/ld
RUN git submodule update
RUN cat debian/patches/series | xargs -I{} patch -f --strip=1 -i "debian/patches/{}" || true
RUN patch -f --strip=1 -i debian/patches/bc-ur-build-tails.diff || true

# FIXME(nochiel) It would be nice if running this line compiled the packaged the files we care about.
RUN gbp buildpackage --git-upstream-tree=master --git-debian-branch=debian/latest --git-ignore-new  --git-force-create --git-submodules