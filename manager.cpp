#include "manager.h"
#include "Utils.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;

static string to_lower(string text) {
    for (size_t i = 0; i < text.size(); i++)
        text[i] = (char)tolower((unsigned char)text[i]);
    return text;
}

void Manager::display_main_menu() {
    cout << endl << "MAIN MENU:" << endl
        << "1. Add pipe" << endl
        << "2. Add station" << endl
        << "3. View all objects" << endl
        << "4. View all pipes" << endl
        << "5. View all stations" << endl
        << "6. Edit pipe" << endl
        << "7. Edit station" << endl
        << "8. Delete pipe" << endl
        << "9. Delete station" << endl
        << "10. Search objects" << endl
        << "11. Batch operations with pipes" << endl
        << "12. Connect stations" << endl
        << "13. Topological sort" << endl
        << "14. Save to file" << endl
        << "15. Load from file" << endl
        << "0. Exit" << endl
        << "Choose option: ";
}

vector<int> Manager::get_pipe_ids() {
    vector<int> ids;
    for (auto& [id, pipe] : pipes)
        ids.push_back(id);
    sort(ids.begin(), ids.end());
    return ids;
}

vector<int> Manager::get_station_ids() {
    vector<int> ids;
    for (auto& [id, station] : stations)
        ids.push_back(id);
    sort(ids.begin(), ids.end());
    return ids;
}

int Manager::ask_pipe_id(string prompt) {
    vector<int> ids = get_pipe_ids();
    cout << "Available pipe IDs: ";
    for (int id : ids)
        cout << id << ' ';
    cout << endl << prompt;
    while (true) {
        int id = GetCorrectNumber(ids.front(), ids.back());
        if (find(ids.begin(), ids.end(), id) != ids.end())
            return id;
        cout << "There is no pipe with ID " << id << ". Try again: ";
    }
}

int Manager::ask_station_id(string prompt) {
    vector<int> ids = get_station_ids();
    cout << "Available station IDs: ";
    for (int id : ids)
        cout << id << ' ';
    cout << endl << prompt;
    while (true) {
        int id = GetCorrectNumber(ids.front(), ids.back());
        if (find(ids.begin(), ids.end(), id) != ids.end())
            return id;
        cout << "There is no station with ID " << id << ". Try again: ";
    }
}

vector<int> Manager::ask_id_subset(vector<int> available) {
    cout << "Available IDs: ";
    for (int id : available)
        cout << id << ' ';
    cout << endl << "Enter one ID per line, 0 to finish." << endl;
    vector<int> chosen;
    while (true) {
        cout << "ID: ";
        int id = GetCorrectNumber(0, available.back());
        if (id == 0)
            break;
        if (find(available.begin(), available.end(), id) == available.end()) {
            cout << "ID " << id << " is not in the list." << endl;
            continue;
        }
        if (find(chosen.begin(), chosen.end(), id) != chosen.end()) {
            cout << "ID " << id << " is already selected." << endl;
            continue;
        }
        chosen.push_back(id);
    }
    sort(chosen.begin(), chosen.end());
    return chosen;
}

void Manager::print_pipes(vector<int> ids) {
    if (ids.empty()) {
        cout << "No pipes found." << endl;
        return;
    }
    cout << "Found " << ids.size() << " pipes:" << endl;
    for (int id : ids)
        if (pipes.count(id))
            cout << pipes[id];
}

void Manager::print_stations(vector<int> ids) {
    if (ids.empty()) {
        cout << "No stations found." << endl;
        return;
    }
    cout << "Found " << ids.size() << " stations:" << endl;
    for (int id : ids)
        if (stations.count(id))
            cout << stations[id];
}

void Manager::add_pipe() {
    Pipe new_pipe(next_pipe_id);
    cin >> new_pipe;
    pipes[next_pipe_id] = new_pipe;
    cout << "Pipe added with ID " << next_pipe_id << "." << endl;
    next_pipe_id++;
}

