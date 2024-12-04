// server/house.h

#ifndef HOUSE_H
#define HOUSE_H

class HouseView {

public:
    void print_is_pet_food_present(int connection_fd, bool is_pet_food_present);
    void print_get_temperature(int connection_fd, double temperature);
    void print_set_doors_status(int connection_fd, bool doors_status);
    void print_set_heating_status(int connection_fd, bool heating_status);
    void print_add_pet_food(int connection_fd, double added_amount, double pet_food_amount);
};

class HouseModel {

private:
    static constexpr double MAX_PET_FOOD_AMOUNT = 1000;
    static constexpr double PET_FOOD_DECREMENT = 50;
    static constexpr double TEMPERATURE = 25.5;

    bool doors_status;
    bool heating_status;
    double pet_food_amount;

public:
    bool is_pet_food_present();
    double get_temperature();
    bool set_doors_status(bool doors_status);
    bool set_heating_status(bool heating_status);
    double add_pet_food(double added_amount);
};

class HouseController {

private:
    HouseModel house_model;
    HouseView house_view;

public:
    HouseController() {};
    HouseController(HouseModel house_model, HouseView house_view);
    bool is_pet_food_present(int connection_fd);
    double get_temperature(int connection_fd);
    void set_doors_status(int connection_fd, bool doors_status);
    void set_heating_status(int connection_fd, bool heating_status);
    void add_pet_food(int connection_fd, double added_amount);
};

#endif
