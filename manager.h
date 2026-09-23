#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Pipe.h"
#include "station.h"
using namespace std;

class Manager {
private:
    unordered_map<int, Pipe> pipes;
    unordered_map<int, Station> stations;
    int next_pipe_id = 1;
    int next_station_id = 1;
    vector<int> found_pipes;

    vector<int> get_pipe_ids();
    vector<int> get_station_ids();
    int ask_pipe_id(string prompt);
    int ask_station_id(string prompt);
    vector<int> ask_id_subset(vector<int> available);
    void print_pipes(vector<int> ids);
    void print_stations(vector<int> ids);
    vector<int> find_pipes_by_name(string part);
    vector<int> find_pipes_by_repair(bool status);
    vector<int> find_stations_by_name(string part);
    vector<int> find_stations_by_unused(double minimum, double maximum);
    void apply_batch(vector<int> ids);

public:
    void display_main_menu();
    void add_pipe();
    void add_station();
    void display_all_pipes();
    void display_all_stations();
    void display_all_objects();
    void edit_pipe();
    void edit_station();
    void delete_pipe();
    void delete_station();
    void search_menu();
    void batch_menu();
    void save_to_file();
    void load_from_file();
};