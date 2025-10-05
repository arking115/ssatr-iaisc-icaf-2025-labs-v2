# Real-Time Systems Lab - Week 1
# Docker environment with RT tools and libraries

FROM ubuntu:22.04

# Prevent interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Install essential RT development tools
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    gdb \
    # RT-specific tools
    rt-tests \
    stress-ng \
    # Timing and tracing tools
    linux-tools-generic \
    trace-cmd \
    # Python for visualization
    python3 \
    python3-pip \
    python3-matplotlib \
    python3-numpy \
    # Utilities
    htop \
    procps \
    vim \
    nano \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy project files
COPY . /workspace/

# Set default command
CMD ["/bin/bash"]
