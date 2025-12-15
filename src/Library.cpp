#include "headers.hpp"
#include "structure.hpp"

Library::Library(std::string studentsfile, std::string booksfile, std::string issuesfile) : Students(studentsfile), Books(booksfile){
    file_name=issuesfile;
    Students::load_Table();
    Books::load_Table();
    Library::load_Table();
}

void Library::load_Table(){
    std::ifstream input(file_name);

    if (!input.is_open()) {
        std::cout<<"Failed to Open Issues table.\n";
        exit(0);
    }

    std::string line;
    std::getline(input, line);  // Skip Header

    while(std::getline(input, line)){
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> row;
        // Split line by comma
        while (std::getline(ss, field, ',')) {
            row.push_back(field);
        }

        if(Helper::validate_date(row[3]) and Helper::validate_date(row[4]));
        else return;

        int id=std::stoi(row[0]), sid=std::stoi(row[1]), bid=std::stoi(row[2]);
        Helper::reverse_date(row[3]);
        Helper::reverse_date(row[4]);
        std::string b_date=row[3], r_date=row[4];                         // getting column values

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

void Library::save_Table(){
    Students::save_Table();
    Books::save_Table();
    std::ofstream output(file_name);

    if (!output.is_open()) {
        std::cout<<"Failed to Save Issues table.\n";
        exit(0);
    }

    output<<"TransactionID,StudentID,BookID,Borrow_Date,Return_Date\n";
    for(auto &x:transactionID){
        std::string row="";
        row+=std::to_string(x); row+=','; row+=std::to_string(id_studentID_bookID[x].first); row+=','; row+=std::to_string(id_studentID_bookID[x].second); row+=',';

        Helper::reverse_date(id_borrow_date[x]);
        Helper::reverse_date(id_return_date[x]);

        row+=id_borrow_date[x]; row+=','; row+=id_return_date[x]; row+='\n';
        output<<row;
    }

    output.close();
}

void Library::insert_Record(int tid, int sid, int bid, std::string bdate){
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

                    std::cout<<"Book Issued Successfully\n";
                }
                else std::cout<<"Book is not available to Issue\n";
            }
            else std::cout<<"Incorrect BookID\n";
        }
        else std::cout<<"Incorrect StudentID\n";
    }
    else std::cout<<"Duplicate TransactionIDs not Allowed\n";
}

void Library::update_Record(int tid, int sid, int bid, std::string rdate){
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

                    std::cout<<"Book Returned Successfully\n";
                }
                else std::cout<<"Book is already in Library\n";
            }
            else std::cout<<"Incorrect BookID\n";
        }
        else std::cout<<"Incorrect StudentID\n";
    }
    else std::cout<<"Incorrect TransactionID\n";
}

void Library::update_Students(){
    std::string line;
    std::cout<<"Enter the Update Condition or '0' if column is not applicable\n";
    std::cout<<"Update By (ID, Name, Branch, Year, AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,0,true));
    else return;

    int id=std::stoi(input[0]),year=std::stoi(input[3]);
    std::string name=input[1], branch=input[2], optr=input.back();

    std::unordered_set<int> selected=select_Students(id, name, branch, year, optr);

    std::cout<<"Enter the new Attributes or '0' if no update needed\n";
    std::cout<<"Enter in Order (Name, Branch, Year) : ";
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    input.clear();
    if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,1,false));
    else return;

    name=input[0], branch=input[1], year=stoi(input[2]);
    Students::update_Students(selected,name,branch,year);
    std::cout<<"Successfully Updated "<<selected.size()<<" Students\n";
}

void Library::update_Books(){
    std::string line;
    std::cout<<"Enter the Update Condition or '0' if column is not applicable\n";
    std::cout<<"Update By (ID, Title, Author, Genre, AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,4,0,true));
    else return;

    std::string title=input[1], author=input[2], genre=input[3], optr=input.back();
    int id=std::stoi(input[0]);

    std::unordered_set<int> selected=select_Books(id,title,author,genre,"0",optr);
    std::cout<<"Enter the new Attributes or '0' if no update needed\n";
    std::cout<<"Enter in Order (Title, Author, Genre) : ";
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    input.clear();
    if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,4,1,false));
    else return;

    title=input[0], author=input[1], genre=input[2];
    Books::update_Books(selected,title,author,genre);
    std::cout<<"Successfully Updated "<<selected.size()<<" Books\n";
}

