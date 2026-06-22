#pragma once
#include "IStorage.hpp"
#include <pqxx/pqxx>
#include <string>

class PostgresStorage : public IStorage {
private:
    std::string conn_str;

public:
    PostgresStorage(const std::string& connection_string);

    std::shared_ptr<User> getUserByUsername(const std::string& username) override;
    void createUser(const User& user) override;

    std::vector<std::shared_ptr<Movie>> getAllMovies() override;
    std::vector<std::shared_ptr<TVShow>> getAllTVShows() override;
    void addMovie(const Movie& movie) override;
    void addTVShow(const TVShow& tvshow) override;

    void addRental(const Rental& rental) override;
    std::vector<Rental> getRentalsByUser(int user_id) override;
};
