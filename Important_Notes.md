# 🚀 Backend Engineering with C++: The Complete Roadmap

Since you already know **C++** and **OOPs (Object-Oriented Programming)**, you have a fantastic foundation. To get to the point where you can comfortably build and understand complex systems like the `Netflix_Inventory_API` we just created, you need to follow a structured path. 

Do not try to learn all of this at once! Follow this roadmap sequentially.

---

## Step 1: Modern C++ Mastery
You know how to create classes and inheritance, but you need to learn how modern C++ (C++11, C++14, C++17, C++20) handles memory safely and efficiently.

*   **RAII (Resource Acquisition Is Initialization):** The most important concept in C++. Learn how constructors and destructors manage memory automatically.
*   **Smart Pointers:** Stop using `new` and `delete`! Learn `std::unique_ptr` and `std::shared_ptr`.
*   **The STL (Standard Template Library):** Master `std::vector`, `std::map`, `std::string`, and algorithms.
*   **Move Semantics:** Learn `std::move` and r-value references to make your code lightning fast.
*   **Lambdas:** Learn how to write anonymous functions (we used these a lot in the Crow routes!).

> [!TIP]
> **Action Item:** Write a small CLI program (like an inventory manager) but *forbid* yourself from using raw pointers (`*`), `new`, or `delete`. Only use smart pointers!

---

## Step 2: Build Systems (CMake)
Professional C++ projects do not compile using a single `g++ main.cpp` command. They use build systems to manage multiple files and external libraries.

*   **CMake Basics:** Learn what a `CMakeLists.txt` file is.
*   **Targets & Linking:** Learn how to use `add_executable` and `target_link_libraries`.
*   **Finding Packages:** Learn how to link external libraries (like how we linked `Threads` and `libpqxx`).

> [!TIP]
> **Action Item:** Take your small CLI program from Step 1, split it into 5 different `.cpp` and `.hpp` files, and successfully compile it using CMake instead of `g++`.

---

## Step 3: Software Design Principles
Before building huge apps, you need to know how to structure them so they don't become a tangled mess.

*   **SOLID Principles:** Learn the 5 principles of object-oriented design. Focus heavily on the **Single Responsibility Principle** and the **Dependency Inversion Principle**.
*   **Controller-Service-Repository Pattern:** Understand how to split your code into Routing (Controller), Logic (Service), and Data (Repository).
*   **Design Patterns:** Learn basic patterns like Factory, Singleton, and Dependency Injection.

> [!TIP]
> **Action Item:** Refactor your CLI program to strictly follow the Service-Repository pattern. Have a "Service" class handle logic and a "Repository" class handle saving data to a text file.

---

## Step 4: Databases & SQL
Text files (`.txt`) are not scalable. You need to learn how relational databases work.

*   **Relational Concepts:** Tables, Rows, Primary Keys, and Foreign Keys.
*   **Basic SQL:** `SELECT`, `INSERT`, `UPDATE`, `DELETE`, `WHERE`.
*   **Advanced SQL:** `JOIN`s, `GROUP BY`, constraints.
*   **ACID Compliance:** Understand why databases are safe for concurrent transactions.
*   **PostgreSQL:** Learn how to use this specific, highly popular database.

> [!TIP]
> **Action Item:** Install PostgreSQL locally. Create a database, create some tables, and practice inserting and querying data using the `psql` command line or a GUI tool like pgAdmin.

---

## Step 5: Web Basics & REST APIs
Now you are ready to connect your C++ logic to the internet.

*   **HTTP Protocol:** Learn how the internet works. What is a GET, POST, PUT, DELETE request? What are Status Codes (200 OK, 404 Not Found, 500 Server Error)?
*   **JSON:** Learn the format used to send data back and forth.
*   **REST Architecture:** Learn the rules for designing good API endpoints (e.g., using `/api/movies` instead of `/api/get_the_movies`).
*   **Crow (C++ Microframework):** Learn how to start a web server in C++ and define routes.

