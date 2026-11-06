
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cmath>

using namespace std;

class CarpoolRide {
private:
    int rideID;
    string driverName;
    string source;
    string destination;
    string travelDate;
    int totalSeats;
    int bookedSeats;
    double fuelCost;
    double tollCost;
    double parkingCost;
    double otherCost;

    static int rideCount;

public:
    CarpoolRide(): rideID(0), driverName(""), source(""), destination(""),
          travelDate(""), totalSeats(0), bookedSeats(0),
          fuelCost(0), tollCost(0), parkingCost(0), otherCost(0) {}

    inline int getRideID() const {
        return rideID;
    }

    inline int getAvailableSeats() const {
        return totalSeats - bookedSeats;
    }

    inline double getTotalExpense() const {
        return fuelCost + tollCost + parkingCost + otherCost;
    }

    static int getRideCount() {
        return rideCount;
    }

    static void incrementRideCount() {
        ++rideCount;
    }

    void createRide() {
        this->rideID = readInt("\nEnter unique Ride ID: ", 1, 999999);

        driverName = readString("Driver name: ");
        source = readString("Source: ");
        destination = readString("Destination: ");
        travelDate = readString("Travel date (DD-MM-YYYY): ");

        totalSeats = readInt("Total seats (including driver): ", 1, 20);
        bookedSeats = readInt("Passenger seats already booked: ", 0, totalSeats - 1);

        fuelCost = readDouble("Fuel expense (Rs.): ");
        tollCost = readDouble("Toll expense (Rs.): ");
        parkingCost = readDouble("Parking expense (Rs.): ");
        otherCost = readDouble("Other expense (Rs.): ");
    }

    void displayRide() const {
        cout << fixed << setprecision(2);

        cout << "\n----------------------------------------\n";
        cout << "Ride ID          : " << rideID << '\n';
        cout << "Driver           : " << driverName << '\n';
        cout << "Route            : " << source << " -> " << destination << '\n';
        cout << "Travel date      : " << travelDate << '\n';
        cout << "Total seats      : " << totalSeats << '\n';
        cout << "Booked passenger : " << bookedSeats << '\n';
        cout << "Available seats  : " << getAvailableSeats() << '\n';
        cout << "Total occupants  : " << bookedSeats + 1 << '\n';
        cout << "Total expense    : Rs. " << getTotalExpense() << '\n';
        cout << "Share per person : Rs. "
             << getTotalExpense() / (bookedSeats + 1) << '\n';
        cout << "----------------------------------------\n";
    }

    friend bool isDuplicateRideID(const CarpoolRide rides[],
                                  int count, int candidateID);

private:
    static string readString(const string& prompt) {
        string value;

        while (true) {
            cout << prompt;
            getline(cin, value);

            if (!value.empty()) {
                return value;
            }

            cout << "Input cannot be empty. Try again.\n";
        }
    }

    static int readInt(const string& prompt, int minimum, int maximum) {
        int value;

        while (true) {
            cout << prompt;

            if (cin >> value && value >= minimum && value <= maximum) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }

            cout << "Invalid input. Enter a value from "
                 << minimum << " to " << maximum << ".\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    static double readDouble(const string& prompt) {
        double value;

        while (true) {
            cout << prompt;

            if (cin >> value && value >= 0 && isfinite(value)) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }

            cout << "Invalid input. Enter a valid non-negative number.\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
};

// Definition of static data member
int CarpoolRide::rideCount = 0;

// Friend function definition
bool isDuplicateRideID(const CarpoolRide rides[],
                       int count, int candidateID) {
    for (int i = 0; i < count; i++) {
        if (rides[i].rideID == candidateID) {
            return true;
        }
    }

    return false;
}

int main() {
    const int MAX_RIDES = 50;

    CarpoolRide rides[MAX_RIDES];
    int choice;
    int count = 0;

    do {
        cout << "\n========== CARPOOL SEAT AND EXPENSE SPLITTER ==========\n";
        cout << "1. Create Ride\n";
        cout << "2. Display All Rides\n";
        cout << "3. Search Ride by ID\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

        case 1: {
            if (count >= MAX_RIDES) {
                cout << "Ride storage is full.\n";
                break;
            }

            CarpoolRide newRide;
            newRide.createRide();

            if (isDuplicateRideID(rides, count, newRide.getRideID())) {
                cout << "Ride ID already exists. Ride not added.\n";
            }
            else {
                rides[count] = newRide;
                count++;
                CarpoolRide::incrementRideCount();

                cout << "\nRide created successfully!\n";
            }

            break;
        }

        case 2: {
            if (count == 0) {
                cout << "\nNo rides available.\n";
                break;
            }

            cout << "\nTotal rides: " << CarpoolRide::getRideCount() << '\n';

            CarpoolRide* ptr = rides;

            for (int i = 0; i < count; i++) {
                (ptr + i)->displayRide();
            }

            break;
        }

        case 3: {
            if (count == 0) {
                cout << "\nNo rides available to search.\n";
                break;
            }

            int searchID = CarpoolRide::getRideCount();

            cout << "\nEnter Ride ID to search: ";

            if (!(cin >> searchID)) {
                cout << "Invalid Ride ID.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool found = false;
            CarpoolRide* ptr = rides;

            for (int i = 0; i < count; i++) {
                if ((ptr + i)->getRideID() == searchID) {
                    cout << "\nRide found!\n";
                    (ptr + i)->displayRide();
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "\nRide ID not found.\n";
            }

            break;
        }

        case 4:
            cout << "\nThank you for using Carpool Seat and Expense Splitter!\n";
            break;

        default:
            cout << "\nInvalid choice. Please select 1 to 4.\n";
        }

    } while (choice != 4);

    return 0;
}