int Manager::create_pipe(int diameter) {
    Pipe new_pipe(next_pipe_id);
    new_pipe.input_name_and_length(cin);
    new_pipe.set_diameter(diameter);
    pipes[next_pipe_id] = new_pipe;
    cout << "Pipe added with ID " << next_pipe_id << "." << endl;
    next_pipe_id++;
    return next_pipe_id - 1;
}

void Manager::add_station() {
    Station new_station(next_station_id);
    cin >> new_station;
    stations[next_station_id] = new_station;
    cout << "Station added with ID " << next_station_id << "." << endl;
    next_station_id++;
}

void Manager::display_all_pipes() {
    if (pipes.empty()) {
        cout << "No pipes available." << endl;
        return;
    }
    cout << "ALL PIPES:" << endl;
    print_pipes(get_pipe_ids());
}

void Manager::display_all_stations() {
    if (stations.empty()) {
        cout << "No stations available." << endl;
        return;
    }
    cout << "ALL STATIONS:" << endl;
    print_stations(get_station_ids());
}

void Manager::display_all_objects() {
    display_all_pipes();
    display_all_stations();
    if (connections.empty()) {
        cout << "No connections available." << endl;
        return;
    }
    cout << "ALL CONNECTIONS:" << endl;
    for (int id : get_pipe_ids())
        if (connections.count(id))
            cout << "  station " << connections[id].first << " -> station "
                << connections[id].second << " by pipe " << id << endl;
}

void Manager::edit_pipe() {
    if (pipes.empty()) {
        cout << "No pipes available." << endl;
        return;
    }
    int id = ask_pipe_id("Enter ID of the pipe to edit: ");
    cout << pipes[id];
    cout << "1. Switch repair status" << endl
        << "2. Send the pipe to repair" << endl
        << "3. Return the pipe to work" << endl
        << "0. Cancel" << endl
        << "Choose option: ";
    switch (GetCorrectNumber(0, 3)) {
    case 1:
        pipes[id].switch_repair();
        break;
    case 2:
        pipes[id].set_repair(true);
        break;
    case 3:
        pipes[id].set_repair(false);
        break;
    default:
        cout << "Nothing was changed." << endl;
        return;
    }
    cout << "Pipe " << id << " is now " << (pipes[id].get_repair() ? "in repair" : "in work") << "." << endl;
}

void Manager::edit_station() {
    if (stations.empty()) {
        cout << "No stations available." << endl;
        return;
    }
    int id = ask_station_id("Enter ID of the station to edit: ");
    cout << stations[id];
    cout << "1. Start one workshop" << endl
        << "2. Stop one workshop" << endl
        << "3. Set the amount of workshops in work" << endl
        << "0. Cancel" << endl
        << "Choose option: ";
    switch (GetCorrectNumber(0, 3)) {
    case 1:
        if (!stations[id].start_workshop()) {
            cout << "All workshops are already in work." << endl;
            return;
        }
        break;
    case 2:
        if (!stations[id].stop_workshop()) {
            cout << "All workshops are already stopped." << endl;
            return;
        }
        break;
    case 3:
        cout << "Type amount of workshops in work (0 - " << stations[id].get_workshops() << "): ";
        stations[id].set_workshops_in_work(GetCorrectNumber(0, stations[id].get_workshops()));
        break;
    default:
        cout << "Nothing was changed." << endl;
        return;
    }
    cout << "Station " << id << " now has " << stations[id].get_workshops_in_work()
        << " of " << stations[id].get_workshops() << " workshops in work." << endl;
}

void Manager::delete_pipe() {
    if (pipes.empty()) {
        cout << "No pipes available." << endl;
        return;
    }
    int id = ask_pipe_id("Enter ID of the pipe to delete: ");
    pipes.erase(id);
    cout << "Pipe " << id << " deleted." << endl;
    if (connections.erase(id) > 0)
        cout << "The connection that used this pipe was removed too." << endl;
}