> [!TIP]
> **Action Item:** Write a very simple C++ API using Crow that just returns `{"message": "Hello World"}` when you visit `http://localhost:8080` in your browser. 

---

## Step 6: Database Integration (C++ to SQL)
Connect Step 4 and Step 5 together.

*   **libpqxx:** Learn how to use this C++ library to talk to your PostgreSQL database.
*   **Executing Queries in C++:** Learn how to execute `SELECT` and `INSERT` from within your C++ code.
*   **SQL Injection Prevention:** Learn why you must use parameterized queries (e.g., `$1`, `$2`) instead of string concatenation.

> [!TIP]
> **Action Item:** Upgrade your Crow API so that when you send a POST request, it saves a new record to your PostgreSQL database. (This is exactly what our `Netflix_Inventory_API` does!).

---

## Step 7: Containerization (Docker)
Once your app is built, you need to ensure it runs everywhere.

*   **Docker Concepts:** What is an Image? What is a Container?
*   **Dockerfile:** Learn how to write instructions to build an image for your C++ app.
*   **Docker Compose:** Learn how to spin up multiple containers at the same time (e.g., your C++ API container + a PostgreSQL database container).

> [!TIP]
> **Action Item:** Try to containerize the simple API you built in Step 5 using a Dockerfile. 

---
---

# Netflix Inventory API: Complete Study Guide

This guide is designed to take you from a basic C++ understanding to mastering the advanced software engineering concepts used in the `Netflix_Inventory_API` project. Since this project simulates a professional, production-ready environment, it introduces concepts that go beyond simple coding and focus on **Software Architecture** and **System Design**.

Here is a step-by-step breakdown of everything you need to learn, why we used it, and where it appears in your code.

---

## 1. C++ Memory Safety & RAII

In your original code, you used raw pointers (e.g., `new Movie(...)`). If you forget to call `delete`, you create a **memory leak**. In modern C++ (C++11 and onwards), we use **Smart Pointers** and **RAII** to handle this automatically.

### What is RAII?
**RAII** stands for *Resource Acquisition Is Initialization*. It is a C++ programming concept that binds the life cycle of a resource (like allocated memory, an open file, or a database connection) to the lifespan of an object. 
- When the object is created (Acquisition), it allocates the memory.
- When the object goes out of scope, its destructor is automatically called, safely cleaning up the memory.

### What is `std::shared_ptr`?
A `std::shared_ptr` is a smart pointer that retains shared ownership of an object through a pointer. Multiple `shared_ptr` objects may own the same object. The object is destroyed and its memory deallocated when the last remaining `shared_ptr` owning the object is destroyed.

> [!TIP]
> **Where to learn:** Search for "C++ Smart Pointers (unique_ptr, shared_ptr)" and "C++ RAII" on YouTube (channels like The Cherno or CppCon are great).

**In your code:**
Look at `models/Content.hpp` and `services/CatalogService.hpp`. Instead of returning `Movie*`, we return `std::shared_ptr<Movie>`. This guarantees that once a movie object is no longer being used by the API, the server automatically frees the memory, preventing the API from crashing over time due to memory exhaustion.

---

## 2. SOLID Principles

**SOLID** is an acronym for five design principles intended to make software designs more understandable, flexible, and maintainable.

1. **S - Single Responsibility Principle (SRP):** A class should have one, and only one, reason to change. 
   - *Example:* `AuthService.hpp` only handles logins/registrations. It doesn't care about movies or routing HTTP requests.
2. **O - Open/Closed Principle (OCP):** Software entities should be open for extension, but closed for modification.
   - *Example:* Your `Content` class is a virtual base class. If you want to add "Audiobooks" later, you just create a new class inheriting from `Content`. You don't need to rewrite the existing `Content` code.
3. **L - Liskov Substitution Principle (LSP):** Objects of a superclass shall be replaceable with objects of its subclasses.
   - *Example:* Wherever `Content` is expected, you can safely pass a `Movie` or a `TVShow`.
