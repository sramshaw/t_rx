FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends   clang-format fswatch default-jre \
                                                    build-essential libncurses-dev bison flex libssl-dev libelf-dev gcc-multilib
# https://tutorialforlinux.com/2023/07/28/gcc-13-2-debian-bookworm-installation-step-by-step/3/
WORKDIR /home/vscode/Downloads
RUN wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz \
    && tar xvzf gcc-13.2.0.tar.gz
WORKDIR /home/vscode/Downloads/gcc-13.2.0
RUN ./contrib/download_prerequisites
RUN export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu \
&&  echo "export LD_LIBRARY_PATH=/usr/lib:/usr/local/lib:/usr/local/lib64:/usr/lib/x86_64-linux-gnu" >> ~/.bashrc
WORKDIR /home/vscode/Downloads/gcc-13.2.0/build
RUN ../configure --enable-languages=c,c++,fortran,objc,obj-c++,go --prefix="/usr/local" --disable-multilib
RUN make -j $(nproc)
RUN make install
RUN update-alternatives --install /usr/bin/gcc gcc /usr/local/bin/gcc 10
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 20

WORKDIR /workspaces/
