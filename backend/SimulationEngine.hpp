#pragma once
#include <vector>
#include <mutex>
#include <string>

enum class PlaneType { FIGHTER, BOMBER, RECON };

struct Plane {
    std::string id;
    PlaneType type;
    double x, y;
    double target_x, target_y;
    double speed;
    double fuel;
    double fuel_consumption_rate;
    bool active;
};

class SimulationEngine {
public:
    SimulationEngine();
    void dispatchPlane(const std::string& id, PlaneType type, double start_x, double start_y, double tx, double ty);
    void updateSimulation(double delta_time);
    std::string getTelemetryJSON();

private:
    std::vector<Plane> fleet;
    std::mutex fleet_mutex;
    void checkCollisions();
};