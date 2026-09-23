#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Station {
private:
    int id = 0;
    string name;
    int workshops = 0;
    int workshops_in_work = 0;
    string station_class;

public:
    Station() {}
    Station(int new_id) : id(new_id) {}

    int get_id() { return id; }
    string get_name() { return name; }
    int get_workshops() { return workshops; }
    int get_workshops_in_work() { return workshops_in_work; }
    string get_station_class() { return station_class; }

    double get_unused_percentage();
    bool start_workshop();
    bool stop_workshop();
    void set_workshops_in_work(int value);

    void save_station(ofstream& outputF);
    void load_station(ifstream& loadF);

    friend istream& operator>>(istream& in, Station& s);
    friend ostream& operator<<(ostream& out, Station& s);
};