void Library::search_Students(){
    std::string line;
    std::cout<<"Enter the Search Condition or '0' if column is not applicable\n";
    std::cout<<"Search By (ID, Name, Branch, Year, AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,0,true));
    else return;

    int id=std::stoi(input[0]),year=std::stoi(input[3]);
    std::string name=input[1],branch=input[2];
    std::string optr=input.back();

    std::unordered_set<int> selected=select_Students(id, name, branch, year, optr);
    display_Students(selected);
}

void Library::search_Books(){
    std::string line;
    std::cout<<"Enter the Search Condition or '0' if column is not applicable\n";
    std::cout<<"Search By (ID, Title, Author, Genre, AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,4,0,true));
    else return;

    int id=std::stoi(input[0]);
    std::string title=input[1], author=input[2], genre=input[3], optr=input.back();

    std::unordered_set<int> selected=select_Books(id,title,author,genre,"0",optr);
    display_Books(selected);
}

void Library::search_Records(){
    std::string line;
    std::cout<<"Enter the Search Condition or '0' if column is not applicable\n";
    std::cout<<"Search By (ID, StudentID, BookID, Borrow_Date, Return_Date, BEFORE/AFTER/ON/AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Library::typeInfo,Library::attributes,Library::type,5,0,true));
    else return;

    int tid=std::stoi(input[0]), sid=stoi(input[1]), bid=stoi(input[2]);
    std::string bdate=input[3], rdate=input[4], optr=input.back();

    if((bdate=="0" or Helper::validate_date(bdate)) and (rdate=="0" or Helper::validate_date(rdate)));
    else return;

    Helper::reverse_date(bdate);
    Helper::reverse_date(rdate);
    std::unordered_set<int> selected=select_Records(tid,sid,bid,bdate,rdate,optr);
    display_Records(selected);
}

void Library::delete_Students(){
    std::string line;
    std::cout<<"Enter the Delete Condition or '0' if column is not applicable\n";
    std::cout<<"Delete By (ID, Name, Branch, Year, AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Students::typeInfo,Students::attributes,Students::type,4,0,true));
    else return;
    
    int id=std::stoi(input[0]),year=std::stoi(input[3]);
    std::string name=input[1],branch=input[2];
    std::string optr=input.back();

    std::unordered_set<int> selected=select_Students(id, name, branch, year, optr);
    std::unordered_set<int> final_selected;

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
            else std::cout<<"Student with ID : "<<sid<<" can't be removed as it hasn't returned book(s) yet.\n";
        }
        else final_selected.insert(sid);
    }
    Students::delete_Students(final_selected);
    delete_History(final_selected,1);

    std::cout<<"Successfully Removed "<<final_selected.size()<<" Students\n";
}

void Library::delete_Books(){
    std::string line;
    std::cout<<"Enter the Delete Condition or '0' if column is not applicable\n";
    std::cout<<"Delete By (ID, Title, Author, Genre, Available, AND/OR/0) : ";
    std::cin.ignore();
    std::getline(std::cin,line);
    
    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Books::typeInfo,Books::attributes,Books::type,5,0,true));
    else return;

    int id=std::stoi(input[0]);
    std::string title=input[1], author=input[2], genre=input[3], available=input[4], optr=input.back();

    std::unordered_set<int> selected=select_Books(id, title, author, genre, available, optr);
    std::unordered_set<int> final_selected;

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
            else std::cout<<"Book with ID : "<<bid<<" can't be removed as it hasn't been returned yet.\n";
        }
        else final_selected.insert(bid);
    }
    Books::delete_Books(final_selected);
    delete_History(final_selected,2);

    std::cout<<"Successfully Removed "<<final_selected.size()<<" Books\n";
}

