#ifndef CONFIG_H
#define CONFIG_H
#define POSTGRESQL_HOST "localhost"
#define POSTGRESQL_PORT 5432
#define POSTGRESQL_DBNAME "ecommerce"
#define POSTGRESQL_USER "rachit"
#define POSTGRESQL_PASSWORD "rachit"

// MongoDB Configuration
#define MONGODB_URI "mongodb://localhost:27017"
#define MONGODB_DBNAME "ecommerce"

// PostgreSQL Schema Definitions
#define POSTGRESQL_SCHEMA_PRODUCTS \
"CREATE TABLE IF NOT EXISTS products (" \
"id SERIAL PRIMARY KEY, " \
"name VARCHAR(255) NOT NULL, " \
"description TEXT, " \
"price NUMERIC(10, 2) NOT NULL, " \
"category VARCHAR(255)" \
");"

#define POSTGRESQL_SCHEMA_SELLERS \
"CREATE TABLE IF NOT EXISTS sellers (" \
"id SERIAL PRIMARY KEY, " \
"name VARCHAR(255) NOT NULL, " \
"contact_info TEXT" \
");"

#define POSTGRESQL_SCHEMA_PRODUCT_SELLERS \
"CREATE TABLE IF NOT EXISTS product_sellers (" \
"id SERIAL PRIMARY KEY, " \
"product_id INTEGER REFERENCES products(id), " \
"seller_id INTEGER REFERENCES sellers(id), " \
"price NUMERIC(10, 2) NOT NULL, " \
"stock INTEGER NOT NULL" \
");"

#define POSTGRESQL_SCHEMA_REVIEWS \
"CREATE TABLE IF NOT EXISTS reviews (" \
"id SERIAL PRIMARY KEY, " \
"product_id INTEGER REFERENCES products(id), " \
"user_id INTEGER, " \
"rating INTEGER CHECK (rating >= 1 AND rating <= 5), " \
"review_text TEXT, " \
"review_date TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP" \
");"

// MongoDB Schema Definitions
#define MONGODB_SCHEMA_PRODUCTS \
"{ " \
"  \"bsonType\": \"object\", " \
"  \"required\": [\"name\", \"price\", \"sellers\"], " \
"  \"properties\": { " \
"    \"name\": { " \
"      \"bsonType\": \"string\", " \
"      \"description\": \"Name of the product\" " \
"    }, " \
"    \"description\": { " \
"      \"bsonType\": \"string\", " \
"      \"description\": \"Description of the product\" " \
"    }, " \
"    \"price\": { " \
"      \"bsonType\": \"double\", " \
"      \"description\": \"Base price of the product\" " \
"    }, " \
"    \"category\": { " \
"      \"bsonType\": \"string\", " \
"      \"description\": \"Category of the product\" " \
"    }, " \
"    \"sellers\": { " \
"      \"bsonType\": \"array\", " \
"      \"items\": { " \
"        \"bsonType\": \"object\", " \
"        \"required\": [\"seller_id\", \"price\", \"stock\"], " \
"        \"properties\": { " \
"          \"seller_id\": { " \
"            \"bsonType\": \"objectId\", " \
"            \"description\": \"Reference to the seller\" " \
"          }, " \
"          \"price\": { " \
"            \"bsonType\": \"double\", " \
"            \"description\": \"Price of the product from this seller\" " \
"          }, " \
"          \"stock\": { " \
"            \"bsonType\": \"int\", " \
"            \"description\": \"Quantity in stock from this seller\" " \
"          } " \
"        } " \
"      }, " \
"      \"description\": \"Array of sellers with their specific prices and stock\" " \
"    } " \
"  } " \
"}"

#define MONGODB_SCHEMA_REVIEWS \
"{ " \
"  \"bsonType\": \"object\", " \
"  \"required\": [\"product_id\", \"rating\", \"review_date\"], " \
"  \"properties\": { " \
"    \"product_id\": { " \
"      \"bsonType\": \"objectId\", " \
"      \"description\": \"Reference to the product\" " \
"    }, " \
"    \"user_id\": { " \
"      \"bsonType\": \"objectId\", " \
"      \"description\": \"Reference to the user (optional)\" " \
"    }, " \
"    \"rating\": { " \
"      \"bsonType\": \"int\", " \
"      \"description\": \"Rating given in the review\", " \
"      \"minimum\": 1, " \
"      \"maximum\": 5 " \
"    }, " \
"    \"review_text\": { " \
"      \"bsonType\": \"string\", " \
"      \"description\": \"Text of the review\" " \
"    }, " \
"    \"review_date\": { " \
"      \"bsonType\": \"date\", " \
"      \"description\": \"Date when the review was made\" " \
"    } " \
"  } " \
"}"

// Function prototypes
void load_config();
void print_config();

#endif // CONFIG_H
