#include <iostream>
#include <cmath>   // for distance calculation
#include <string>  // for handling strings

class Plane {
private:
    std::string origin;
    std::string destination;
    double distance;
    double pos;
    double vel;
    double at_SCE;

public:
    // Constructor
    Plane(const std::string& from, const std::string& to) 
        : origin(from), destination(to), pos(0), vel(0), at_SCE(0) 
    {
        distance = calculateDistance(origin, destination); // Assuming function from Q2
        std::cout << "Plane Created at " << this << std::endl;
    }

    // Destructor
    ~Plane() {
        std::cout << "Plane Destroyed" << std::endl;
    }

    // Getters
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    double getDistance() const { return distance; }
    double getPosition() const { return pos; }
    double getVelocity() const { return vel; }
    double getAtSCE() const { return at_SCE; }

    // Setters
    void setPosition(double position) { pos = position >= 0 ? position : 0; }
    void setVelocity(double velocity) { vel = velocity >= 0 ? velocity : 0; }
    void setAtSCE(double timeAtSCE) { at_SCE = timeAtSCE >= 0 ? timeAtSCE : 0; }

    // Method to calculate distance between two airports (Assuming some logic from Q2)
    double calculateDistance(const std::string& from, const std::string& to) {
        // Dummy implementation - replace with actual logic based on Q2
        return std::abs(static_cast<int>(from.length() - to.length())) * 100.0;
    }

    // Operate function (based on a presumed flowchart)
    void operate(double timeStep) {
        if (timeStep < 0) {
            timeStep = 0;  // Ensure non-negative time step
        }
        pos += vel * timeStep;
        if (pos > distance) {
            pos = distance; // Plane has reached destination
            vel = 0;        // Plane stops moving
        }
        // Update other variables based on flowchart logic (dummy example):
        at_SCE -= timeStep;
        if (at_SCE < 0) {
            at_SCE = 0;
        }

        // Additional flowchart logic would go here...
    }
};

int main() {
    // Create a plane object
    Plane plane("LAX", "JFK");

    // Example usage of setters and operate function
    plane.setVelocity(600);  // Set velocity to 600 units (km/h or m/s depending on your unit)
    plane.operate(5);        // Simulate 5 seconds of operation
    std::cout << "Plane position after 5 seconds: " << plane.getPosition() << std::endl;

    return 0;
}
