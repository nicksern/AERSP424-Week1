#include <iostream>
#include <iomanip> // for setting decimal precision
using namespace std;

// Function to calculate moment
double calculateMoment(double weight, double arm) {
    return weight * arm;
}

int main() {
    // Declare variables
    double empty_weight, empty_weight_moment;
    const int num_front_occupants, num_rear_occupants;
    double front_seat_arm, rear_seat_arm;
    double fuel_weight_per_gallon, fuel_tank_arm;
    double baggage_weight, baggage_arm;
    double gallons_of_fuel;
    
    // Design limits
    const double MAX_GROSS_WEIGHT = 2950.0;
    const double FORWARD_CG_LIMIT = 82.1;
    const double AFT_CG_LIMIT = 84.7;

    // Input airplane empty weight and moment
    cout << "Enter the airplane empty weight (lbs): ";
    cin >> empty_weight;
    cout << "Enter the airplane empty-weight moment (lbs-inches): ";
    cin >> empty_weight_moment;

    // Input front seat occupants and moment arm
    cout << "Enter the number of front seat occupants: ";
    cin >> num_front_occupants;
    double front_occupant_weights[num_front_occupants];
    for (int i = 0; i < num_front_occupants; ++i) {
        cout << "Enter the weight of front seat occupant " << i+1 << " (lbs): ";
        cin >> front_occupant_weights[i];
    }
    cout << "Enter the front seat moment arm (inches): ";
    cin >> front_seat_arm;

    // Input rear seat occupants and moment arm
    cout << "Enter the number of rear seat occupants: ";
    cin >> num_rear_occupants;
    double rear_occupant_weights[num_rear_occupants];
    for (int i = 0; i < num_rear_occupants; ++i) {
        cout << "Enter the weight of rear seat occupant " << i+1 << " (lbs): ";
        cin >> rear_occupant_weights[i];
    }
    cout << "Enter the rear seat moment arm (inches): ";
    cin >> rear_seat_arm;

    // Input fuel information
    cout << "Enter the number of gallons of usable fuel: ";
    cin >> gallons_of_fuel;
    cout << "Enter the usable fuel weight per gallon (lbs): ";
    cin >> fuel_weight_per_gallon;
    cout << "Enter the fuel tank moment arm (inches): ";
    cin >> fuel_tank_arm;

    // Input baggage weight and moment arm
    cout << "Enter the baggage weight (lbs): ";
    cin >> baggage_weight;
    cout << "Enter the baggage moment arm (inches): ";
    cin >> baggage_arm;

    // Calculate total weight and moment for front occupants
    double total_front_weight = 0.0;
    double total_front_moment = 0.0;
    for (int i = 0; i < num_front_occupants; ++i) {
        total_front_weight += front_occupant_weights[i];
    }
    total_front_moment = calculateMoment(total_front_weight, front_seat_arm);

    // Calculate total weight and moment for rear occupants
    double total_rear_weight = 0.0;
    double total_rear_moment = 0.0;
    for (int i = 0; i < num_rear_occupants; ++i) {
        total_rear_weight += rear_occupant_weights[i];
    }
    total_rear_moment = calculateMoment(total_rear_weight, rear_seat_arm);

    // Calculate total weight and moment for fuel
    double total_fuel_weight = gallons_of_fuel * fuel_weight_per_gallon;
    double total_fuel_moment = calculateMoment(total_fuel_weight, fuel_tank_arm);

    // Calculate total weight and moment for baggage
    double total_baggage_moment = calculateMoment(baggage_weight, baggage_arm);

    // Calculate total gross weight and total moment
    double gross_weight = empty_weight + total_front_weight + total_rear_weight + total_fuel_weight + baggage_weight;
    double total_moment = empty_weight_moment + total_front_moment + total_rear_moment + total_fuel_moment + total_baggage_moment;

    // Calculate Center of Gravity (CG)
    double CG = total_moment / gross_weight;

    // Display gross weight and CG
    cout << fixed << setprecision(2);
    cout << "Gross weight: " << gross_weight << " lbs" << endl;
    cout << "Center of Gravity (CG): " << CG << " inches" << endl;

    // Check if gross weight and CG are within limits
    if (gross_weight > MAX_GROSS_WEIGHT) {
        cout << "Error: Gross weight exceeds maximum allowable limit of " << MAX_GROSS_WEIGHT << " lbs." << endl;
    } else if (CG < FORWARD_CG_LIMIT || CG > AFT_CG_LIMIT) {
        cout << "Error: Center of Gravity is out of allowable range (" << FORWARD_CG_LIMIT << " to " << AFT_CG_LIMIT << " inches)." << endl;

        // Adjust fuel to bring CG within limits
        double new_gallons_of_fuel = gallons_of_fuel;
        while (CG < FORWARD_CG_LIMIT || CG > AFT_CG_LIMIT) 
        {
            if (CG < FORWARD_CG_LIMIT) {
                new_gallons_of_fuel -= 0.01; // Drain fuel
            } else if (CG > AFT_CG_LIMIT) {
                new_gallons_of_fuel += 0.01; // Add fuel
            }

            total_fuel_weight = new_gallons_of_fuel * fuel_weight_per_gallon;
            total_fuel_moment = calculateMoment(total_fuel_weight, fuel_tank_arm);
            gross_weight = empty_weight + total_front_weight + total_rear_weight + total_fuel_weight + baggage_weight;
            total_moment = empty_weight_moment + total_front_moment + total_rear_moment + total_fuel_moment + total_baggage_moment;
            CG = total_moment / gross_weight;
        }

        cout << "Adjusted fuel amount to meet CG limits: " << new_gallons_of_fuel << " gallons" << endl;
        cout << "New Gross weight: " << gross_weight << " lbs" << endl;
        cout << "New Center of Gravity (CG): " << CG << " inches" << endl;
    } else {
        cout << "Gross weight and CG are within limits." << endl;
    }

    return 0;
}
