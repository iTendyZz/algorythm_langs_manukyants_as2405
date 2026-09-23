#include "Pipe.h"
#include "Utils.h"
#include <cmath>
#include <limits>

void Pipe::input_name_and_length(istream& in) {
    cout << "Type pipe name: ";
    INPUT_LINE(in, name);

    cout << "Type pipe length in km: ";
    length = GetCorrectNumber<double>(0.1, 5000);
}

double Pipe::get_throughput() {
    if (repair || length <= 0 || diameter <= 0)
        return 0;

    double diameter_in_metres = diameter / 1000.0;
    double length_in_metres = length * 1000.0;
    double correction = 1000000.0; 

    return sqrt(pow(diameter_in_metres, 5) / length_in_metres) * correction;
}

double Pipe::get_weight() {
    if (repair)
        return numeric_limits<double>::infinity();

    return length;
}

void Pipe::save_pipe(ofstream& outputF) {
    outputF << "PIPE" << endl;
    outputF << id << endl << name << endl << length << endl << diameter << endl << repair << endl;
}

void Pipe::load_pipe(ifstream& loadF) {
    loadF >> id;
    getline(loadF >> ws, name);
    loadF >> length;
    loadF >> diameter;
    loadF >> repair;
}

istream& operator>>(istream& in, Pipe& p) {
    p.input_name_and_length(in);

    cout << "Type pipe diameter in mm (500, 700, 1000, 1400): ";
    p.diameter = (int)GetCorrectDiameter();
    p.repair = false;

    return in;
}

ostream& operator<<(ostream& out, Pipe& p) {
    out << "  ID " << p.id << ": " << p.name
        << " | length: " << p.length << " km"
        << " | diameter: " << p.diameter << " mm"
        << " | status: " << (p.repair ? "in repair" : "in work")
        << " | throughput: " << p.get_throughput() << endl;

    return out;
}
