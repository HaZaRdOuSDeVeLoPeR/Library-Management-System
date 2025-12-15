#pragma once
#include "headers.hpp"

class Helper{
    private:
    friend class Students;
    friend class Books;
    friend class Library;

    static bool check_number(std::string&);
    static bool process_brackets(std::string&);
    static bool process_attributes(std::string&, std::vector<std::string>&, char, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&, int, int, bool);
    static bool validate_date(std::string&);
    static void reverse_date(std::string&);
};

// struct Student{
//     int studentID;
//     string name;
//     string branch;
//     int year;
class Students{
    private:
    std::string file_name;

    protected:
    std::vector<std::string> attributes={"StudentID","Name","Branch","Year"};
    std::vector<std::string> type={"Int","String","String","Int"};
    std::vector<std::string> typeInfo={typeid(int).name(),typeid(std::string).name(),typeid(std::string).name(),typeid(int).name()};
    std::set<int> studentID;

    std::unordered_map<int, std::string> id_name;
    std::unordered_map<int, std::string> id_branch;
    std::unordered_map<int, int> id_year;

    std::unordered_map<std::string, std::unordered_set<int>> name_id;
    std::unordered_map<std::string, std::unordered_set<int>> branch_id;
    std::unordered_map<int, std::unordered_set<int>> year_id;

    Students(std::string);
    void load_Table();
    void save_Table();
    void insert_Student();
    void delete_Students(std::unordered_set<int>&);
    void update_Students(std::unordered_set<int>&, std::string, std::string, int);
    void display_Students(std::unordered_set<int>&);
    std::unordered_set<int> select_Students(int, std::string, std::string, int, std::string);
};

// };
// struct Book{
//     int bookID;
//     string title;
//     string author;
//     string genre;
//     int available;
// };
class Books{
    private:
    std::string file_name;

    protected:
    std::vector<std::string> attributes={"BookID","Title","Author","Genre","Available"};
    std::vector<std::string> type={"Int","String","String","String","String"};
    std::vector<std::string> typeInfo={typeid(int).name(),typeid(std::string).name(),typeid(std::string).name(),typeid(std::string).name(),typeid(std::string).name()};
    std::set<int> bookID;

    std::unordered_map<int, std::string> id_title;
    std::unordered_map<int, std::string> id_author;
    std::unordered_map<int, std::string> id_genre;
    std::unordered_map<int, std::string> id_available;

    std::unordered_map<std::string, std::unordered_set<int>> title_id;
    std::unordered_map<std::string, std::unordered_set<int>> author_id;
    std::unordered_map<std::string, std::unordered_set<int>> genre_id;
    std::unordered_map<std::string, std::unordered_set<int>> available_id;

    Books(std::string);
    void load_Table();
    void save_Table();
    void insert_Book();
    void delete_Books(std::unordered_set<int>&);
    void update_Books(std::unordered_set<int>&, std::string, std::string, std::string);
    void display_Books(std::unordered_set<int>&);
    std::unordered_set<int> select_Books(int id, std::string, std::string, std::string, std::string, std::string);
};

// struct Issue{
//     int transactionID;
//     int studentID;
//     int bookID;
//     string borrow_date;
//     string return_date;
// };
class Library : protected Students, protected Books{
    private:
    std::string file_name;
    friend class DatabaseTools;

    protected:
    std::vector<std::string> attributes={"TransactionID","StudentID","BookID","Borrow_Date","Return_Date"};
    std::vector<std::string> type={"Int","Int","Int","String","String"};
    std::vector<std::string> typeInfo={typeid(int).name(),typeid(int).name(),typeid(int).name(),typeid(std::string).name(),typeid(std::string).name()};
    std::set<int> transactionID;

    std::unordered_map<int, std::pair<int,int>> id_studentID_bookID;
    std::unordered_map<int, std::string> id_borrow_date;
    std::unordered_map<int, std::string> id_return_date;

    std::unordered_map<int, std::unordered_set<int>> studentID_id;
    std::unordered_map<int, std::unordered_set<int>> bookID_id;
    std::unordered_map<std::string, std::unordered_set<int>> borrow_date_id;
    std::unordered_map<std::string, std::unordered_set<int>> return_date_id;

    Library(std::string, std::string booksfile, std::string issuesfile);
    void load_Table();
    void save_Table();
    void insert_Record(int, int, int, std::string);
    void update_Record(int, int, int, std::string);
    void update_Students();
    void update_Books();
    void search_Students();
    void search_Books();
    void search_Records();
    void delete_Students();
    void delete_Books();
    void delete_Records(std::unordered_set<int>&);
    void delete_History(std::unordered_set<int>&, int);
    void Issue_Book();
    void Return_Book();
    void display_Records(std::unordered_set<int>&);
    std::unordered_set<int> select_Records(int, int, int, std::string, std::string, std::string);
};

class DatabaseTools{
    private:
    int choice;
    int state=0;
    std::vector<void(DatabaseTools::*)()> functions={&Insert, &Update, &Delete, &Search, &Issue, &Return, &SavePoint, &RollBack, &Commit};
    Library *Libraries[100]={nullptr};

    public:
    void NewDatabase(std::string, std::string, std::string);
    void Insert();
    void Update();
    void Delete();
    void Search();
    void Issue();
    void Return();
    void SavePoint();
    void RollBack();
    void Commit();
    void Menu();
};