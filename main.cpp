#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include "Utils.h"
#include "manager.h"
using namespace std;
using namespace std::chrono;

int main()
{
    redirect_output_wrapper cerr_out(cerr);
    string time = format("{:%d_%m_%Y_%H_%M_%S}", zoned_time{ current_zone(), floor<seconds>(system_clock::now()) });
    ofstream logfile("log_" + time + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);

    Manager manager;
    while (1) {
        manager.display_main_menu();
        switch (GetCorrectNumber(0, 15)) {
            case 0:
                return 0;
            case 1:
                manager.add_pipe();
                break;
            case 2:
                manager.add_station();
                break;
            case 3:
                manager.display_all_objects();
                break;
            case 4:
                manager.display_all_pipes();
                break;
            case 5:
                manager.display_all_stations();
                break;
            case 6:
                manager.edit_pipe();
                break;
            case 7:
                manager.edit_station();
                break;
            case 8:
                manager.delete_pipe();
                break;
            case 9:
                manager.delete_station();
                break;
            case 10:
                manager.search_menu();
                break;
            case 11:
                manager.batch_menu();
                break;
            case 12:
                manager.connect_stations();
                break;
            case 13:
                manager.topological_sort();
                break;
            case 14:
                manager.save_to_file();
                break;
            case 15:
                manager.load_from_file();
                break;
        }
    }
}