4. **I - Interface Segregation Principle (ISP):** No client should be forced to depend on methods it does not use.
   - *Example:* We created a clean `IStorage.hpp` interface so our services only see the database commands they need.
5. **D - Dependency Inversion Principle (DIP):** High-level modules should not depend on low-level modules. Both should depend on abstractions (interfaces).
   - *Example:* `CatalogService` depends on `IStorage` (an interface), not directly on `PostgresStorage`. This means if you switch to MySQL tomorrow, `CatalogService` doesn't change at all!

> [!IMPORTANT]
> **Where to learn:** Look up "SOLID Principles in C++" or "Object-Oriented Design Principles".

---

## 3. The Controller-Service-Repository Pattern

This is the standard architectural pattern for modern web backends (used in Spring Boot, NestJS, .NET, etc.). We applied it to your C++ code.

*   **Controllers (`src/main.cpp`):** The "Traffic Cops". They receive the HTTP requests from the internet (e.g., a user clicking "Rent"), unpack the JSON data, and immediately hand the data to the Service layer. They *do not* contain business logic.
*   **Services (`services/`):** The "Brains". This is where your actual Netflix logic lives. `RentalService` calculates due dates and total costs. It doesn't know *how* the internet works, and it doesn't know *how* the database saves data. It only handles the logic.
*   **Repositories (`repositories/`):** The "Librarians". They handle the messy SQL queries (`SELECT`, `INSERT`). They take the clean C++ objects provided by the Service and translate them into a format PostgreSQL understands.

> [!NOTE]
> **Why?** Separation of concerns! If you want to change your database, you only touch the Repository. If you want to add a discount code feature, you only touch the Service.

---

## 4. REST APIs and Crow

Your previous app was a CLI (Command Line Interface), meaning a user had to type in a terminal. A **REST API** allows frontend applications (like a React website or an iOS app) to communicate with your C++ backend over the internet using standard HTTP methods.

*   `GET`: Retrieve data (e.g., `GET /api/movies` fetches the catalog).
*   `POST`: Send new data (e.g., `POST /api/rentals` rents a movie).

**Crow** is the framework we used in `src/main.cpp` to listen for these internet requests and send back JSON responses.

> [!TIP]
> **Where to learn:** Search "What is a REST API?" and "HTTP Methods explained". For Crow specifically, read the [CrowCpp Documentation](https://crowcpp.org/).

---

## 5. Relational Databases & ACID Compliance

Instead of saving data to `_movie.txt`, we upgraded to **PostgreSQL**.

*   **ACID Compliance:** This stands for Atomicity, Consistency, Isolation, Durability. It guarantees that database transactions are processed reliably. For example, if two users try to rent the exact same movie at the exact same millisecond, an ACID-compliant database ensures your system handles it safely without corrupting data.
*   **libpqxx:** This is the C++ library we used in `PostgresStorage.cpp` to send SQL queries to the PostgreSQL database securely (preventing SQL injection).

> [!TIP]
> **Where to learn:** Look up "SQL Basics (SELECT, INSERT, JOIN)" and "What is ACID in databases?".

---

## 6. Docker & Containerization

When you build a complex C++ app, it might work on your computer but crash on someone else's because they have a different version of Ubuntu, GCC, or are missing PostgreSQL. 

*   **Docker (`Dockerfile`):** Creates a lightweight, isolated virtual environment (a "Container") that contains your C++ app and the exact operating system dependencies it needs to run perfectly every time.
*   **Docker Compose (`docker-compose.yml`):** Allows us to spin up *multiple* containers at once and connect them. In your project, Docker Compose boots up a PostgreSQL database container, boots up your C++ API container, and networks them together so they can talk.

> [!TIP]
> **Where to learn:** "Docker in 100 Seconds" by Fireship on YouTube, or "Docker Compose for Beginners".
