#!/bin/bash

# Bosch-style Build Script
set -e  # Exit on error

echo "🚗 Bosch LiDAR Calibration Build System"
echo "========================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Build function
build_project() {
    echo -e "${YELLOW}🔨 Building C++ project...${NC}"
    
    mkdir -p build
    cd build
    
    # CMake konfigurieren
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_CXX_COMPILER=g++ \
          ..
    
    # Kompilieren
    make -j$(nproc)
    
    echo -e "${GREEN}✅ Build successful!${NC}"
    cd ..
}

# Test function
run_tests() {
    echo -e "${YELLOW}🧪 Running tests...${NC}"
    
    # C++ Tests (wenn vorhanden)
    if [ -f "build/lidar_calibration_main" ]; then
        echo "Running C++ executable..."
        ./build/lidar_calibration_main
    fi
    
    # Python Tests
    if [ -f "src/python/test_framework.py" ]; then
        echo "Running Python test framework..."
        python3 src/python/test_framework.py
    fi
    
    echo -e "${GREEN}✅ Tests completed!${NC}"
}

# Docker build
build_docker() {
    echo -e "${YELLOW}🐳 Building Docker image...${NC}"
    
    docker build -t bosch-lidar-calibration:latest -f docker/Dockerfile .
    
    echo -e "${GREEN}✅ Docker image built!${NC}"
}

# Main menu
main() {
    case $1 in
        "all")
            build_project
            run_tests
            build_docker
            ;;
        "build")
            build_project
            ;;
        "test")
            run_tests
            ;;
        "docker")
            build_docker
            ;;
        "help")
            echo "Usage: $0 [all|build|test|docker|help]"
            echo "  all    - Build everything"
            echo "  build  - Build C++ project only"
            echo "  test   - Run tests only"
            echo "  docker - Build Docker image only"
            ;;
        *)
            echo -e "${RED}❌ Unknown command: $1${NC}"
            echo "Use: $0 help"
            exit 1
            ;;
    esac
}

# Run main function
main "$@"