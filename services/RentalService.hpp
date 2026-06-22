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
};
