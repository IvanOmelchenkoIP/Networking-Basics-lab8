// server/house.cpp

#include "house.h"

#include <iostream>
#include <string>

using namespace std;

HouseController::HouseController(HouseModel house_model, HouseView house_view) {
    this->house_model = house_model;
    this->house_view = house_view;
}

bool HouseController::is_pet_food_present(int connection_fd) {
    bool pet_food_status = house_model.is_pet_food_present();
    house_view.print_is_pet_food_present(connection_fd, pet_food_status);
    return pet_food_status;
}

double HouseController::get_temperature(int connection_fd) {
    double temperature = house_model.get_temperature();
    house_view.print_get_temperature(connection_fd, temperature);
    return temperature;
}

void HouseController::set_doors_status(int connection_fd, bool doors_status) {
    bool curretn_doors_status = house_model.set_doors_status(doors_status);
    house_view.print_set_doors_status(connection_fd, curretn_doors_status);
}

void HouseController::set_heating_status(int connection_fd, bool heating_status) {
    bool current_heating_status = house_model.set_heating_status(heating_status);
    house_view.print_set_heating_status(connection_fd, heating_status);
}
void HouseController::add_pet_food(int connection_fd, double added_amount) {
    double pet_food_amount = house_model.add_pet_food(added_amount);
    house_view.print_add_pet_food(connection_fd, added_amount, pet_food_amount);
}


void HouseView::print_is_pet_food_present(int connection_fd, bool is_pet_food_present) {
    string pet_food_status_message = is_pet_food_present ? "present" : "not present";
    cout << "Client GET retuest at fd " << connection_fd << ". Pet food status: " 
            << pet_food_status_message << ". Results will be sent to client\n";
}

void HouseView::print_get_temperature(int connection_fd, double temperature) {
    cout << "Client GET retuest at fd " << connection_fd << ". Current temperature: " 
            << temperature << ". Results will be sent to client\n";
}

void HouseView::print_set_doors_status(int connection_fd, bool doors_status) {
    string doors_status_message = doors_status ? "open" : "closed";
    cout << "Client SET request at fd " << connection_fd << ". Doors status set to: "
            << doors_status_message << "\n";
}

void HouseView::print_set_heating_status(int connection_fd, bool heating_status) {
    string heating_status_message = heating_status ? "enabled" : "disabled";
    cout << "Client SET retuest at fd " << connection_fd << ". Heating status set to: "
            << heating_status_message << "\n";
}

void HouseView::print_add_pet_food(int connection_fd, double added_amount, double pet_food_amount) {
    cout << "Client ADD retuest at fd " << connection_fd << ". Amount of pet food requested to add: "
            << added_amount << ". Current amount of pet food: " << pet_food_amount << "\n";
}   


bool HouseModel::is_pet_food_present() {
    if (pet_food_amount > 0) pet_food_amount -= PET_FOOD_DECREMENT;
    pet_food_amount = pet_food_amount < 0 ? 0 : pet_food_amount;
    return pet_food_amount <= 0 ? false : true;
}

double HouseModel::get_temperature() {
    return TEMPERATURE;
}

bool HouseModel::set_doors_status(bool doors_status) {
    return this->doors_status = doors_status;
}

bool HouseModel::set_heating_status(bool heating_status) {
    return this->heating_status = heating_status;
}

double HouseModel::add_pet_food(double added_amount) {
    if (pet_food_amount < MAX_PET_FOOD_AMOUNT) pet_food_amount += added_amount;
    return pet_food_amount = pet_food_amount > MAX_PET_FOOD_AMOUNT ? MAX_PET_FOOD_AMOUNT : pet_food_amount;
}
