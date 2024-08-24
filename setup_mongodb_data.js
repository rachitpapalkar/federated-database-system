// Connect to MongoDB and select the ecommerce database
const db = connect("mongodb://localhost:27017/ecommerce");

// Drop existing collections if they exist
db.products.drop();
db.reviews.drop();

// Create products collection with schema validation
db.createCollection("products", {
    validator: {
        $jsonSchema: {
            bsonType: "object",
            required: ["name", "price", "sellers"],
            properties: {
                name: {
                    bsonType: "string",
                    description: "Name of the product"
                },
                description: {
                    bsonType: "string",
                    description: "Description of the product"
                },
                price: {
                    bsonType: "double",
                    description: "Base price of the product"
                },
                category: {
                    bsonType: "string",
                    description: "Category of the product"
                },
                sellers: {
                    bsonType: "array",
                    items: {
                        bsonType: "object",
                        required: ["seller_id", "price", "stock"],
                        properties: {
                            seller_id: {
                                bsonType: "objectId",
                                description: "Reference to the seller"
                            },
                            price: {
                                bsonType: "double",
                                description: "Price of the product from this seller"
                            },
                            stock: {
                                bsonType: "int",
                                description: "Quantity in stock from this seller"
                            }
                        }
                    },
                    description: "Array of sellers with their specific prices and stock"
                }
            }
        }
    }
});

// Create reviews collection with schema validation
db.createCollection("reviews", {
    validator: {
        $jsonSchema: {
            bsonType: "object",
            required: ["product_id", "rating", "review_date"],
            properties: {
                product_id: {
                    bsonType: "objectId",
                    description: "Reference to the product"
                },
                user_id: {
                    bsonType: "objectId",
                    description: "Reference to the user (optional)"
                },
                rating: {
                    bsonType: "int",
                    description: "Rating given in the review",
                    minimum: 1,
                    maximum: 5
                },
                review_text: {
                    bsonType: "string",
                    description: "Text of the review"
                },
                review_date: {
                    bsonType: "date",
                    description: "Date when the review was made"
                }
            }
        }
    }
});

// Insert sample data into the products collection
db.products.insertMany([
    {
        name: "iPhone 12",
        description: "The latest iPhone model",
        price: 799.99,
        category: "Electronics",
        sellers: [
            {
                seller_id: ObjectId(), // Replace with specific ObjectId if needed
                price: 799.99,
                stock: 10
            },
            {
                seller_id: ObjectId(), // Replace with specific ObjectId if needed
                price: 789.99,
                stock: 5
            }
        ]
    },
    {
        name: "Samsung Galaxy S21",
        description: "The latest Samsung Galaxy model",
        price: 699.99,
        category: "Electronics",
        sellers: [
            {
                seller_id: ObjectId(), // Replace with specific ObjectId if needed
                price: 699.99,
                stock: 8
            },
            {
                seller_id: ObjectId(), // Replace with specific ObjectId if needed
                price: 689.99,
                stock: 3
            }
        ]
    }
]);

// Insert sample data into the reviews collection
db.reviews.insertMany([
    {
        product_id: ObjectId(), // Replace with specific ObjectId from the products collection
        user_id: ObjectId(), // Replace with specific ObjectId if needed
        rating: 5,
        review_text: "Great phone!",
        review_date: new Date()
    },
    {
        product_id: ObjectId(), // Replace with specific ObjectId from the products collection
        user_id: ObjectId(), // Replace with specific ObjectId if needed
        rating: 4,
        review_text: "Good phone, but battery life could be better",
        review_date: new Date()
    }
]);
