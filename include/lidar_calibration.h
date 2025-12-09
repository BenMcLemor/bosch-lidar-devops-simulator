#ifndef LIDAR_CALIBRATION_H
#define LIDAR_CALIBRATION_H

#include <vector>
#include <string>

struct Point3D {
    float x, y, z;
    Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

class LidarCalibrator {
public:
    LidarCalibrator();
    
    // Kalibriert einen LiDAR-Sensor
    bool calibrateSensor(const std::vector<Point3D>& point_cloud);
    
    // Berechnet Kalibrierungs-Fehler
    float calculateCalibrationError() const;
    
    // Transformiert Punktwolke
    std::vector<Point3D> transformPointCloud(
        const std::vector<Point3D>& points,
        const std::vector<float>& transformation_matrix) const;
    
    // Performance-optimierte Version (wie bei Bosch)
    std::vector<Point3D> processPointCloudFast(
        const std::vector<Point3D>& points) const;
    
    // Gibt Kalibrierungs-Status zurück
    std::string getCalibrationStatus() const;
    
private:
    float calibration_error_;
    bool is_calibrated_;
    std::vector<float> transformation_matrix_;
};

#endif // LIDAR_CALIBRATION_H