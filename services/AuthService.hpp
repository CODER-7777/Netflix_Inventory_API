#pragma once
#include <memory>
#include <string>
#include "IStorage.hpp"

class AuthService {
private:
    std::shared_ptr<IStorage> storage;

public:
    AuthService(std::shared_ptr<IStorage> db) : storage(db) {}

    std::shared_ptr<User> login(const std::string& username, const std::string& pin) {
        auto user = storage->getUserByUsername(username);
        if (user && user->pin == pin) {
            return user;
        }
        return nullptr;
    }

    bool registerUser(const std::string& username, const std::string& pin, const std::string& role = "user") {
        auto existing = storage->getUserByUsername(username);
        if (existing) return false;

        User u;
        u.username = username;
        u.pin = pin;
        u.role = role;
        storage->createUser(u);
        return true;
    }
};
