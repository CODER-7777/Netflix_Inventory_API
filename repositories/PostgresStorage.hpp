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
    void removeMovie(int id) override;
    void removeTVShow(int id) override;

    void addRental(const Rental& rental) override;
    void removeRental(int id) override;
    std::vector<Rental> getRentalsByUser(int user_id) override;

    void addPurchase(const Purchase& purchase) override;
    std::vector<Purchase> getPurchasesByUser(int user_id) override;
};
