# Netflix Inventory API

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Crow](https://img.shields.io/badge/Framework-Crow-orange)
![PostgreSQL](https://img.shields.io/badge/Database-PostgreSQL-316192)
![Docker](https://img.shields.io/badge/Docker-Enabled-2496ED.svg)

A scalable, production-ready RESTful API for managing a Netflix-style streaming catalog. Built entirely in C++20, this system handles concurrent user authentication, content browsing, and stateful rental transactions while ensuring strict memory safety and ACID-compliant database operations.

---

## System Architecture

The application strictly adheres to the **Controller-Service-Repository** pattern, entirely decoupling HTTP routing logic from core business rules and database operations.

```mermaid
graph TD
    Client([API Client / Frontend])
    
    subgraph "Dockerized C++ API Server (Crow)"
        Controller[Controller Layer\nHTTP Routing & JSON Validation]
        Service[Service Layer\nBusiness Logic & Domain Rules]
        Repo[Repository Layer\nIStorage Interface]
        
        Client <-->|REST / JSON| Controller
        Controller <-->|C++ Domain Models| Service
        Service <-->|CRUD Operations| Repo
    end
    
    DB[(PostgreSQL\nACID Compliant)]
    Repo <-->|libpqxx SQL Queries| DB
```

---

## Key Features

- **Memory Safety via RAII:** Eliminates memory leaks and undefined behavior by enforcing strict modern C++ RAII principles and `std::shared_ptr` ownership models.
- **SOLID Design:** Designed with a highly extensible, interface-driven codebase. New subscription models or content types can be injected without modifying underlying routing or database layers.
- **Polymorphic Domain Model:** Utilizes virtual base classes to efficiently manage varying content types (Movies, TV Shows) under a unified system.
- **Persistent Transactions:** Integrates PostgreSQL via a custom repository interface to guarantee stateful, ACID-compliant concurrency.
- **Dockerized Deployment:** Standardized build and deployment pipeline using CMake and Docker Compose.

---

## Database Schema

The system uses a relational PostgreSQL schema to enforce data integrity across user authentication and rental tracking.

```mermaid
erDiagram
    USERS ||--o{ RENTALS : "places"
    USERS ||--o{ PURCHASES : "makes"
    USERS {
        int id PK
        string username
        string pin
        string role
    }
    MOVIES {
        int id PK
        string title
        string genre
        float rating
        int duration
        int rent_cost
        int purchase_cost
    }
    TV_SHOWS {
        int id PK
        string title
        int seasons
        int episodes_per_season
        int rent_per_season
    }
    RENTALS {
        int id PK
        int user_id FK
        int content_id
        string content_type
        date start_date
        date due_date
        int cost
    }
```

---

## Getting Started

### Prerequisites
- Docker Desktop or Docker Engine + Docker Compose

### Build and Run

1. **Clone the repository:**
   ```bash
   git clone https://github.com/CODER-7777/Netflix_Inventory_API.git
   cd Netflix_Inventory_API
   ```

2. **Boot the cluster:**
   ```bash
   docker compose up --build
   ```
   *This command automatically initializes the PostgreSQL database with the required schema and compiles the C++ REST API.*

3. **Access the API:**
   The API will be live at `http://localhost:8080`.

---

## API Endpoints

### Authentication
- `POST /api/register` - Register a new user
- `POST /api/login` - Authenticate and retrieve user context

### Catalog
- `GET /api/movies` - Retrieve the entire movie catalog
- `POST /api/movies` - (Admin) Add a new movie to the catalog

### Transactions
- `POST /api/rentals` - Rent a movie or TV show

*(Further endpoints for TV Shows, Purchases, and User specifics are defined in the extensible `main.cpp` controller.)*
