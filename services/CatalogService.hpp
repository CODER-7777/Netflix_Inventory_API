#pragma once
#include <memory>
#include <vector>
#include "IStorage.hpp"

class CatalogService {
private:
    std::shared_ptr<IStorage> storage;

public:
    CatalogService(std::shared_ptr<IStorage> db) : storage(db) {}

    std::vector<std::shared_ptr<Movie>> getMovies() {
        return storage->getAllMovies();
    }

    std::vector<std::shared_ptr<TVShow>> getTVShows() {
        return storage->getAllTVShows();
    }

    void addMovie(const Movie& movie) {
        storage->addMovie(movie);
    }

    void addTVShow(const TVShow& tvshow) {
        storage->addTVShow(tvshow);
    }
};
