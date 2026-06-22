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

    void removeMovie(int id) {
        storage->removeMovie(id);
    }

    void removeTVShow(int id) {
        storage->removeTVShow(id);
    }

    std::vector<std::shared_ptr<Movie>> searchMovies(const std::string& query) {
        auto all = storage->getAllMovies();
        if (query.empty()) return all;
        
        std::vector<std::shared_ptr<Movie>> filtered;
        for (const auto& m : all) {
            if (m->title.find(query) != std::string::npos || m->genre.find(query) != std::string::npos) {
                filtered.push_back(m);
            }
        }
        return filtered;
    }
};
