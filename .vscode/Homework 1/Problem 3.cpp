#include <iostream>
#include <string>
#include <map>

using namespace std;

class Plane {
private:
    // Private members
    double pos;
    double vel;
    double distance;
    bool at_SCE;
    string origin;
    string destination;
    
    // Flight distance map (container from question 2)
    map<string, int> flightDistances;

public:
    // Constructor that takes in two strings (origin and destination)
    Plane(string from, string to) {
        origin = from;
        destination = to;
        pos = 0.0;
        vel = 0.0;
        distance = 0.0;
        at_SCE = (from == "SCE");

        // Initialize flight distances (container)
        flightDistances["PHL"] = 160;
        flightDistances["ORD"] = 640;
        flightDistances["EWR"] = 220;

        // Set the distance for this flight from the container
        if (flightDistances.find(to) != flightDistances.end()) {
            distance = flightDistances[to];
        } else {
            distance = 0.0; // If destination is not found, set distance to 0
        }
    }

    // Destructor
    ~Plane() {
        // No specific resource management needed, just printing for visibility
        cout << "Plane from " << origin << " to " << destination << " destroyed." << endl;
    }

    // Function 'operate' that takes a double 'dt' and returns nothing
    void operate(double dt) {
        // Placeholder function to simulate operation (no real functionality yet)
        // This function would typically update the position based on velocity and dt
        pos += vel * dt;
        cout << "Plane operated for " << dt << " seconds. Current position: " << pos << endl;
    }

    // Getter functions
    double getPos() const { return pos; }
    string getOrigin() const { return origin; }
    string getDestination() const { return destination; }
    bool getAtSCE() const { return at_SCE; }

    // Getter and setter for velocity
    double getVel() const { return vel; }
    void setVel(double newVel) { vel = newVel; }
};

int main() {
    // Example usage (will not run actual functionality, just for setup)
    Plane plane("SCE", "PHL");
    
    // Test getters and setters
    plane.setVel(300.0);
    cout << "Velocity set to: " << plane.getVel() << " mph" << endl;
    
    plane.operate(1.0);  // Simulate operation for 1 second

    return 0;
}
