#include "Pipe.h"
#include "Utils.h"

void Pipe::input_name_and_length(istream& in) {
    cout << "Type pipe name: ";
    INPUT_LINE(in, name);
    cout << "Type pipe length in km: ";
    length = GetCorrectNumber<double>(0.1, 5000);
}

void Pipe::save_pipe(ofstream& outputF) {
    outputF << "PIPE" << endl;
    outputF << name << endl << length << endl << diameter << endl << repair << endl;
}

void Pipe::load_pipe(ifstream& loadF) {
    getline(loadF >> ws, name);
    loadF >> length;
    loadF >> diameter;
    loadF >> repair;
}

istream& operator>>(istream& in, Pipe& p) {
    p.input_name_and_length(in);
    cout << "Type pipe diameter in mm: ";
    p.diameter = GetCorrectNumber<int>(1, 10000);
    p.repair = false;
    return in;
}

ostream& operator<<(ostream& out, Pipe& p) {
    out << "  " << p.name
        << " | length: " << p.length << " km"
        << " | diameter: " << p.diameter << " mm"
        << " | status: " << (p.repair ? "in repair" : "in work") << endl;
    return out;
}