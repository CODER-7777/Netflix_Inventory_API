#pragma once
#include <vector>
#include <memory>
#include "Content.hpp"
#include "User.hpp"
#include "Rental.hpp"

class IStorage {
public:
    virtual ~IStorage() = default;

    // User Operations
    virtual std::shared_ptr<User> getUserByUsername(const std::string& username) = 0;
    virtual void createUser(const User& user) = 0;

    // Catalog Operations
    virtual std::vector<std::shared_ptr<Movie>> getAllMovies() = 0;
    virtual std::vector<std::shared_ptr<TVShow>> getAllTVShows() = 0;
    virtual void addMovie(const Movie& movie) = 0;
    virtual void addTVShow(const TVShow& tvshow) = 0;

    // Rental Operations
    virtual void addRental(const Rental& rental) = 0;
    virtual std::vector<Rental> getRentalsByUser(int user_id) = 0;
};
