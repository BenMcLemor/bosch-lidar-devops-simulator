# Multi-Stage Build für C++ und Python
FROM ubuntu:22.04 AS builder

# Installiere Build-Tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    libomp-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Kopiere Quellcode
COPY . .

# Build C++ Code
RUN rm -rf build && \
    mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j4

# Installiere Python Abhängigkeiten
RUN pip3 install -r requirements.txt

# Runtime Image
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    python3 \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Kopiere Binaries vom Builder
COPY --from=builder /app/build/lidar_calibration_main /app/
COPY --from=builder /app/src/python/test_framework.py /app/
COPY --from=builder /usr/lib/x86_64-linux-gnu/libomp.so* /usr/lib/x86_64-linux-gnu/
COPY --from=builder /usr/lib/x86_64-linux-gnu/libgcc_s.so* /usr/lib/x86_64-linux-gnu/
COPY --from=builder /usr/lib/x86_64-linux-gnu/libstdc++.so* /usr/lib/x86_64-linux-gnu/

# Setze Environment Variables
ENV PYTHONPATH=/app
ENV LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH

# Expose Port für zukünftige REST API
#EXPOSE 8080F

# Standard Command
CMD ["python3", "test_framework.py"]