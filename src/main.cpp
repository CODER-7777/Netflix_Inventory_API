#include "crow.h"
#include <memory>
#include <cstdlib>
#include "PostgresStorage.hpp"
#include "AuthService.hpp"
#include "CatalogService.hpp"
#include "RentalService.hpp"

int main() {
    crow::SimpleApp app;

    // Get DB configuration from environment variables
    const char* db_host = std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "localhost";
    const char* db_port = std::getenv("DB_PORT") ? std::getenv("DB_PORT") : "5432";
    const char* db_user = std::getenv("DB_USER") ? std::getenv("DB_USER") : "postgres";
    const char* db_pass = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "postgres";
    const char* db_name = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "netflix_inventory";

    std::string conn_str = "host=" + std::string(db_host) + 
                           " port=" + std::string(db_port) + 
                           " dbname=" + std::string(db_name) + 
                           " user=" + std::string(db_user) + 
                           " password=" + std::string(db_pass);

    // Initialize dependencies
    auto storage = std::make_shared<PostgresStorage>(conn_str);
    auto authService = std::make_shared<AuthService>(storage);
    auto catalogService = std::make_shared<CatalogService>(storage);
    auto rentalService = std::make_shared<RentalService>(storage);

    // Auth Routes
    CROW_ROUTE(app, "/api/register").methods("POST"_method)
    ([authService](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);

        std::string username = x["username"].s();
        std::string pin = x["pin"].s();
        
        if (authService->registerUser(username, pin)) {
            return crow::response(201, "User registered");
        }
        return crow::response(409, "User already exists");
    });

    CROW_ROUTE(app, "/api/login").methods("POST"_method)
    ([authService](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);

        std::string username = x["username"].s();
        std::string pin = x["pin"].s();

        auto user = authService->login(username, pin);
        if (user) {
            crow::json::wvalue res;
            res["id"] = user->id;
            res["username"] = user->username;
            res["role"] = user->role;
            return crow::response(res);
        }
        return crow::response(401, "Invalid credentials");
    });

    // Catalog Routes - Movies
    CROW_ROUTE(app, "/api/movies").methods("GET"_method)
    ([catalogService](const crow::request& req) {
        std::string search_query = req.url_params.get("search") ? req.url_params.get("search") : "";
        auto movies = catalogService->searchMovies(search_query);
        
        crow::json::wvalue::list res_list;
        for (const auto& m : movies) {
            crow::json::wvalue item;
            item["id"] = m->id;
            item["title"] = m->title;
            item["genre"] = m->genre;
            item["rating"] = m->rating;
            item["duration"] = m->duration;
            item["rent_cost"] = m->rent_cost;
            item["purchase_cost"] = m->purchase_cost;
            res_list.push_back(std::move(item));
        }
        return crow::response(crow::json::wvalue(res_list));
    });

    CROW_ROUTE(app, "/api/movies").methods("POST"_method)
    ([catalogService](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);
        
        Movie m;
        m.title = x["title"].s();
        m.genre = x["genre"].s();
        m.rating = x["rating"].d();
        m.duration = x["duration"].i();
        m.rent_cost = x["rent_cost"].i();
        m.purchase_cost = x["purchase_cost"].i();

        catalogService->addMovie(m);
        return crow::response(201, "Movie added");
    });

    CROW_ROUTE(app, "/api/movies/<int>").methods("DELETE"_method)
    ([catalogService](int id){
        catalogService->removeMovie(id);
        return crow::response(200, "Movie deleted");
    });

    // Catalog Routes - TV Shows
    CROW_ROUTE(app, "/api/tvshows").methods("GET"_method)
    ([catalogService]() {
        // We'll just fetch all for now
        auto tvshows = catalogService->getTVShows();
        crow::json::wvalue::list res_list;
        for (const auto& m : tvshows) {
            crow::json::wvalue item;
            item["id"] = m->id;
            item["title"] = m->title;
            item["seasons"] = m->seasons;
            item["episodes_per_season"] = m->episodes_per_season;
            item["rent_per_season"] = m->rent_per_season;
            res_list.push_back(std::move(item));
        }
        return crow::response(crow::json::wvalue(res_list));
    });

    CROW_ROUTE(app, "/api/tvshows").methods("POST"_method)
    ([catalogService](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);
        
        TVShow m;
        m.title = x["title"].s();
        m.seasons = x["seasons"].i();
        m.episodes_per_season = x["episodes_per_season"].i();
        m.rent_per_season = x["rent_per_season"].i();

        catalogService->addTVShow(m);
        return crow::response(201, "TV Show added");
    });

    CROW_ROUTE(app, "/api/tvshows/<int>").methods("DELETE"_method)
    ([catalogService](int id){
        catalogService->removeTVShow(id);
        return crow::response(200, "TV Show deleted");
    });

    // Rental & Purchase Routes
    CROW_ROUTE(app, "/api/rentals").methods("POST"_method)
    ([rentalService](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);

        int user_id = x["user_id"].i();
        int content_id = x["content_id"].i();
        std::string type = x["content_type"].s();
        int months = x["months"].i();
        int cost = x["monthly_cost"].i();

        rentalService->rentContent(user_id, content_id, type, months, cost);
        return crow::response(201, "Rented successfully");
    });

    CROW_ROUTE(app, "/api/rentals/<int>").methods("DELETE"_method)
    ([rentalService](int id){
        rentalService->returnRental(id);
        return crow::response(200, "Rental returned");
    });

    CROW_ROUTE(app, "/api/purchases").methods("POST"_method)
    ([rentalService](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400);

        int user_id = x["user_id"].i();
        int content_id = x["content_id"].i();
        std::string type = x["content_type"].s();
        int cost = x["cost"].i();

        rentalService->purchaseContent(user_id, content_id, type, cost);
        return crow::response(201, "Purchased successfully");
    });

    // User Dashboard
    CROW_ROUTE(app, "/api/users/<int>/dashboard").methods("GET"_method)
    ([rentalService](int user_id){
        crow::json::wvalue res;
        
        auto rentals = rentalService->getUserRentals(user_id);
        crow::json::wvalue::list rent_list;
        for (const auto& r : rentals) {
            crow::json::wvalue item;
            item["id"] = r.id;
            item["content_id"] = r.content_id;
            item["content_type"] = r.content_type;
            item["cost"] = r.cost;
            rent_list.push_back(std::move(item));
        }

        auto purchases = rentalService->getUserPurchases(user_id);
        crow::json::wvalue::list purch_list;
        for (const auto& p : purchases) {
            crow::json::wvalue item;
            item["id"] = p.id;
            item["content_id"] = p.content_id;
            item["content_type"] = p.content_type;
            item["cost"] = p.cost;
            purch_list.push_back(std::move(item));
        }

        res["rentals"] = std::move(rent_list);
        res["purchases"] = std::move(purch_list);
        res["total_charges"] = rentalService->calculateTotalCharges(user_id);

        return crow::response(res);
    });

    app.port(8080).multithreaded().run();
    return 0;
}
