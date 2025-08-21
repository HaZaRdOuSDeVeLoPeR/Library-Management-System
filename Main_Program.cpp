#include<bits/stdc++.h>
using namespace std;
class Helper{
    private:
    friend class Students;
    friend class Books;
    friend class Library;

    static bool check_number(string &number){
        for(int i=0;i<number.length();i++){
            if(!isdigit(number[i])) return false;
        }   
        return true;
    }

    static bool process_brackets(string &line){
        if(line[0]!='(' or line.back()!=')'){
            cout<<"Missing Brackets\n";
            return false;
        }
        line.erase(line.begin());
        line.pop_back();
        return true;
    }

    static bool process_attributes(string &line, vector<string> &processed, char seperator, vector<string> &typeInfo, vector<string> &attributes, vector<string> &type, int end, int offset, bool state){
        stringstream ss(line);
        string vals;

        int i=offset;
        while(i<end and getline(ss,vals,seperator)){
            if(vals[0]=='\'' and vals.back()=='\''){
                vals.erase(vals.begin());
                vals.pop_back();
                if(vals.length() and typeInfo[i]==typeid(string).name()) processed.push_back(vals);
                else{
                    cout<<attributes[i]<<" must be of type "<<type[i]<<"\n";
                    return false;
                }
            }
            else if(check_number(vals)){
                if(vals.length() and typeInfo[i]==typeid(int).name()) processed.push_back(vals);
                else{
                    cout<<attributes[i]<<" must be of type "<<type[i]<<"\n";
                    return false;
                }
            }
            else{
                cout<<attributes[i]<<" must be of type "<<type[i]<<"\n";
                return false;
            }
            i++;
        }

        if(state and getline(ss,vals,seperator)){
            if(vals=="0" or vals=="AND" or vals=="OR" or vals=="ON" or vals=="BEFORE" or "AFTER") processed.push_back(vals);
        }
        else if(state){
            cout<<"Missing operator\n";
            return false;
        }

        if(processed.size()>=end-offset) return true;
        else{
            cout<<"Missing Attributes\n";
            return false;
        }
    }

    static bool validate_date(string &date){
        if((isdigit(date[0]) and isdigit(date[1]) and date[2]=='-' and isdigit(date[3]) and isdigit(date[4]) and date[5]=='-' and isdigit(date[6]) and isdigit(date[7]) and isdigit(date[8]) and isdigit(date[9])) or date=="Overdue") return true;
        else{
            cout<<"Wrong Date\n";
            return false;
        }
    }
    
    static void reverse_date(string &date){
        if(date=="Overdue" or date=="0") return;
        stringstream ss(date);
        string day,month,year;
        getline(ss,day,'-'); getline(ss,month,'-'); getline(ss,year,'-');
        date=year+'-'+month+'-'+day;
    }
};
// struct Student{
//     int studentID;
//     string name;
//     string branch;
//     int year;
class Students{
    private:
    string file_name;

    protected:
    vector<string> attributes={"StudentID","Name","Branch","Year"};
    vector<string> type={"Int","String","String","Int"};
    vector<string> typeInfo={typeid(int).name(),typeid(string).name(),typeid(string).name(),typeid(int).name()};
    set<int> studentID;

    unordered_map<int,string> id_name;
    unordered_map<int,string> id_branch;
    unordered_map<int,int> id_year;

    unordered_map<string,unordered_set<int>> name_id;
    unordered_map<string,unordered_set<int>> branch_id;
    unordered_map<int,unordered_set<int>> year_id;

    Students(string file){
        file_name=file;
    }

    void load_Table(){
        ifstream input(file_name);

        if(!input.is_open()) {
            cout<<"Failed to Open Students table.\n";
            exit(0);
        }

        string line;
        getline(input, line);  // Skip Header

        while(getline(input, line)){
            stringstream ss(line);
            string field;
            vector<string> row;

            // Split line by comma
            while (getline(ss, field, ',')) {
                row.push_back(field);
            }

            int id=stoi(row[0]), year=stoi(row[3]);         // getting column values
            string name=row[1], branch=row[2];

            if(studentID.find(id)==studentID.end()){
                studentID.insert(id);
                id_name[id]=name;
                id_branch[id]=branch;
                id_year[id]=year;
                
                name_id[name].insert(id);
                branch_id[branch].insert(id);
                year_id[year].insert(id);
            }
        }

    }

