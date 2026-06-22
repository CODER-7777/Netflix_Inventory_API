#include "PostgresStorage.hpp"
#include <iostream>

PostgresStorage::PostgresStorage(const std::string& connection_string) : conn_str(connection_string) {
    try {
        pqxx::connection c(conn_str);
        std::cout << "Connected to DB successfully.\n";
    } catch (const std::exception &e) {
        std::cerr << "DB Connection Error: " << e.what() << std::endl;
    }
}

std::shared_ptr<User> PostgresStorage::getUserByUsername(const std::string& username) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        pqxx::result r = w.exec_params("SELECT id, username, pin, role FROM users WHERE username = $1", username);
        
        if (r.empty()) return nullptr;
        
        auto user = std::make_shared<User>();
        user->id = r[0][0].as<int>();
        user->username = r[0][1].as<std::string>();
        user->pin = r[0][2].as<std::string>();
        user->role = r[0][3].as<std::string>();
        return user;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}

void PostgresStorage::createUser(const User& user) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("INSERT INTO users (username, pin, role) VALUES ($1, $2, $3)", 
                      user.username, user.pin, user.role.empty() ? "user" : user.role);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<std::shared_ptr<Movie>> PostgresStorage::getAllMovies() {
    std::vector<std::shared_ptr<Movie>> movies;
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT id, title, genre, rating, duration, rent_cost, purchase_cost FROM movies");
        
        for (auto const& row : r) {
            auto m = std::make_shared<Movie>();
            m->id = row[0].as<int>();
            m->title = row[1].as<std::string>();
            m->genre = row[2].is_null() ? "" : row[2].as<std::string>();
            m->rating = row[3].is_null() ? 0.0 : row[3].as<double>();
            m->duration = row[4].is_null() ? 0 : row[4].as<int>();
            m->rent_cost = row[5].is_null() ? 0 : row[5].as<int>();
            m->purchase_cost = row[6].is_null() ? 0 : row[6].as<int>();
            movies.push_back(m);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return movies;
}

std::vector<std::shared_ptr<TVShow>> PostgresStorage::getAllTVShows() {
    std::vector<std::shared_ptr<TVShow>> shows;
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT id, title, genre, rating, seasons, episodes_per_season, rent_per_season, purchase_per_season FROM tv_shows");
        
        for (auto const& row : r) {
            auto t = std::make_shared<TVShow>();
            t->id = row[0].as<int>();
            t->title = row[1].as<std::string>();
            t->genre = row[2].is_null() ? "" : row[2].as<std::string>();
            t->rating = row[3].is_null() ? 0.0 : row[3].as<double>();
            t->seasons = row[4].is_null() ? 0 : row[4].as<int>();
            t->episodes_per_season = row[5].is_null() ? 0 : row[5].as<int>();
            t->rent_per_season = row[6].is_null() ? 0 : row[6].as<int>();
            t->purchase_per_season = row[7].is_null() ? 0 : row[7].as<int>();
            shows.push_back(t);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return shows;
}

void PostgresStorage::addMovie(const Movie& movie) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("INSERT INTO movies (title, genre, rating, duration, rent_cost, purchase_cost) VALUES ($1, $2, $3, $4, $5, $6)", 
                      movie.title, movie.genre, movie.rating, movie.duration, movie.rent_cost, movie.purchase_cost);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PostgresStorage::addTVShow(const TVShow& tvshow) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("INSERT INTO tv_shows (title, genre, rating, seasons, episodes_per_season, rent_per_season, purchase_per_season) VALUES ($1, $2, $3, $4, $5, $6, $7)", 
                      tvshow.title, tvshow.genre, tvshow.rating, tvshow.seasons, tvshow.episodes_per_season, tvshow.rent_per_season, tvshow.purchase_per_season);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PostgresStorage::addRental(const Rental& rental) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("INSERT INTO rentals (user_id, content_id, content_type, start_date, due_date, cost) VALUES ($1, $2, $3, $4, $5, $6)", 
                      rental.user_id, rental.content_id, rental.content_type, rental.start_date, rental.due_date, rental.cost);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<Rental> PostgresStorage::getRentalsByUser(int user_id) {
    std::vector<Rental> rentals;
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        pqxx::result r = w.exec_params("SELECT id, user_id, content_id, content_type, start_date::text, due_date::text, cost FROM rentals WHERE user_id = $1", user_id);
        
        for (auto const& row : r) {
            Rental rent;
            rent.id = row[0].as<int>();
            rent.user_id = row[1].as<int>();
            rent.content_id = row[2].as<int>();
            rent.content_type = row[3].as<std::string>();
            rent.start_date = row[4].as<std::string>();
            rent.due_date = row[5].as<std::string>();
            rent.cost = row[6].as<int>();
            rentals.push_back(rent);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return rentals;
}

void PostgresStorage::removeMovie(int id) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("DELETE FROM movies WHERE id = $1", id);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PostgresStorage::removeTVShow(int id) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("DELETE FROM tv_shows WHERE id = $1", id);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PostgresStorage::removeRental(int id) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("DELETE FROM rentals WHERE id = $1", id);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PostgresStorage::addPurchase(const Purchase& purchase) {
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        w.exec_params("INSERT INTO purchases (user_id, content_id, content_type, cost) VALUES ($1, $2, $3, $4)", 
                      purchase.user_id, purchase.content_id, purchase.content_type, purchase.cost);
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<Purchase> PostgresStorage::getPurchasesByUser(int user_id) {
    std::vector<Purchase> purchases;
    try {
        pqxx::connection c(conn_str);
        pqxx::work w(c);
        pqxx::result r = w.exec_params("SELECT id, user_id, content_id, content_type, cost FROM purchases WHERE user_id = $1", user_id);
        
        for (auto const& row : r) {
            Purchase p;
            p.id = row[0].as<int>();
            p.user_id = row[1].as<int>();
            p.content_id = row[2].as<int>();
            p.content_type = row[3].as<std::string>();
            p.cost = row[4].as<int>();
            purchases.push_back(p);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return purchases;
}
