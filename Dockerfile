FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y build-essential cmake git wget \
    libasio-dev libpqxx-dev libpq-dev && \
    apt-get clean

# Install Crow (header-only)
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.0+5/crow_all.h -O /usr/local/include/crow.h

WORKDIR /app
COPY . /app

# Build
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

CMD ["./build/Netflix_Inventory_API"]
