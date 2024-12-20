#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    // Declare Map to store the destination name as a string and distance as an int
    map<string, int> flight_dist;

    // Adding the flight distances to the map
    flight_dist["PHL"] = 160;  // Flight distance between State College and Philadelphia
    flight_dist["ORD"] = 640;  // Flight distance between State College and Chicago
    flight_dist["EWR"] = 220;  // Flight distance between State College and Newark

    // Outputting the flight information
    cout << "Flight distances from University Park Airport (SCE):" << endl;
    for (const auto& flight : flight_dist) {
        cout << "SCE to " << flight.first << ": " << flight.second << " miles." << endl;
    }

    return 0;
}
