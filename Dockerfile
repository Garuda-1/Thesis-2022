# CLion remote docker environment (How to build docker container, run and stop it)
#
# Build and run:
#   docker build -t clion/remote-cpp-env:0.5 -f Dockerfile .
#   docker network create thesis-network
#   docker run -d --cap-add sys_ptrace -p127.0.0.1:22099:22 --name clion_remote_env -v $HOME/Thesis-2022:/Thesis  --network thesis-network --network-alias clion clion/remote-cpp-env:0.5
#   docker run --name experiments-db -p 5432:5432 -e POSTGRES_PASSWORD=postgres -d --network thesis-network --network-alias postgres postgres
#   ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"


# 

#
# stop:
#   docker stop clion_remote_env
#
# ssh credentials (test user):
#   user@password

FROM ubuntu:20.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y ssh \
      build-essential \
      gcc \
      g++ \
      gdb \
      clang \
      make \
      ninja-build \
      cmake \
      autoconf \
      automake \
      locales-all \
      dos2unix \
      rsync \
      tar \
      python \
      git \
      htop \
      libboost-all-dev \
      autoconf \
      automake \
      libtool \
      libgoogle-glog-dev \
      libgflags-dev \
      libeigen3-dev \
  && apt-get clean

RUN git clone https://github.com/CMA-ES/libcmaes.git \
  && mkdir libcmaes/build \
  && cd libcmaes/build \
  && cmake .. \
  && make -j2 \
  && make install \
  && cd -

RUN wget http://pqxx.org/download/software/libpqxx/libpqxx-4.0.tar.gz \
  && tar xvfz libpqxx-4.0.tar.gz \
  && cd libpqxx-4.0 \
  && ./configure \
  && make \
  && make install \
  && cd -

RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion \
  && mkdir /run/sshd

RUN useradd -m user \
  && yes password | passwd user

RUN usermod -s /bin/bash user

CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]