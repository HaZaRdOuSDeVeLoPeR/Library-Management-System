#include "headers.hpp"
#include "structure.hpp"

void DatabaseTools::NewDatabase(std::string student, std::string book, std::string issue){
    Libraries[state]=new Library(student,book,issue);
}

void DatabaseTools::Insert(){
    std::cout<<"Select the Table to Insert:\n1)Students\t2)Books\n";
    std::cin>>choice;
    if(choice==1) Libraries[state]->insert_Student();
    else if(choice==2) Libraries[state]->insert_Book();
}

void DatabaseTools::Update(){
    std::cout<<"Select the Table to Update:\n1)Students\t2)Books\n";
    std::cin>>choice;
    if(choice==1) Libraries[state]->update_Students();
    else if(choice==2) Libraries[state]->update_Books();
}

void DatabaseTools::Delete(){
    std::cout<<"Select the Table to Delete:\n1)Students\t2)Books\n";
    std::cin>>choice;
    if(choice==1) Libraries[state]->delete_Students();
    else if(choice==2) Libraries[state]->delete_Books();
}

void DatabaseTools::Search(){
    std::cout<<"Select the table to Search:\n1)Students\t2)Books\t\t3)Records\n";
    std::cin>>choice;
    if(choice==1) Libraries[state]->search_Students();
    else if(choice==2) Libraries[state]->search_Books();
    else if(choice==3) Libraries[state]->search_Records();
}

void DatabaseTools::Issue(){
    Libraries[state]->Issue_Book();
}

void DatabaseTools::Return(){
    Libraries[state]->Return_Book();
}

void DatabaseTools::SavePoint(){
    if(state<99){
        Libraries[state+1]=new Library(*Libraries[state]);      // make a copy of current state
        state++;                                                // mark the new copy as current state
        std::cout<<"Successfully Saved\n";
    }
    else std::cout<<"More than 100 SavePoints Not Allowed\n";
}

void DatabaseTools::RollBack(){
    if(state>0){
        delete(Libraries[state]);       // delete the current state
        Libraries[state--]=nullptr;     // move to previous state
        std::cout<<"Successfully Restored\n";
    }
    else std::cout<<"No SavePoints to RollBack\n";
}

void DatabaseTools::Commit(){
    std::swap(Libraries[0],Libraries[state]);
    for(int i=1;i<=state;i++){
        delete(Libraries[i]);
        Libraries[i]=nullptr;
    }
    state=0;
    Libraries[0]->save_Table();
    std::cout<<"Successfully Committed\n";
}

void DatabaseTools::Menu(){
    while(choice!=10){
        std::cout<<"\n\nMAIN MENU\n1) Insert\t2) Update\t3) Delete\t4) Search\n5) Issue\t6) Return\n7) SavePoint\t8) RollBack\t9) Commit\t10) Exit\n";
        std::cout<<"Choose the Transaction Type : ";
        std::cin>>choice;
        if(choice>0 and choice<10){
            std::cout<<"\n";
            (this->*functions[choice-1])();
        }
        else if(choice>10) std::cout<<"Invalid Choice\n";
    }
}