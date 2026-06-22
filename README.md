# Netflix Inventory API

A scalable C++20 REST API for managing a Netflix-style streaming catalog.

## Architecture & Design
- **REST Framework**: Crow (C++ Microframework)
- **Database**: PostgreSQL (via libpqxx)
- **Pattern**: Controller-Service-Repository pattern implementing SOLID principles.
- **Models**: Polymorphic, memory-safe domain model using `std::shared_ptr`.
- **Containerization**: Docker and Docker Compose for easy deployment.

## Running the API
```bash
docker compose up --build
```

The API will be available on `http://localhost:8080`.