    void save_Table(){
        ofstream output(file_name);

        if (!output.is_open()) {
            cout<<"Failed to Save Students table.\n";
            exit(0);
        }

        output<<"StudentID,Name,Branch,Year\n";
        for(auto &x:studentID){
            string row="";
            row+=to_string(x); row+=','; row+=id_name[x]; row+=','; row+=id_branch[x]; row+=','; row+=to_string(id_year[x]); row+='\n';
            output<<row;
        }

        output.close();
    }

    void insert_Student(){
        string line;
        cout<<"Enter (StudentID,Name,Branch,Year) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',typeInfo,attributes,type,4,0,false));
        else return;

        int id=stoi(input[0]),year=stoi(input[3]);
        string name=input[1], branch=input[2];
        if(studentID.find(id)==studentID.end()){
            studentID.insert(id);
            id_name[id]=name;
            id_branch[id]=branch;
            id_year[id]=year;

            name_id[name].insert(id);
            branch_id[branch].insert(id);
            year_id[year].insert(id);
            cout<<"Student added Successfully\n";
        }
        else{
            cout<<"Student with duplicate IDs is not allowed."<<"\n";
        }
    }

    void delete_Students(unordered_set<int> &selected){
        for(auto &x:selected){
            studentID.erase(x);

            string name=id_name[x], branch=id_branch[x];
            int year=id_year[x];

            id_name.erase(x);
            id_branch.erase(x);
            id_year.erase(x);

            name_id[name].erase(x);
            if(name_id[name].size()==0) name_id.erase(name);

            branch_id[branch].erase(x);
            if(branch_id[branch].size()==0) branch_id.erase(branch);

            year_id[year].erase(x);
            if(year_id[year].size()==0) year_id.erase(year);
        }
    }

    void update_Students(unordered_set<int> &selected, string name, string branch, int year){
        for(auto &sid:selected){
            if(name!="0"){
                name_id[id_name[sid]].erase(sid);
                if(name_id[id_name[sid]].size()==0) name_id.erase(id_name[sid]);
                id_name[sid]=name;
                name_id[name].insert(sid);
            }
            if(branch!="0"){
                branch_id[id_branch[sid]].erase(sid);
                if(branch_id[id_branch[sid]].size()==0) branch_id.erase(id_branch[sid]);
                id_branch[sid]=branch;
                branch_id[branch].insert(sid);
            }
            if(year!=0){
                year_id[id_year[sid]].erase(sid);
                if(year_id[id_year[sid]].size()==0) year_id.erase(id_year[sid]);
                id_year[sid]=year;
                year_id[year].insert(sid);
            }
        }
    }

    void display_Students(unordered_set<int> &selected){
        cout<<setw(12)<<"StudentID"<<setw(20)<<"Name"<<setw(10)<<"Branch"<<setw(8)<<"Year"<<"\n";
        for(auto &x:selected){
            cout<<setw(12)<<x<<setw(20)<<id_name[x]<<setw(10)<<id_branch[x]<<setw(8)<<id_year[x]<<"\n";
        }
        cout<<"\nFetched "<<selected.size()<<" rows Successfully\n";
    }

