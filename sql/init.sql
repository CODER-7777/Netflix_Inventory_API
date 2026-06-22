CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    pin VARCHAR(255) NOT NULL,
    role VARCHAR(20) DEFAULT 'user'
);

CREATE TABLE movies (
    id SERIAL PRIMARY KEY,
    title VARCHAR(100) NOT NULL,
    genre VARCHAR(50),
    rating NUMERIC(3, 1),
    duration INT,
    rent_cost INT,
    purchase_cost INT
);

CREATE TABLE tv_shows (
    id SERIAL PRIMARY KEY,
    title VARCHAR(100) NOT NULL,
    genre VARCHAR(50),
    rating NUMERIC(3, 1),
    seasons INT,
    episodes_per_season INT,
    rent_per_season INT,
    purchase_per_season INT
);

CREATE TABLE rentals (
    id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(id),
    content_id INT,
    content_type VARCHAR(20), -- 'MOVIE' or 'TVSHOW'
    start_date DATE NOT NULL,
    due_date DATE NOT NULL,
    cost INT
);

CREATE TABLE purchases (
    id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(id),
    content_id INT,
    content_type VARCHAR(20),
    cost INT
);

-- Insert admin user (pin: adminpass)
INSERT INTO users (username, pin, role) VALUES ('admin', 'adminpass', 'admin');
