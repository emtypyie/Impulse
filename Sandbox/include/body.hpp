#pragma once
#include "Vector3d.hpp"
#include <string>
#include <utility>
#include <vector>

class Body {
public:
    std::string name;
    
    // Kinematics
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;

    // Physical Properties
    double mass = 0.0;
    double radius = 0.0;
    
    // Sandbox Controls
    bool isStatic = false;

    // --- ORBIT TRACK DATA HISTORY ---
    std::vector<Vector3d> pathHistory;
    size_t maxHistoryPoints = 500; // Removed const so vector memory can shift freely!

    Body() = default;

    Body(std::string bodyName, Vector3d pos, Vector3d vel, double bodyMass, double bodyRadius, bool locked = false)
        : name(std::move(bodyName)), 
          position(pos), 
          velocity(vel), 
          acceleration(0.0, 0.0, 0.0), 
          mass(bodyMass), 
          radius(bodyRadius), 
          isStatic(locked) {}

    // Adds the current position coordinates to our track history
    void appendPathPoint() {
        if (isStatic) return;
        pathHistory.push_back(position);
        
        if (pathHistory.size() > maxHistoryPoints) {
            pathHistory.erase(pathHistory.begin());
        }
    }

    bool checkCollision(const Body& secondBody) const {
        Vector3d direction = secondBody.position - position;
        double distance = direction.length();
        return distance <= (radius + secondBody.radius);
    }
};
