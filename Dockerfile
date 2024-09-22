FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y cmake g++ build-essential

# Set the working directory inside the container
WORKDIR /usr/src/app

COPY CMakeLists.txt .
COPY dag.cpp .
COPY run.sh .

RUN chmod +777 run.sh

CMD ["bash", "./run.sh"]