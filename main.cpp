#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"
#include "Pipe.h"
#include "station.h"
using namespace std;

int main()
{
    Pipe pipe;
    Station station;
    bool pipe_exists = false;
    bool station_exists = false;

    while (1) {
        cout << endl << "MAIN MENU:" << endl
            << "1. Add pipe" << endl
            << "2. Add station" << endl
            << "3. View all objects" << endl
            << "4. Edit pipe" << endl
            << "5. Edit station" << endl
            << "6. Save to file" << endl
            << "7. Load from file" << endl
            << "0. Exit" << endl
            << "Choose option: ";

        switch (GetCorrectNumber(0, 7)) {
        case 0:
            return 0;
        case 1:
            cin >> pipe;
            pipe_exists = true;
            cout << "Pipe added." << endl;
            break;
        case 2:
            cin >> station;
            station_exists = true;
            cout << "Station added." << endl;
            break;
        case 3:
            if (pipe_exists)
                cout << pipe;
            else
                cout << "No pipe available." << endl;
            if (station_exists)
                cout << station;
            else
                cout << "No station available." << endl;
            break;
        case 4:
            if (!pipe_exists) {
                cout << "No pipe available." << endl;
                break;
            }
            cout << pipe;
            cout << "1. Switch repair status" << endl
                << "2. Send the pipe to repair" << endl
                << "3. Return the pipe to work" << endl
                << "0. Cancel" << endl
                << "Choose option: ";
            switch (GetCorrectNumber(0, 3)) {
            case 1:
                pipe.switch_repair();
                break;
            case 2:
                pipe.set_repair(true);
                break;
            case 3:
                pipe.set_repair(false);
                break;
            default:
                cout << "Nothing was changed." << endl;
                break;
            }
            cout << "Pipe is now " << (pipe.get_repair() ? "in repair" : "in work") << "." << endl;
            break;
        case 5:
            if (!station_exists) {
                cout << "No station available." << endl;
                break;
            }
            cout << station;
            cout << "1. Start one workshop" << endl
                << "2. Stop one workshop" << endl
                << "3. Set the amount of workshops in work" << endl
                << "0. Cancel" << endl
                << "Choose option: ";
            switch (GetCorrectNumber(0, 3)) {
            case 1:
                if (!station.start_workshop())
                    cout << "All workshops are already in work." << endl;
                break;
            case 2:
                if (!station.stop_workshop())
                    cout << "All workshops are already stopped." << endl;
                break;
            case 3:
                cout << "Type amount of workshops in work (0 - " << station.get_workshops() << "): ";
                station.set_workshops_in_work(GetCorrectNumber(0, station.get_workshops()));
                break;
            default:
                cout << "Nothing was changed." << endl;
                break;
            }
            cout << "Station now has " << station.get_workshops_in_work()
                << " of " << station.get_workshops() << " workshops in work." << endl;
            break;
        case 6: {
            string filename;
            cout << "Enter file name to save: ";
            INPUT_LINE(cin, filename);
            ofstream outputF(filename);
            if (!outputF.is_open()) {
                cout << "Error opening file for writing!" << endl;
                break;
            }
            if (pipe_exists)
                pipe.save_pipe(outputF);
            if (station_exists)
                station.save_station(outputF);
            cout << "Successfully saved to " << filename << endl;
            break;
        }
        case 7: {
            string filename;
            cout << "Enter file name to load: ";
            INPUT_LINE(cin, filename);
            ifstream loadF(filename);
            if (!loadF.is_open()) {
                cout << "Error opening file for reading!" << endl;
                break;
            }
            string marker;
            while (loadF >> marker) {
                if (marker == "PIPE") {
                    pipe.load_pipe(loadF);
                    pipe_exists = true;
                }
                else if (marker == "CS") {
                    station.load_station(loadF);
                    station_exists = true;
                }
                else {
                    cout << "File " << filename << " is damaged, nothing was loaded." << endl;
                    break;
                }
            }
            cout << "Successfully loaded from " << filename << endl;
            break;
        }
        }
    }
}