void Manager::delete_station() {
    if (stations.empty()) {
        cout << "No stations available." << endl;
        return;
    }
    int id = ask_station_id("Enter ID of the station to delete: ");
    stations.erase(id);
    cout << "Station " << id << " deleted." << endl;
    int removed = 0;
    for (auto it = connections.begin(); it != connections.end();) {
        if (it->second.first == id || it->second.second == id) {
            it = connections.erase(it);
            removed++;
        }
        else {
            ++it;
        }
    }
    if (removed > 0)
        cout << removed << " connections of this station were removed too." << endl;
}

vector<int> Manager::find_pipes_by_name(string part) {
    vector<int> result;
    for (auto& [id, pipe] : pipes)
        if (to_lower(pipe.get_name()).find(to_lower(part)) != string::npos)
            result.push_back(id);
    sort(result.begin(), result.end());
    return result;
}

vector<int> Manager::find_pipes_by_repair(bool status) {
    vector<int> result;
    for (auto& [id, pipe] : pipes)
        if (pipe.get_repair() == status)
            result.push_back(id);
    sort(result.begin(), result.end());
    return result;
}

vector<int> Manager::find_stations_by_name(string part) {
    vector<int> result;
    for (auto& [id, station] : stations)
        if (to_lower(station.get_name()).find(to_lower(part)) != string::npos)
            result.push_back(id);
    sort(result.begin(), result.end());
    return result;
}

vector<int> Manager::find_stations_by_unused(double minimum, double maximum) {
    vector<int> result;
    for (auto& [id, station] : stations)
        if (station.get_unused_percentage() >= minimum && station.get_unused_percentage() <= maximum)
            result.push_back(id);
    sort(result.begin(), result.end());
    return result;
}

vector<int> Manager::find_free_pipes(int diameter) {
    vector<int> result;
    for (auto& [id, pipe] : pipes)
        if (pipe.get_diameter() == diameter && connections.count(id) == 0)
            result.push_back(id);
    sort(result.begin(), result.end());
    return result;
}

void Manager::search_menu() {
    while (true) {
        cout << endl << "SEARCH MENU:" << endl
            << "1. Find pipes by name" << endl
            << "2. Find pipes by repair status" << endl
            << "3. Find stations by name" << endl
            << "4. Find stations by percentage of unused workshops" << endl
            << "0. Back to main menu" << endl
            << "Choose option: ";
        switch (GetCorrectNumber(0, 4)) {
        case 0:
            return;
        case 1: {
            string part;
            cout << "Enter part of the pipe name: ";
            INPUT_LINE(cin, part);
            found_pipes = find_pipes_by_name(part);
            print_pipes(found_pipes);
            break;
        }
        case 2: {
            cout << "Enter repair status (0 - in work, 1 - in repair): ";
            found_pipes = find_pipes_by_repair(GetCorrectNumber(0, 1) == 1);
            print_pipes(found_pipes);
            break;
        }
        case 3: {
            string part;
            cout << "Enter part of the station name: ";
            INPUT_LINE(cin, part);
            print_stations(find_stations_by_name(part));
            break;
        }
        case 4: {
            cout << "Enter minimum percentage of unused workshops: ";
            double minimum = GetCorrectNumber<double>(0, 100);
            cout << "Enter maximum percentage of unused workshops: ";
            double maximum = GetCorrectNumber<double>(minimum, 100);
            print_stations(find_stations_by_unused(minimum, maximum));
            break;
        }
        }
    }
}

void Manager::batch_menu() {
    if (pipes.empty()) {
        cout << "No pipes available." << endl;
        return;
    }
    cout << endl << "SELECT PIPES:" << endl
        << "1. All pipes" << endl
        << "2. Pipes found by name" << endl
        << "3. Pipes with a given repair status" << endl
        << "4. Pipes chosen by ID" << endl
        << "5. Result of the last search" << endl
        << "0. Cancel" << endl
        << "Choose option: ";
    vector<int> selected;
    switch (GetCorrectNumber(0, 5)) {
    case 0:
        return;
    case 1:
        selected = get_pipe_ids();
        break;
    case 2: {
        string part;
        cout << "Enter part of the pipe name: ";
        INPUT_LINE(cin, part);
        selected = find_pipes_by_name(part);
        break;
    }
    case 3:
        cout << "Enter repair status (0 - in work, 1 - in repair): ";
        selected = find_pipes_by_repair(GetCorrectNumber(0, 1) == 1);
        break;
    case 4:
        selected = ask_id_subset(get_pipe_ids());
        break;
    case 5:
        selected = found_pipes;
        break;
    }
    if (selected.empty()) {
        cout << "No pipes selected for the batch operation." << endl;
        return;
    }
    cout << "Selected " << selected.size() << " pipes:" << endl;
    print_pipes(selected);
    apply_batch(selected);
}