void Library::delete_Records(std::unordered_set<int> &selected){
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
            std::string bdate=id_borrow_date[remove];
            id_borrow_date.erase(remove);

            borrow_date_id[bdate].erase(remove);
            if(borrow_date_id[bdate].size()==0) borrow_date_id.erase(bdate);
        }
        if(id_return_date.find(remove)!=id_return_date.end()){
            std::string rdate=id_return_date[remove];
            id_return_date.erase(remove);

            return_date_id[rdate].erase(remove);
            if(return_date_id[rdate].size()==0) return_date_id.erase(rdate);
        }
    }
}

void Library::delete_History(std::unordered_set<int> &selected, int table){
    if(table==1){                       // remove history of selected students
        std::unordered_set<int> removed_tid;     // get all removed tids for currently selected sid
        for(auto &sid:selected){
            for(auto &tid:studentID_id[sid]){
                removed_tid.insert(tid);
            }
        }
        delete_Records(removed_tid);
    }
    else{       // remove history of selected books
        std::unordered_set<int> removed_tid;     // get all removed tids for currently selected sid
        for(auto &bid:selected){
            for(auto &tid:bookID_id[bid]){
                removed_tid.insert(tid);
            }
        }
        delete_Records(removed_tid);
    }
}

void Library::Issue_Book(){
    std::string line;
    std::cout<<"Enter the (TransactionID,StudentID,BookID,BorrowDate) to Issue : ";
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Library::typeInfo,Library::attributes,Library::type,4,0,false));
    else return;

    int tid=std::stoi(input[0]), sid=std::stoi(input[1]), bid=std::stoi(input[2]);
    std::string bdate=input.back();

    if(Helper::validate_date(bdate));
    else return;

    Helper::reverse_date(bdate);
    insert_Record(tid,sid,bid,bdate);
}

void Library::Return_Book(){
    std::string line;
    std::cout<<"Enter the (TransactionID,StudentID,BookID,ReturnDate) to Return : ";
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',Library::typeInfo,Library::attributes,Library::type,4,0,false));
    else return;

    int tid=std::stoi(input[0]), sid=std::stoi(input[1]), bid=std::stoi(input[2]);
    std::string rdate=input.back();

    if(Helper::validate_date(rdate));
    else return;

    Helper::reverse_date(rdate);
    update_Record(tid,sid,bid,rdate);
}

void Library::display_Records(std::unordered_set<int> &selected){
    std::cout<<"\n";
    std::cout<<std::setw(12)<<"TransactionID"<<std::setw(12)<<"StudentID"<<std::setw(20)<<"StudentName"<<std::setw(10)<<"BookID"<<std::setw(70)<<"BookName"<<std::setw(15)<<"BorrowDate"<<std::setw(15)<<"ReturnDate"<<"\n";
    for(auto &x:selected){
        std::cout<<std::setw(12)<<x<<std::setw(12)<<id_studentID_bookID[x].first<<std::setw(20)<<id_name[id_studentID_bookID[x].first]<<std::setw(10)
        <<id_studentID_bookID[x].second<<std::setw(70)<<id_title[id_studentID_bookID[x].second]<<std::setw(15)<<id_borrow_date[x]<<std::setw(15)<<id_return_date[x]<<"\n";
    }
    std::cout<<"\nFetched "<<selected.size()<<" rows Successfully\n";
}

std::unordered_set<int> Library::select_Records(int tid, int sid, int bid, std::string bdate, std::string rdate, std::string optr){
    std::cout<<bdate<<" "<<rdate<<"\n";
    if(optr=="BEFORE" or optr=="AFTER" or optr=="ON"){       // if select operation is date
        std::unordered_set<int> selected;
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
                                std::cout<<back.first<<" "<<bdate<<"\n";
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
                std::cout<<"Operand Operator Mismatch\n";
                return {};
            }
        }
        else{
            std::cout<<"Operand Operator Mismatch\n";
            return {};
        }
        return selected;
    }
    else{           // if select operation is on ids
        std::unordered_map<int,int> selected_id_freq;
        std::unordered_set<int> selected;
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
            std::cout<<"Missing Operator\n";
            return {};
        }

        return selected;
    }
}