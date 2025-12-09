#include "lidar_calibration.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>

// Simuliere LiDAR-Daten
std::vector<Point3D> generateTestPointCloud(int num_points = 10000) {
    std::vector<Point3D> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    
    for (int i = 0; i < num_points; ++i) {
        points.emplace_back(dis(gen), dis(gen), dis(gen));
    }
    
    return points;
}

int main() {
    std::cout << "🚗 Bosch LiDAR Calibration Simulator" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // Simuliere 4 LiDAR-Sensoren wie bei Bosch
    LidarCalibrator lidars[4];
    std::vector<std::vector<Point3D>> point_clouds;
    
    // Generiere Testdaten für jeden Sensor
    for (int i = 0; i < 4; ++i) {
        std::cout << "\n📡 Generating point cloud for LiDAR " << i+1 << "..." << std::endl;
        point_clouds.push_back(generateTestPointCloud(5000));
    }
    
    // Parallele Kalibrierung (wie im echten System)
    std::cout << "\n🔧 Starting parallel calibration of 4 LiDARs..." << std::endl;
    
    bool all_calibrated = true;
    for (int i = 0; i < 4; ++i) {
        std::cout << "\nCalibrating LiDAR " << i+1 << ":" << std::endl;
        if (!lidars[i].calibrateSensor(point_clouds[i])) {
            all_calibrated = false;
        }
    }
    
    // Ergebnisse
    std::cout << "\n📊 CALIBRATION RESULTS:" << std::endl;
    std::cout << "========================" << std::endl;
    
    for (int i = 0; i < 4; ++i) {
        std::cout << "LiDAR " << i+1 << ": " 
                  << lidars[i].getCalibrationStatus() << std::endl;
    }
    
    // Konsistenz-Check (Integration Test)
    float max_error = 0.0f;
    for (int i = 0; i < 4; ++i) {
        float error = lidars[i].calculateCalibrationError();
        if (error > max_error) max_error = error;
    }
    
    std::cout << "\n🔍 INTEGRATION TEST:" << std::endl;
    std::cout << "Maximum error difference: " << max_error << std::endl;
    
    if (max_error < 0.02f && all_calibrated) {
        std::cout << "✅ All LiDARs calibrated consistently!" << std::endl;
        std::cout << "🚗 Vehicle ready for autonomous driving!" << std::endl;
    } else {
        std::cout << "❌ Calibration consistency check failed!" << std::endl;
    }
    
    return 0;
}