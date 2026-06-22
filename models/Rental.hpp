#pragma once
#include <string>

class Rental {
public:
    int id;
    int user_id;
    int content_id;
    std::string content_type;
    std::string start_date;
    std::string due_date;
    int cost;
};

class Purchase {
public:
    int id;
    int user_id;
    int content_id;
    std::string content_type;
    int cost;
};
