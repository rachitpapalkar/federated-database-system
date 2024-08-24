-- Drop existing tables if they exist
DROP TABLE IF EXISTS reviews;
DROP TABLE IF EXISTS product_sellers;
DROP TABLE IF EXISTS sellers;
DROP TABLE IF EXISTS products;

-- Create the products table
CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    price NUMERIC NOT NULL,
    category VARCHAR(255)
);

-- Create the sellers table
CREATE TABLE sellers (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    contact_info TEXT
);

-- Create the product_sellers table (many-to-many relationship)
CREATE TABLE product_sellers (
    product_id INT REFERENCES products(id),
    seller_id INT REFERENCES sellers(id),
    price NUMERIC NOT NULL,
    stock INT NOT NULL,
    PRIMARY KEY (product_id, seller_id)
);

-- Create the reviews table
CREATE TABLE reviews (
    id SERIAL PRIMARY KEY,
    product_id INT REFERENCES products(id),
    user_id INT,
    rating INT CHECK (rating >= 1 AND rating <= 5),
    review_text TEXT,
    review_date DATE NOT NULL
);

-- Insert sample data into the products table
INSERT INTO products (name, description, price, category) VALUES
('Laptop', 'A high-performance laptop', 1200.00, 'Electronics'),
('Smartphone', 'A latest model smartphone', 800.00, 'Electronics');

-- Insert sample data into the sellers table
INSERT INTO sellers (name, contact_info) VALUES
('Seller One', 'Contact info for Seller One'),
('Seller Two', 'Contact info for Seller Two');

-- Insert sample data into the product_sellers table
INSERT INTO product_sellers (product_id, seller_id, price, stock) VALUES
(1, 1, 1150.00, 10),
(1, 2, 1180.00, 5),
(2, 1, 780.00, 20),
(2, 2, 790.00, 15);

-- Insert sample data into the reviews table
INSERT INTO reviews (product_id, user_id, rating, review_text, review_date) VALUES
(1, 1, 5, 'Excellent laptop!', '2023-01-15'),
(2, 2, 4, 'Very good smartphone', '2023-02-20');
