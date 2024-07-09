#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

struct Time {
    int hour;
    int minute;
};

struct Flight {
    string departureCity;
    Time departureTime;
    string destinationCity;
};

// Function to convert time from string to Time struct
Time stringToTime(const string& timeStr) {
    Time time;
    sscanf(timeStr.c_str(), "%d:%d", &time.hour, &time.minute);
    return time;
}

// Function to calculate arrival time considering time zones
Time calculateArrivalTime(const Time& departureTime, int timeZoneDifference) {
    Time arrivalTime;
    arrivalTime.hour = departureTime.hour + timeZoneDifference;
    arrivalTime.minute = departureTime.minute;
    
    if (arrivalTime.hour < 0) {
        arrivalTime.hour += 24;
    } else if (arrivalTime.hour >= 24) {
        arrivalTime.hour -= 24;
    }
    
    return arrivalTime;
}

int main() {
    char choice;
    do {
        string fileName;
        cout << "Enter the name of the input file: ";
        cin >> fileName;

        ifstream inputFile(fileName);
        if (!inputFile) {
            cerr << "Error: Unable to open file " << fileName << endl;
            return 1;
        }

        int numFlights;
        inputFile >> numFlights;
        inputFile.ignore(); // Ignore the newline character after numFlights

        vector<Flight> flights(numFlights);

        // Read flight data from the file
        for (int i = 0; i < numFlights; ++i) {
            getline(inputFile, flights[i].departureCity, ',');
            string departureTimeStr;
            getline(inputFile, departureTimeStr, ',');
            flights[i].departureTime = stringToTime(departureTimeStr);
            getline(inputFile, flights[i].destinationCity);
        }

        // Output flight details with arrival time calculated
        cout << "Flight Details:" << endl;
        for (int i = 0; i < numFlights; ++i) {
            cout << "Flight " << i + 1 << ": " << flights[i].departureCity << " - "
                 << setw(2) << setfill('0') << flights[i].departureTime.hour << ":"
                 << setw(2) << setfill('0') << flights[i].departureTime.minute << " "
                 << flights[i].destinationCity << " - ";
            
            // Calculate time zone difference based on departure and destination cities
            int timeZoneDifference = flights[i].destinationCity.size() - flights[i].departureCity.size();

            // Calculate and output arrival time
            Time arrivalTime = calculateArrivalTime(flights[i].departureTime, timeZoneDifference);
            cout << setw(2) << setfill('0') << arrivalTime.hour << ":"
                 << setw(2) << setfill('0') << arrivalTime.minute << endl;
        }

        inputFile.close();

        cout << "Do you want to run the program again? (Y/N): ";
        cin >> choice;
    } while (toupper(choice) == 'Y');

    return 0;
}
