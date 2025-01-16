FROM ubuntu

RUN apt-get update
RUN apt-get install -y python3 g++ clang-format fswatch

WORKDIR /app