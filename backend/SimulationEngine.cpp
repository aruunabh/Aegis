#include "SimulationEngine.hpp"
#include <cmath>
#include <iostream>
// In a full build, include <nlohmann/json.hpp> for JSON serialization

SimulationEngine::SimulationEngine() {}

void SimulationEngine::dispatchPlane(const std::string& id, PlaneType type, double sx, double sy, double tx, double ty) {
    std::lock_guard<std::mutex> lock(fleet_mutex);
    Plane p;
    p.id = id;
    p.type = type;
    p.x = sx; p.y = sy;
    p.target_x = tx; p.target_y = ty;
    p.active = true;

    if (type == PlaneType::FIGHTER) {
        p.speed = 150.0; p.fuel = 5000.0; p.fuel_consumption_rate = 10.0;
    } else if (type == PlaneType::BOMBER) {
        p.speed = 80.0; p.fuel = 15000.0; p.fuel_consumption_rate = 25.0;
    }
    fleet.push_back(p);
}

void SimulationEngine::updateSimulation(double delta_time) {
    std::lock_guard<std::mutex> lock(fleet_mutex);
    for (auto& plane : fleet) {
        if (!plane.active) continue;

        // Move towards target
        double dx = plane.target_x - plane.x;
        double dy = plane.target_y - plane.y;
        double dist = std::sqrt(dx*dx + dy*dy);
        
        if (dist > 1.0) {
            plane.x += (dx / dist) * plane.speed * delta_time;
            plane.y += (dy / dist) * plane.speed * delta_time;
        }

        // Consume fuel
        plane.fuel -= plane.fuel_consumption_rate * delta_time;
        if (plane.fuel <= 0) plane.active = false;
    }
    checkCollisions();
}

void SimulationEngine::checkCollisions() {
    // Basic O(n^2) spatial check for mid-air collision logic
    for (size_t i = 0; i < fleet.size(); ++i) {
        for (size_t j = i + 1; j < fleet.size(); ++j) {
            if (!fleet[i].active || !fleet[j].active) continue;
            double dx = fleet[i].x - fleet[j].x;
            double dy = fleet[i].y - fleet[j].y;
            if (std::sqrt(dx*dx + dy*dy) < 5.0) {
                fleet[i].active = false;
                fleet[j].active = false;
                std::cout << "Collision detected between " << fleet[i].id << " and " << fleet[j].id << "!\n";
            }
        }
    }
}