    unordered_set<int> select_Students(int id, string name, string branch, int year, string optr){
        unordered_map<int,int> selected_id_freq;
        unordered_set<int> selected;

        int attributes=0;

        if(id!=0){
            if(studentID.find(id)!=studentID.end()) selected_id_freq[id]++;
            attributes++;
        }
        if(name!="0"){
            for(auto &x:name_id[name]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        if(branch!="0"){
            for(auto &x:branch_id[branch]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        if(year!=0){
            for(auto &x:year_id[year]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        
        if(optr=="AND"){
            for(auto &x:selected_id_freq){
                if(x.second==attributes){
                    selected.insert(x.first);
                }
            }
        }
        else if(optr=="OR"){
            for(auto &x:selected_id_freq){
                selected.insert(x.first);
            }
        }
        else if(attributes==1 and optr=="0"){
            for(auto &x:selected_id_freq){
                selected.insert(x.first);
            }
        }
        else{
            cout<<"Missing Operator\n";
            return {};
        }

        return selected;
    }
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
    string file_name;

    protected:
    vector<string> attributes={"BookID","Title","Author","Genre","Available"};
    vector<string> type={"Int","String","String","String","String"};
    vector<string> typeInfo={typeid(int).name(),typeid(string).name(),typeid(string).name(),typeid(string).name(),typeid(string).name()};
    set<int> bookID;

    unordered_map<int,string> id_title;
    unordered_map<int,string> id_author;
    unordered_map<int,string> id_genre;
    unordered_map<int,string> id_available;

    unordered_map<string,unordered_set<int>> title_id;
    unordered_map<string,unordered_set<int>> author_id;
    unordered_map<string,unordered_set<int>> genre_id;
    unordered_map<string,unordered_set<int>> available_id;

    Books(string file){
        file_name=file;
    }

    void load_Table(){
        ifstream input(file_name);

        if (!input.is_open()) {
            cout<<"Failed to Open Books table.\n";
            exit(0);
        }

        string line;
        getline(input, line);  // Skip Header

        while(getline(input, line)){
            stringstream ss(line);
            string field;
            vector<string> row;

            // Split line by comma
            while (getline(ss, field, ',')) {
                row.push_back(field);
            }
            int id=stoi(row[0]);
            string title=row[1], author=row[2], genre=row[3], available=row[4];    // getting column values
            if(available=="Yes" or available=="No");
            else{
                cout<<"Availability column can accept only Yes/No\n";
                exit(0);
            }

            if(bookID.find(id)==bookID.end()){
                bookID.insert(id);
                id_title[id]=title;
                id_author[id]=author;
                id_genre[id]=genre;
                id_available[id]=available;
                
                title_id[title].insert(id);
                author_id[author].insert(id);
                genre_id[genre].insert(id);
                available_id[available].insert(id);
            }
        }

    }

    void save_Table(){
        ofstream output(file_name);

        if (!output.is_open()) {
            cout<<"Failed to Save Books table.\n";
            exit(0);
        }

        output<<"BookID,Title,Author,Genre,Available\n";
        for(auto &x:bookID){
            string row="";
            row+=to_string(x); row+=','; row+=id_title[x]; row+=','; row+=id_author[x]; row+=','; row+=id_genre[x]; row+=',';row+=id_available[x]; row+='\n';
            output<<row;
        }

        output.close();
    }

    void insert_Book(){
        string line;
        cout<<"Enter (BookID, Title, Author, Genre) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',typeInfo,attributes,type,4,0,false));
        else return;

        int id=stoi(input[0]);
        string title=input[1], author=input[2], genre=input[3], available="Yes";
        if(bookID.find(id)==bookID.end()){
            bookID.insert(id);
            id_title[id]=title;
            id_author[id]=author;
            id_genre[id]=genre;
            id_available[id]="Yes";

            title_id[title].insert(id);
            author_id[author].insert(id);
            genre_id[genre].insert(id);
            available_id[available].insert(id);
            cout<<"Book added Successfully\n";
        }
        else{
            cout<<"Book with duplicate IDs is not allowed."<<"\n";
        }
    }

    void delete_Books(unordered_set<int> &selected){
        for(auto &x:selected){
            bookID.erase(x);

            string title=id_title[x], author=id_author[x], genre=id_genre[x], available=id_available[x];

            id_title.erase(x);
            id_author.erase(x);
            id_genre.erase(x);
            id_available.erase(x);

            title_id[title].erase(x);
            if(title_id[title].size()==0) title_id.erase(title);

            author_id[author].erase(x);
            if(author_id[author].size()==0) author_id.erase(author);

            genre_id[genre].erase(x);
            if(genre_id[genre].size()==0) genre_id.erase(genre);

            available_id[available].erase(x);
            if(available_id[available].size()==0) available_id.erase(available);
        }
    }
    
    void update_Books(unordered_set<int> &selected, string title, string author, string genre){
        for(auto &bid:selected){
            if(title!="0"){
                title_id[id_title[bid]].erase(bid);
                if(title_id[id_title[bid]].size()==0) title_id.erase(id_title[bid]);
                id_title[bid]=title;
                title_id[title].insert(bid);
            }
            if(author!="0"){
                author_id[id_author[bid]].erase(bid);
                if(author_id[id_author[bid]].size()==0) author_id.erase(id_author[bid]);
                id_author[bid]=author;
                author_id[author].insert(bid);
            }
            if(genre!="0"){
                genre_id[id_genre[bid]].erase(bid);
                if(genre_id[id_genre[bid]].size()==0) genre_id.erase(id_genre[bid]);
                id_genre[bid]=genre;
                genre_id[genre].insert(bid);
            }
        }
    }

    void display_Books(unordered_set<int> &selected){
        cout<<"\n";
        cout<<setw(6)<<"BookID"<<setw(80)<<"Title"<<setw(40)<<"Author"<<setw(10)<<"Genre"<<setw(12)<<"Available"<<"\n";
        for(auto &x:selected){
            cout<<setw(6)<<x<<setw(80)<<id_title[x]<<setw(40)<<id_author[x]<<setw(10)<<id_genre[x]<<setw(12)<<id_available[x]<<"\n";
        }
        cout<<"\nFetched "<<selected.size()<<" rows Successfully\n";
    }

    unordered_set<int> select_Books(int id, string title, string author, string genre, string available, string optr){
        unordered_map<int,int> selected_id_freq;
        unordered_set<int> selected;

        int attributes=0;

        if(id!=0){
            if(bookID.find(id)!=bookID.end()) selected_id_freq[id]++;
            attributes++;
        }
        if(title!="0"){
            for(auto &x:title_id[title]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        if(author!="0"){
            for(auto &x:author_id[author]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        if(genre!="0"){
            for(auto &x:genre_id[genre]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        if(available!="0"){
            for(auto &x:available_id[available]){
                selected_id_freq[x]++;
            }
            attributes++;
        }
        
        if(optr=="AND"){
            for(auto &x:selected_id_freq){
                if(x.second==attributes){
                    selected.insert(x.first);
                }
            }
        }
        else if(optr=="OR"){
            for(auto &x:selected_id_freq){
                selected.insert(x.first);
            }
        }
        else if(attributes==1 and optr=="0"){
            for(auto &x:selected_id_freq){
                selected.insert(x.first);
            }
        }
        else{
            cout<<"Missing Operator\n";
            return {};
        }

        return selected;
    }
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
    string file_name;
    friend class DatabaseTools;

    protected:
    vector<string> attributes={"TransactionID","StudentID","BookID","Borrow_Date","Return_Date"};
    vector<string> type={"Int","Int","Int","String","String"};
    vector<string> typeInfo={typeid(int).name(),typeid(int).name(),typeid(int).name(),typeid(string).name(),typeid(string).name()};
    set<int> transactionID;

    unordered_map<int,pair<int,int>> id_studentID_bookID;
    unordered_map<int,string> id_borrow_date;
    unordered_map<int,string> id_return_date;

    unordered_map<int,unordered_set<int>> studentID_id;
    unordered_map<int,unordered_set<int>> bookID_id;
    unordered_map<string,unordered_set<int>> borrow_date_id;
    unordered_map<string,unordered_set<int>> return_date_id;

    Library(string studentsfile, string booksfile, string issuesfile) : Students(studentsfile), Books(booksfile){
        file_name=issuesfile;
        Students::load_Table();
        Books::load_Table();
        Library::load_Table();
    }

    void load_Table(){
        ifstream input(file_name);

        if (!input.is_open()) {
            cout<<"Failed to Open Issues table.\n";
            exit(0);
        }

        string line;
        getline(input, line);  // Skip Header

        while(getline(input, line)){
            stringstream ss(line);
            string field;
            vector<string> row;
            // Split line by comma
            while (getline(ss, field, ',')) {
                row.push_back(field);
            }

            if(Helper::validate_date(row[3]) and Helper::validate_date(row[4]));
            else return;

            int id=stoi(row[0]), sid=stoi(row[1]), bid=stoi(row[2]);
            Helper::reverse_date(row[3]);
            Helper::reverse_date(row[4]);
            string b_date=row[3], r_date=row[4];                         // getting column values

            if(transactionID.find(id)==transactionID.end() and bookID.find(bid)!=bookID.end() and studentID.find(sid)!=studentID.end()){
                transactionID.insert(id);
                id_studentID_bookID[id]={sid,bid};
                id_borrow_date[id]=b_date;
                id_return_date[id]=r_date;
                
                studentID_id[sid].insert(id);
                bookID_id[bid].insert(id);
                borrow_date_id[b_date].insert(id);
                return_date_id[r_date].insert(id);
            }
        }
    }

    void save_Table(){
        Students::save_Table();
        Books::save_Table();
        ofstream output(file_name);

        if (!output.is_open()) {
            cout<<"Failed to Save Issues table.\n";
            exit(0);
        }

        output<<"TransactionID,StudentID,BookID,Borrow_Date,Return_Date\n";
        for(auto &x:transactionID){
            string row="";
            row+=to_string(x); row+=','; row+=to_string(id_studentID_bookID[x].first); row+=','; row+=to_string(id_studentID_bookID[x].second); row+=',';

            Helper::reverse_date(id_borrow_date[x]);
            Helper::reverse_date(id_return_date[x]);

            row+=id_borrow_date[x]; row+=','; row+=id_return_date[x]; row+='\n';
            output<<row;
        }

        output.close();
    }

    void insert_Record(int tid, int sid, int bid, string bdate){
        if(transactionID.find(tid)==transactionID.end()){
            if(studentID.find(sid)!=studentID.end()){
                if(bookID.find(bid)!=bookID.end()){
                    if(id_available[bid]=="Yes"){
                        id_available[bid]="No";         // Modify Book Table
                        available_id["No"].insert(bid);
                        available_id["Yes"].erase(bid);
                        if(available_id["Yes"].size()==0) available_id.erase("Yes");

                        transactionID.insert(tid);
                        id_studentID_bookID[tid]={sid,bid};
                        id_borrow_date[tid]=bdate;
                        id_return_date[tid]="Overdue";

                        studentID_id[sid].insert(tid);
                        bookID_id[bid].insert(tid);
                        borrow_date_id[bdate].insert(tid);
                        return_date_id["Overdue"].insert(tid);

                        cout<<"Book Issued Successfully\n";
                    }
                    else cout<<"Book is not available to Issue\n";
                }
                else cout<<"Incorrect BookID\n";
            }
            else cout<<"Incorrect StudentID\n";
        }
        else cout<<"Duplicate TransactionIDs not Allowed\n";
    }

    void update_Record(int tid, int sid, int bid, string rdate){
        if(transactionID.find(tid)!=transactionID.end()){
            if(studentID.find(sid)!=studentID.end()){
                if(bookID.find(bid)!=bookID.end()){
                    if(id_available[bid]=="No"){
                        id_available[bid]="Yes";         // Modify Book Table
                        available_id["Yes"].insert(bid);
                        available_id["No"].erase(bid);
                        if(available_id["No"].size()==0) available_id.erase("No");

                        id_return_date[tid]=rdate;
                        return_date_id[rdate].insert(tid);
                        return_date_id["Overdue"].erase(tid);
                        if(return_date_id["Overdue"].size()) return_date_id.erase("Overdue");

                        cout<<"Book Returned Successfully\n";
                    }
                    else cout<<"Book is already in Library\n";
                }
                else cout<<"Incorrect BookID\n";
            }
            else cout<<"Incorrect StudentID\n";
        }
        else cout<<"Incorrect TransactionID\n";
    }

    void update_Students(){
        string line;
        cout<<"Enter the Update Condition or '0' if column is not applicable\n";
        cout<<"Update By (ID, Name, Branch, Year, AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,0,true));
        else return;

        int id=stoi(input[0]),year=stoi(input[3]);
        string name=input[1], branch=input[2], optr=input.back();

        unordered_set<int> selected=select_Students(id, name, branch, year, optr);
    
        cout<<"Enter the new Attributes or '0' if no update needed\n";
        cout<<"Enter in Order (Name, Branch, Year) : ";
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        input.clear();
        if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,1,false));
        else return;

        name=input[0], branch=input[1], year=stoi(input[2]);
        Students::update_Students(selected,name,branch,year);
        cout<<"Successfully Updated "<<selected.size()<<" Students\n";
    }

    void update_Books(){
        string line;
        cout<<"Enter the Update Condition or '0' if column is not applicable\n";
        cout<<"Update By (ID, Title, Author, Genre, AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,4,0,true));
        else return;

        string title=input[1], author=input[2], genre=input[3], optr=input.back();
        int id=stoi(input[0]);

        unordered_set<int> selected=select_Books(id,title,author,genre,"0",optr);
        cout<<"Enter the new Attributes or '0' if no update needed\n";
        cout<<"Enter in Order (Title, Author, Genre) : ";
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        input.clear();
        if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,4,1,false));
        else return;

        title=input[0], author=input[1], genre=input[2];
        Books::update_Books(selected,title,author,genre);
        cout<<"Successfully Updated "<<selected.size()<<" Books\n";
    }

    void search_Students(){
        string line;
        cout<<"Enter the Search Condition or '0' if column is not applicable\n";
        cout<<"Search By (ID, Name, Branch, Year, AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,0,true));
        else return;

        int id=stoi(input[0]),year=stoi(input[3]);
        string name=input[1],branch=input[2];
        string optr=input.back();

        unordered_set<int> selected=select_Students(id, name, branch, year, optr);
        display_Students(selected);
    }

    void search_Books(){
        string line;
        cout<<"Enter the Search Condition or '0' if column is not applicable\n";
        cout<<"Search By (ID, Title, Author, Genre, AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,4,0,true));
        else return;

        int id=stoi(input[0]);
        string title=input[1], author=input[2], genre=input[3], optr=input.back();

        unordered_set<int> selected=select_Books(id,title,author,genre,"0",optr);
        display_Books(selected);
    }

    void search_Records(){
        string line;
        cout<<"Enter the Search Condition or '0' if column is not applicable\n";
        cout<<"Search By (ID, StudentID, BookID, Borrow_Date, Return_Date, BEFORE/AFTER/ON/AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Library::typeInfo,Library::attributes,Library::type,5,0,true));
        else return;

        int tid=stoi(input[0]), sid=stoi(input[1]), bid=stoi(input[2]);
        string bdate=input[3], rdate=input[4], optr=input.back();

        if((bdate=="0" or Helper::validate_date(bdate)) and (rdate=="0" or Helper::validate_date(rdate)));
        else return;

        Helper::reverse_date(bdate);
        Helper::reverse_date(rdate);
        unordered_set<int> selected=select_Records(tid,sid,bid,bdate,rdate,optr);
        display_Records(selected);
    }

    void delete_Students(){
        string line;
        cout<<"Enter the Delete Condition or '0' if column is not applicable\n";
        cout<<"Delete By (ID, Name, Branch, Year, AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,0,true));
        else return;
        
        int id=stoi(input[0]),year=stoi(input[3]);
        string name=input[1],branch=input[2];
        string optr=input.back();

        unordered_set<int> selected=select_Students(id, name, branch, year, optr);
        unordered_set<int> final_selected;

        for(auto &sid: selected){
            if(studentID_id.find(sid)!=studentID_id.end()){       // if student has issued any book
                bool is_safe=true;
                for(auto &tid:studentID_id[sid]){
                    if(id_return_date[tid]=="Overdue"){
                        is_safe=false;
                        break;
                    }
                }
                if(is_safe) final_selected.insert(sid);
                else cout<<"Student with ID : "<<sid<<" can't be removed as it hasn't returned book(s) yet.\n";
            }
            else final_selected.insert(sid);
        }
        Students::delete_Students(final_selected);
        delete_History(final_selected,1);

        cout<<"Successfully Removed "<<final_selected.size()<<" Students\n";
    }

    void delete_Books(){
        string line;
        cout<<"Enter the Delete Condition or '0' if column is not applicable\n";
        cout<<"Delete By (ID, Title, Author, Genre, Available, AND/OR/0) : ";
        cin.ignore();
        getline(cin,line);
        
        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,5,0,true));
        else return;

        int id=stoi(input[0]);
        string title=input[1], author=input[2], genre=input[3], available=input[4], optr=input.back();

        unordered_set<int> selected=select_Books(id, title, author, genre, available, optr);
        unordered_set<int> final_selected;

        for(auto &bid: selected){
            if(bookID_id.find(bid)!=studentID_id.end()){       // if book has been issued at a point of time
                bool is_safe=true;
                for(auto &tid:bookID_id[bid]){
                    if(id_return_date[tid]=="Overdue"){
                        is_safe=false;
                        break;
                    }
                }
                if(is_safe) final_selected.insert(bid);
                else cout<<"Book with ID : "<<bid<<" can't be removed as it hasn't been returned yet.\n";
            }
            else final_selected.insert(bid);
        }
        Books::delete_Books(final_selected);
        delete_History(final_selected,2);

        cout<<"Successfully Removed "<<final_selected.size()<<" Books\n";
    }

    void delete_Records(unordered_set<int> &selected){
        for(auto &remove:selected){
            if(transactionID.find(remove)!=transactionID.end()){
                transactionID.erase(remove);
            }
            if(id_studentID_bookID.find(remove)!=id_studentID_bookID.end()){
                int sid=id_studentID_bookID[remove].first;
                int bid=id_studentID_bookID[remove].second;
                id_studentID_bookID.erase(remove);

                studentID_id[sid].erase(remove);
                if(studentID_id[sid].size()==0) studentID_id.erase(sid);

                bookID_id[bid].erase(remove);
                if(bookID_id[bid].size()==0) bookID_id.erase(bid);
            }
            if(id_borrow_date.find(remove)!=id_borrow_date.end()){
                string bdate=id_borrow_date[remove];
                id_borrow_date.erase(remove);

                borrow_date_id[bdate].erase(remove);
                if(borrow_date_id[bdate].size()==0) borrow_date_id.erase(bdate);
            }
            if(id_return_date.find(remove)!=id_return_date.end()){
                string rdate=id_return_date[remove];
                id_return_date.erase(remove);

                return_date_id[rdate].erase(remove);
                if(return_date_id[rdate].size()==0) return_date_id.erase(rdate);
            }
        }
    }

    void delete_History(unordered_set<int> &selected, int table){
        if(table==1){                       // remove history of selected students
            unordered_set<int> removed_tid;     // get all removed tids for currently selected sid
            for(auto &sid:selected){
                for(auto &tid:studentID_id[sid]){
                    removed_tid.insert(tid);
                }
            }
            delete_Records(removed_tid);
        }
        else{       // remove history of selected books
            unordered_set<int> removed_tid;     // get all removed tids for currently selected sid
            for(auto &bid:selected){
                for(auto &tid:bookID_id[bid]){
                    removed_tid.insert(tid);
                }
            }
            delete_Records(removed_tid);
        }
    }

    void Issue_Book(){
        string line;
        cout<<"Enter the (TransactionID,StudentID,BookID,BorrowDate) to Issue : ";
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Library::typeInfo,Library::attributes,Library::type,4,0,false));
        else return;

        int tid=stoi(input[0]), sid=stoi(input[1]), bid=stoi(input[2]);
        string bdate=input.back();

        if(Helper::validate_date(bdate));
        else return;

        Helper::reverse_date(bdate);
        insert_Record(tid,sid,bid,bdate);
    }

    void Return_Book(){
        string line;
        cout<<"Enter the (TransactionID,StudentID,BookID,ReturnDate) to Return : ";
        getline(cin,line);

        if(Helper::process_brackets(line));
        else return;

        vector<string> input;
        if(Helper::process_attributes(line,input,',',Library::typeInfo,Library::attributes,Library::type,4,0,false));
        else return;

        int tid=stoi(input[0]), sid=stoi(input[1]), bid=stoi(input[2]);
        string rdate=input.back();

        if(Helper::validate_date(rdate));
        else return;

        Helper::reverse_date(rdate);
        update_Record(tid,sid,bid,rdate);
    }

    void display_Records(unordered_set<int> &selected){
        cout<<"\n";
        cout<<setw(12)<<"TransactionID"<<setw(12)<<"StudentID"<<setw(20)<<"StudentName"<<setw(10)<<"BookID"<<setw(70)<<"BookName"<<setw(15)<<"BorrowDate"<<setw(15)<<"ReturnDate"<<"\n";
        for(auto &x:selected){
            cout<<setw(12)<<x<<setw(12)<<id_studentID_bookID[x].first<<setw(20)<<id_name[id_studentID_bookID[x].first]<<setw(10)
            <<id_studentID_bookID[x].second<<setw(70)<<id_title[id_studentID_bookID[x].second]<<setw(15)<<id_borrow_date[x]<<setw(15)<<id_return_date[x]<<"\n";
        }
        cout<<"\nFetched "<<selected.size()<<" rows Successfully\n";
    }

    unordered_set<int> select_Records(int tid, int sid, int bid, string bdate, string rdate, string optr){
        cout<<bdate<<" "<<rdate<<"\n";
        if(optr=="BEFORE" or optr=="AFTER" or optr=="ON"){       // if select operation is date
            unordered_set<int> selected;
            if(tid==0 and sid==0 and bid==0){       // other operands must be null
                if((bdate=="0" and rdate!="0") or (bdate!="0" and rdate!="0")){
                    if(optr=="BEFORE"){        // if operation is BEFORE
                        if(bdate!="0"){     // if operation is on bdate   
                            for(auto &borrow:borrow_date_id){
                                if(borrow.first<bdate){
                                    for(auto &tids:borrow.second){
                                        selected.insert(tids);
                                    }
                                }
                            }
                        }
                        else{               // if operation is on rdate
                            for(auto &back:return_date_id){
                                if(back.first<rdate and back.first!="Overdue"){
                                    for(auto &tids:back.second){
                                        selected.insert(tids);
                                    }
                                }
                            }
                        }
                    }
                    else if(optr=="AFTER"){       // if operation is AFTER
                        if(bdate!="0"){     // if operation is on bdate   
                            for(auto &borrow:borrow_date_id){
                                if(borrow.first>bdate){
                                    for(auto &tids:borrow.second){
                                        selected.insert(tids);
                                    }
                                }
                            }
                        }
                        else{               // if operation is on rdate
                            for(auto &back:return_date_id){
                                if(back.first>rdate or back.first=="Overdue"){
                                    cout<<back.first<<" "<<bdate<<"\n";
                                    for(auto &tids:back.second){
                                        selected.insert(tids);
                                    }
                                }
                            }
                        }
                    }
                    else{                           // if operation is ON
                        if(bdate!="0"){     // if operation is on bdate   
                            for(auto &borrow:borrow_date_id){
                                if(borrow.first==bdate){
                                    for(auto &tids:borrow.second){
                                        selected.insert(tids);
                                    }
                                }
                            }
                        }
                        else{               // if operation is on rdate
                            for(auto &back:return_date_id){
                                if(back.first==rdate and back.first!="Overdue"){
                                    for(auto &tids:back.second){
                                        selected.insert(tids);
                                    }
                                }
                            }
                        }
                    }

                }
                else{
                    cout<<"Operand Operator Mismatch\n";
                    return {};
                }
            }
            else{
                cout<<"Operand Operator Mismatch\n";
                return {};
            }
            return selected;
        }
        else{           // if select operation is on ids
            unordered_map<int,int> selected_id_freq;
            unordered_set<int> selected;
            int attributes=0;

            if(tid!=0){
                if(transactionID.find(tid)!=transactionID.end()) selected_id_freq[tid]++;
                attributes++;
            }
            if(sid!=0){
                for(auto &x:studentID_id[sid]){
                    selected_id_freq[x]++;
                }
                attributes++;
            }
            if(bid!=0){
                for(auto &x:bookID_id[bid]){
                    selected_id_freq[x]++;
                }
                attributes++;
            }
            
            if(optr=="AND"){
                for(auto &x:selected_id_freq){
                    if(x.second==attributes){
                        selected.insert(x.first);
                    }
                }
            }
            else if(optr=="OR"){
                for(auto &x:selected_id_freq){
                    selected.insert(x.first);
                }
            }
            else if(attributes==1 and optr=="0"){
                for(auto &x:selected_id_freq){
                    selected.insert(x.first);
                }
            }
            else{
                cout<<"Missing Operator\n";
                return {};
            }

            return selected;
        }
    }
};

class DatabaseTools{
    private:
    int choice;
    int state=0;
    vector<void(DatabaseTools::*)()> functions={&Insert,&Update,&Delete,&Search,&Issue,&Return,&SavePoint,&RollBack,&Commit};
    Library *Libraries[100]={nullptr};

    public:
    void NewDatabase(string student, string book, string issue){
        Libraries[state]=new Library(student,book,issue);
    }
    
    void Insert(){
        cout<<"Select the Table to Insert:\n1)Students\t2)Books\n";
        cin>>choice;
        if(choice==1) Libraries[state]->insert_Student();
        else if(choice==2) Libraries[state]->insert_Book();
    }
    
    void Update(){
        cout<<"Select the Table to Update:\n1)Students\t2)Books\n";
        cin>>choice;
        if(choice==1) Libraries[state]->update_Students();
        else if(choice==2) Libraries[state]->update_Books();
    }
    
    void Delete(){
        cout<<"Select the Table to Delete:\n1)Students\t2)Books\n";
        cin>>choice;
        if(choice==1) Libraries[state]->delete_Students();
        else if(choice==2) Libraries[state]->delete_Books();
    }
    
    void Search(){
        cout<<"Select the table to Search:\n1)Students\t2)Books\t\t3)Records\n";
        cin>>choice;
        if(choice==1) Libraries[state]->search_Students();
        else if(choice==2) Libraries[state]->search_Books();
        else if(choice==3) Libraries[state]->search_Records();
    }
    
    void Issue(){
        Libraries[state]->Issue_Book();
    }
    
    void Return(){
        Libraries[state]->Return_Book();
    }
    
    void SavePoint(){
        if(state<99){
            Libraries[state+1]=new Library(*Libraries[state]);      // make a copy of current state
            state++;                                                // mark the new copy as current state
            cout<<"Successfully Saved\n";
        }
        else cout<<"More than 100 SavePoints Not Allowed\n";
    }
    
    void RollBack(){
        if(state>0){
            delete(Libraries[state]);       // delete the current state
            Libraries[state--]=nullptr;     // move to previous state
            cout<<"Successfully Restored\n";
        }
        else cout<<"No SavePoints to RollBack\n";
    }
    
    void Commit(){
        swap(Libraries[0],Libraries[state]);
        for(int i=1;i<=state;i++){
            delete(Libraries[i]);
            Libraries[i]=nullptr;
        }
        state=0;
        Libraries[0]->save_Table();
        cout<<"Successfully Committed\n";
    }
    
    void Menu(){
        while(choice!=10){
            cout<<"\n\nMAIN MENU\n1) Insert\t2) Update\t3) Delete\t4) Search\n5) Issue\t6) Return\n7) SavePoint\t8) RollBack\t9) Commit\t10) Exit\n";
            cout<<"Choose the Transaction Type : ";
            cin>>choice;
            if(choice>0 and choice<10){
                cout<<"\n";
                (this->*functions[choice-1])();
            }
            else if(choice>10) cout<<"Invalid Choice\n";
        }
    }
};

int main(){
    DatabaseTools my_library;
    my_library.NewDatabase("Students.csv","Books.csv","Issues.csv");
    my_library.Menu();
}