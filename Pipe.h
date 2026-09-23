#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Pipe {
private:
    int id = 0;
    string name;
    double length = 0;
    int diameter = 0;
    bool repair = false;

public:
    Pipe() {}
    Pipe(int new_id) : id(new_id) {}

    int get_id() {return id;}
    string get_name() { return name;}
    double get_length() {return length;}
    int get_diameter() {return diameter;}
    bool get_repair() {return repair;}

    void set_repair(bool new_repair) {repair = new_repair;}
    void switch_repair() {repair = !repair;}
    void set_diameter(int new_diameter) {diameter = new_diameter;}

    void input_name_and_length(istream& in);
    void save_pipe(ofstream& outputF);
    void load_pipe(ifstream& loadF);

    friend istream& operator>>(istream& in, Pipe& p);
    friend ostream& operator<<(ostream& out, Pipe& p);
};