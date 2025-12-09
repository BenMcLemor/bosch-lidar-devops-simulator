#include "lidar_calibration.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <algorithm>

LidarCalibrator::LidarCalibrator() 
    : calibration_error_(0.0f), is_calibrated_(false) {
    transformation_matrix_ = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

bool LidarCalibrator::calibrateSensor(const std::vector<Point3D>& point_cloud) {
    std::cout << "🔧 Starting LiDAR calibration..." << std::endl;
    std::cout << "📊 Processing " << point_cloud.size() << " points" << std::endl;
    
    if (point_cloud.empty()) {
        std::cerr << "❌ Error: Empty point cloud!" << std::endl;
        return false;
    }
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Simulierte Kalibrierungs-Berechnung (wie bei Bosch)
    // In der Realität: ICP-Algorithmus, RANSAC, etc.
    
    float sum_x = 0, sum_y = 0, sum_z = 0;
    for (const auto& point : point_cloud) {
        sum_x += point.x;
        sum_y += point.y;
        sum_z += point.z;
    }
    
    // Berechne Schwerpunkt
    float center_x = sum_x / point_cloud.size();
    float center_y = sum_y / point_cloud.size();
    float center_z = sum_z / point_cloud.size();
    
    // Aktualisiere Transformationsmatrix
    transformation_matrix_[3] = -center_x;  // Translation X
    transformation_matrix_[7] = -center_y;  // Translation Y
    transformation_matrix_[11] = -center_z; // Translation Z
    
    // Berechne Kalibrierungsfehler (simuliert)
    calibration_error_ = 0.01f + (rand() % 100) / 10000.0f;
    is_calibrated_ = true;
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    
    std::cout << "✅ Calibration completed!" << std::endl;
    std::cout << "📐 Calibration error: " << calibration_error_ << std::endl;
    std::cout << "⏱️  Processing time: " << duration.count() << "ms" << std::endl;
    
    return true;
}

float LidarCalibrator::calculateCalibrationError() const {
    return calibration_error_;
}

std::vector<Point3D> LidarCalibrator::transformPointCloud(
    const std::vector<Point3D>& points,
    const std::vector<float>& transformation_matrix) const {
    
    std::vector<Point3D> transformed_points;
    transformed_points.reserve(points.size());
    
    for (const auto& point : points) {
        Point3D transformed;
        transformed.x = point.x * transformation_matrix[0] + 
                       point.y * transformation_matrix[1] + 
                       point.z * transformation_matrix[2] + 
                       transformation_matrix[3];
        
        transformed.y = point.x * transformation_matrix[4] + 
                       point.y * transformation_matrix[5] + 
                       point.z * transformation_matrix[6] + 
                       transformation_matrix[7];
        
        transformed.z = point.x * transformation_matrix[8] + 
                       point.y * transformation_matrix[9] + 
                       point.z * transformation_matrix[10] + 
                       transformation_matrix[11];
        
        transformed_points.push_back(transformed);
    }
    
    return transformed_points;
}

std::vector<Point3D> LidarCalibrator::processPointCloudFast(
    const std::vector<Point3D>& points) const {
    
    // Simulierte optimierte Version (wie mit CUDA bei Bosch)
    std::vector<Point3D> processed;
    processed.reserve(points.size());
    
    #pragma omp parallel for  // Parallelisierung wie CUDA
    for (size_t i = 0; i < points.size(); ++i) {
        // Einfache Filterung: Entferne Punkte zu nah am Ursprung
        if (std::sqrt(points[i].x*points[i].x + 
                      points[i].y*points[i].y + 
                      points[i].z*points[i].z) > 0.1f) {
            processed.push_back(points[i]);
        }
    }
    
    return processed;
}

std::string LidarCalibrator::getCalibrationStatus() const {
    if (is_calibrated_) {
        return "CALIBRATED (Error: " + std::to_string(calibration_error_) + ")";
    }
    return "NOT_CALIBRATED";
}