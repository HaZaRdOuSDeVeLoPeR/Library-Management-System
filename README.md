# 📚 Library Management System (C++)

A **menu-driven Library Management System** implemented in C++ that simulates a relational database using CSV files.  
It supports **CRUD operations**, **book issuing/returning**, and **transaction management** while maintaining **referential integrity** between Students, Books, and Issues tables.

---

## 🚀 Features
- **Three Tables (CSV Files)**
  - **Students:** `StudentID`, `Name`, `Branch`, `Year`
  - **Books:** `BookID`, `Title`, `Author`, `Genre`, `Available`
  - **Issues:** `TransactionID`, `StudentID`, `BookID`, `Borrow_Date`, `Return_Date`
- **Operations**
  - Insert, Update, Delete, Search (with filters and `AND`/`OR` logic)
  - Issue & Return books (automatically updates availability)
  - Prevent deletion if data is linked (referential integrity)
- **Transaction Control**
  - Savepoint
  - Rollback
  - Commit (writes changes to CSV)
- **Date Validation & Comparison**
  - Supports `BEFORE`, `AFTER`, and `ON` for search queries
- **Fast Lookups**
  - Uses `unordered_map` & `unordered_set` for efficient O(1) operations

---

## 📂 File Structure
│── Main_Program.cpp # Main C++ code
│── Students.csv # Sample student records
│── Books.csv # Sample book records
│── Issues.csv # Sample issue records
│── README.md # Project documentation

---

## 🛠️ Tech Stack
- **Language:** C++17
- **Data Storage:** CSV files
- **Concepts Used:**
  - Object-Oriented Programming (Encapsulation, Inheritance)
  - STL Containers (`unordered_map`, `unordered_set`, `vector`)
  - File I/O (`ifstream`, `ofstream`)
  - String parsing (`stringstream`)
  - Data validation
  - Manual referential integrity checks
  - Transaction simulation

---

## 📌 How to Run
### Compile:
```bash
g++ Main_Program.cpp -o library

---

🧩 Example CSV Data
Students.csv

StudentID,Name,Branch,Year
1,John Doe,CSE,2
2,Jane Smith,EEE,3

---

Books.csv

BookID,Title,Author,Genre,Available
101,Data Structures,Mark Allen,CS,Yes
102,Physics Fundamentals,David Hall,Physics,No
Issues.csv

---

Issues.csv

TransactionID,StudentID,BookID,Borrow_Date,Return_Date
78519,134,109,12-02-2025,21-05-2025
10106,12,104,15-09-2024,Overdue
10364,18,106,06-08-2024,Overdue
