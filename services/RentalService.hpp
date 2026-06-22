#pragma once
#include <memory>
#include <vector>
#include "IStorage.hpp"

class RentalService {
private:
    std::shared_ptr<IStorage> storage;

public:
    RentalService(std::shared_ptr<IStorage> db) : storage(db) {}

    void rentContent(int user_id, int content_id, const std::string& type, int months, int monthly_cost) {
        Rental r;
        r.user_id = user_id;
        r.content_id = content_id;
        r.content_type = type;
        r.cost = monthly_cost * months;
        r.start_date = "CURRENT_DATE"; // Simplified for SQL, ideally generated here
        r.due_date = "CURRENT_DATE + INTERVAL '" + std::to_string(months) + " months'"; // specific to postgres
        
        storage->addRental(r);
    }

    std::vector<Rental> getUserRentals(int user_id) {
        return storage->getRentalsByUser(user_id);
    }

    void returnRental(int rental_id) {
        storage->removeRental(rental_id);
    }

    void purchaseContent(int user_id, int content_id, const std::string& type, int cost) {
        Purchase p;
        p.user_id = user_id;
        p.content_id = content_id;
        p.content_type = type;
        p.cost = cost;
        storage->addPurchase(p);
    }

    std::vector<Purchase> getUserPurchases(int user_id) {
        return storage->getPurchasesByUser(user_id);
    }

    int calculateTotalCharges(int user_id) {
        int total = 0;
        auto rentals = storage->getRentalsByUser(user_id);
        for (const auto& r : rentals) total += r.cost;

        auto purchases = storage->getPurchasesByUser(user_id);
        for (const auto& p : purchases) total += p.cost;

        return total;
    }
};