void Manager::apply_batch(vector<int> ids) {
    cout << endl << "BATCH OPERATION:" << endl
        << "1. Send all selected pipes to repair" << endl
        << "2. Return all selected pipes to work" << endl
        << "3. Switch repair status of all selected pipes" << endl
        << "4. Delete all selected pipes" << endl
        << "5. Choose a part of the selection and continue" << endl
        << "0. Cancel" << endl
        << "Choose option: ";
    int changed = 0;
    switch (GetCorrectNumber(0, 5)) {
    case 1:
        for (int id : ids)
            if (pipes.count(id)) {
                pipes[id].set_repair(true);
                changed++;
            }
        cout << changed << " pipes sent to repair." << endl;
        break;
    case 2:
        for (int id : ids)
            if (pipes.count(id)) {
                pipes[id].set_repair(false);
                changed++;
            }
        cout << changed << " pipes returned to work." << endl;
        break;
    case 3:
        for (int id : ids)
            if (pipes.count(id)) {
                pipes[id].switch_repair();
                changed++;
            }
        cout << changed << " pipes changed their status." << endl;
        break;
    case 4: {
        cout << "Delete " << ids.size() << " pipes? (1 - yes, 0 - no): ";
        if (GetCorrectNumber(0, 1) == 0) {
            cout << "Operation canceled." << endl;
            return;
        }
        for (int id : ids) {
            if (pipes.erase(id) > 0)
                changed++;
            connections.erase(id);
        }
        found_pipes.clear();
        cout << changed << " pipes deleted." << endl;
        break;
    }
    case 5: {
        vector<int> part = ask_id_subset(ids);
        if (part.empty()) {
            cout << "No pipes selected for the batch operation." << endl;
            return;
        }
        cout << "Selected " << part.size() << " pipes:" << endl;
        print_pipes(part);
        apply_batch(part);
        break;
    }
    default:
        cout << "Operation canceled." << endl;
        break;
    }
}

void Manager::connect_stations() {
    if (stations.size() < 2) {
        cout << "At least two stations are required." << endl;
        return;
    }
    int from = ask_station_id("Enter ID of the input station: ");
    int to = ask_station_id("Enter ID of the output station: ");
    while (to == from) {
        cout << "Input and output stations must be different." << endl;
        to = ask_station_id("Enter ID of the output station: ");
    }
    cout << "Enter pipe diameter in mm (500, 700, 1000, 1400): ";
    int diameter = (int)GetCorrectDiameter();
    vector<int> free_pipes = find_free_pipes(diameter);
    int pipe_id = 0;
    if (free_pipes.empty()) {
        cout << "There is no free pipe with diameter " << diameter << " mm, a new one will be created." << endl;
        pipe_id = create_pipe(diameter);
    }
    else {
        cout << "Free pipes with diameter " << diameter << " mm:" << endl;
        print_pipes(free_pipes);
        cout << "Enter ID of the pipe to use, or 0 to create a new pipe: ";
        while (true) {
            pipe_id = GetCorrectNumber(0, free_pipes.back());
            if (pipe_id == 0) {
                pipe_id = create_pipe(diameter);
                break;
            }
            if (find(free_pipes.begin(), free_pipes.end(), pipe_id) != free_pipes.end())
                break;
            cout << "Pipe " << pipe_id << " is not in the list. Try again: ";
        }
    }
    connections[pipe_id] = make_pair(from, to);
    cout << "Station " << from << " -> station " << to << " connected by pipe " << pipe_id << "." << endl;
}

