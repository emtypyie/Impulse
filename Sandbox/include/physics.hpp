#pragma once
#include "body.hpp"
#include <vector>
#include <cmath>
#include <algorithm> // Needed for std::max

class PhysicsEngine {
private:
    std::vector<Body> bodies;
    double timeElapsed = 0.0;

    void computeAccelerations() {
        const double G = 1.0; 
        const double softening = 0.001; 

        for (auto& body : bodies) {
            body.acceleration = Vector3d(0.0, 0.0, 0.0);
        }

        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                Vector3d direction = bodies[j].position - bodies[i].position;
                double distSq = direction.lengthSquared() + softening;
                double dist = std::sqrt(distSq);

                double forceMagnitude = (G * bodies[i].mass * bodies[j].mass) / distSq;
                Vector3d unitDirection = direction / dist;
                Vector3d forceVector = unitDirection * forceMagnitude;

                if (!bodies[i].isStatic) {
                    bodies[i].acceleration += forceVector / bodies[i].mass;
                }
                if (!bodies[j].isStatic) {
                    bodies[j].acceleration -= forceVector / bodies[j].mass;
                }
            }
        }
    }

    void handleCollisions() {
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                if (bodies[i].checkCollision(bodies[j])) {
                    size_t larger = (bodies[i].mass >= bodies[j].mass) ? i : j;
                    size_t smaller = (larger == i) ? j : i;

                    double combinedMass = bodies[larger].mass + bodies[smaller].mass;
                    
                    if (combinedMass > 0.0 && !bodies[larger].isStatic) {
                        Vector3d momentum1 = bodies[larger].velocity * bodies[larger].mass;
                        Vector3d momentum2 = bodies[smaller].velocity * bodies[smaller].mass;
                        bodies[larger].velocity = (momentum1 + momentum2) / combinedMass;
                    }

                    bodies[larger].mass = combinedMass;
                    bodies[larger].radius = std::max(bodies[larger].radius, bodies[smaller].radius + (bodies[smaller].radius * 0.2));

                    bodies.erase(bodies.begin() + smaller);
                    --i; 
                    break;
                }
            }
        }
    }

public:
    PhysicsEngine() = default;

    void spawnBody(const Body& newBody) { bodies.push_back(newBody); }
    void reset() { bodies.clear(); timeElapsed = 0.0; }
    const std::vector<Body>& getBodies() const { return bodies; }

    // --- ADAPTIVE TIMESTEP INTEGRATOR ---
    void step(double targetDt) {
        if (bodies.empty()) return;

        // 1. Find the highest acceleration in the entire system this frame
        double maxAccel = 0.0;
        for (const auto& body : bodies) {
            double accelMag = body.acceleration.length();
            if (accelMag > maxAccel) maxAccel = accelMag;
        }

        // 2. Adaptive Logic: If forces are high, slice 'dt' into smaller pieces
        // This effectively slows down simulation time during close encounters to preserve accuracy
        double adaptiveDt = targetDt;
        if (maxAccel > 0.0) {
            // Stability Constant / Max Acceleration (Safety Clamp)
            double stabilityLimit = 0.5 / sqrt(maxAccel);
            if (adaptiveDt > stabilityLimit) {
                adaptiveDt = stabilityLimit;
            }
        }

        // 3. Symplectic Leapfrog Integration
        timeElapsed += adaptiveDt;

        computeAccelerations(); // Kick 1
        for (auto& body : bodies) {
            if (body.isStatic) continue;
            body.velocity += body.acceleration * (adaptiveDt * 0.5);
        }

        for (auto& body : bodies) { // Drift
            if (body.isStatic) continue;
            body.position += body.velocity * adaptiveDt;
        }

        computeAccelerations(); // Kick 2
        for (auto& body : bodies) {
            if (body.isStatic) continue;
            body.velocity += body.acceleration * (adaptiveDt * 0.5);
        }

        handleCollisions();
        for (auto& body : bodies) {
            body.appendPathPoint();
        }
    }
};
