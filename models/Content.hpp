#pragma once
#include <string>

class Content {
public:
    int id;
    std::string title;
    std::string genre;
    double rating;

    virtual ~Content() = default;
};

class Movie : public Content {
public:
    int duration;
    int rent_cost;
    int purchase_cost;
};

class TVShow : public Content {
public:
    int seasons;
    int episodes_per_season;
    int rent_per_season;
    int purchase_per_season;
};
