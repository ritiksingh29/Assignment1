CREATE TABLE rules (
    id INT PRIMARY KEY AUTO_INCREMENT,
    rule_string VARCHAR(255) NOT NULL,
    ast_structure TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE user_attributes (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    age INT,
    department VARCHAR(50),
    salary DECIMAL(10, 2),
    experience INT
);


-- inserting into databases
INSERT INTO rules (rule_string, ast_structure) 
VALUES ("((age > 30 AND department = 'Sales') OR (age < 25 AND department = 'Marketing')) AND (salary > 50000 OR experience > 5)", 'AST JSON structure');


-- retrieving database
SELECT rule_string FROM rules WHERE id = 1;