void Manager::topological_sort() {
    if (stations.empty()) {
        cout << "No stations available." << endl;
        return;
    }
    unordered_map<int, vector<int>> adjacency;
    unordered_map<int, int> incoming;
    for (auto& [id, station] : stations)
        incoming[id] = 0;
    for (auto& [pipe_id, connection] : connections) {
        adjacency[connection.first].push_back(connection.second);
        incoming[connection.second]++;
    }
    vector<int> order;
    while (true) {
        int current = 0;
        for (auto& [id, degree] : incoming)
            if (degree == 0 && (current == 0 || id < current))
                current = id;
        if (current == 0)
            break;
        incoming[current] = -1;
        order.push_back(current);
        for (int next : adjacency[current])
            incoming[next]--;
    }
    if (order.size() != stations.size()) {
        cout << "The network has a cycle, topological sort is impossible." << endl;
        return;
    }
    cout << "Topological order of stations: ";
    for (int id : order)
        cout << id << ' ';
    cout << endl;
}

void Manager::save_to_file() {
    string filename;
    cout << "Enter file name to save: ";
    INPUT_LINE(cin, filename);
    ofstream outputF(filename);
    if (!outputF.is_open()) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    outputF << "NEXT_IDS" << endl << next_pipe_id << ' ' << next_station_id << endl;
    for (auto& [id, pipe] : pipes)
        pipe.save_pipe(outputF);
    for (auto& [id, station] : stations)
        station.save_station(outputF);
    for (auto& [pipe_id, connection] : connections)
        outputF << "CONNECTION" << endl << pipe_id << endl
            << connection.first << ' ' << connection.second << endl;
    cout << "Successfully saved to " << filename << endl;
}

void Manager::load_from_file() {
    string filename;
    cout << "Enter file name to load: ";
    INPUT_LINE(cin, filename);
    ifstream loadF(filename);
    if (!loadF.is_open()) {
        cout << "Error opening file for reading!" << endl;
        return;
    }
    Manager loaded;
    string marker;
    while (loadF >> marker) {
        if (marker == "NEXT_IDS") {
            loadF >> loaded.next_pipe_id >> loaded.next_station_id;
        }
        else if (marker == "PIPE") {
            Pipe pipe;
            pipe.load_pipe(loadF);
            loaded.pipes[pipe.get_id()] = pipe;
        }
        else if (marker == "CS") {
            Station station;
            station.load_station(loadF);
            loaded.stations[station.get_id()] = station;
        }
        else if (marker == "CONNECTION") {
            int pipe_id = 0;
            int from = 0;
            int to = 0;
            loadF >> pipe_id >> from >> to;
            loaded.connections[pipe_id] = make_pair(from, to);
        }
        else {
            cout << "File " << filename << " is damaged, nothing was loaded." << endl;
            return;
        }
        if (loadF.fail() && !loadF.eof()) {
            cout << "File " << filename << " is damaged, nothing was loaded." << endl;
            return;
        }
    }
    loaded.drop_broken_connections();
    pipes = loaded.pipes;
    stations = loaded.stations;
    connections = loaded.connections;
    next_pipe_id = loaded.next_pipe_id;
    next_station_id = loaded.next_station_id;
    found_pipes.clear();
    for (auto& [id, pipe] : pipes)
        if (id >= next_pipe_id)
            next_pipe_id = id + 1;
    for (auto& [id, station] : stations)
        if (id >= next_station_id)
            next_station_id = id + 1;
    cout << "Successfully loaded from " << filename << endl;
    cout << pipes.size() << " pipes, " << stations.size() << " stations, "
        << connections.size() << " connections." << endl;
}

void Manager::drop_broken_connections() {
    for (auto it = connections.begin(); it != connections.end();) {
        if (pipes.count(it->first) == 0 ||
            stations.count(it->second.first) == 0 ||
            stations.count(it->second.second) == 0 ||
            it->second.first == it->second.second)
            it = connections.erase(it);
        else
            ++it;
    }
}