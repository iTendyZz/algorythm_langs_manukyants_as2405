#include "station.h"
#include "Utils.h"

double Station::get_unused_percentage() {
    if (workshops <= 0)
        return 0;
    return (workshops - workshops_in_work) / (double)workshops * 100;
}

bool Station::start_workshop() {
    if (workshops_in_work >= workshops)
        return false;
    workshops_in_work++;
    return true;
}

bool Station::stop_workshop() {
    if (workshops_in_work <= 0)
        return false;
    workshops_in_work--;
    return true;
}

void Station::set_workshops_in_work(int value) {
    if (value < 0)
        value = 0;
    if (value > workshops)
        value = workshops;
    workshops_in_work = value;
}

void Station::save_station(ofstream& outputF) {
    outputF << "CS" << endl;
    outputF << id << endl << name << endl << workshops << endl
            << workshops_in_work << endl << station_class << endl;
}

void Station::load_station(ifstream& loadF) {
    loadF >> id;
    getline(loadF >> ws, name);
    loadF >> workshops;
    loadF >> workshops_in_work;
    getline(loadF >> ws, station_class);
}

istream& operator>>(istream& in, Station& s) {
    cout << "Type station name: ";
    INPUT_LINE(in, s.name);
    cout << "Type amount of workshops: ";
    s.workshops = GetCorrectNumber<int>(1, 100);
    cout << "Type amount of workshops in work: ";
    s.workshops_in_work = GetCorrectNumber<int>(0, s.workshops);
    cout << "Type class of station: ";
    INPUT_LINE(in, s.station_class);
    return in;
}

ostream& operator<<(ostream& out, Station& s) {
    out << "  ID " << s.id << ": " << s.name
        << " | workshops: " << s.workshops_in_work << " of " << s.workshops << " in work"
        << " | unused: " << s.get_unused_percentage() << " %"
        << " | class: " << s.station_class << endl;
    return out;
}