#include "headers.hpp"
#include "structure.hpp"

Students :: Students(std::string file){
    file_name=file;
}

void Students::load_Table(){
    std::ifstream input(file_name);

    if(!input.is_open()) {
        std::cout<<"Failed to Open Students table.\n";
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

        int id=std::stoi(row[0]), year=stoi(row[3]);         // getting column values
        std::string name=row[1], branch=row[2];

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

void Students::save_Table(){
    std::ofstream output(file_name);

    if (!output.is_open()) {
        std::cout<<"Failed to Save Students table.\n";
        exit(0);
    }

    output<<"StudentID,Name,Branch,Year\n";
    for(auto &x:studentID){
        std::string row="";
        row+=std::to_string(x); row+=','; row+=id_name[x]; row+=','; row+=id_branch[x]; row+=','; row+=std::to_string(id_year[x]); row+='\n';
        output<<row;
    }

    output.close();
}

void Students::insert_Student(){
    std::string line;
    std::cout<<"Enter (StudentID,Name,Branch,Year) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',typeInfo,attributes,type,4,0,false));
    else return;

    int id=std::stoi(input[0]),year=std::stoi(input[3]);
    std::string name=input[1], branch=input[2];
    if(studentID.find(id)==studentID.end()){
        studentID.insert(id);
        id_name[id]=name;
        id_branch[id]=branch;
        id_year[id]=year;

        name_id[name].insert(id);
        branch_id[branch].insert(id);
        year_id[year].insert(id);
        std::cout<<"Student added Successfully\n";
    }
    else{
        std::cout<<"Student with duplicate IDs is not allowed."<<"\n";
    }
}

void Students::delete_Students(std::unordered_set<int> &selected){
    for(auto &x:selected){
        studentID.erase(x);

        std::string name=id_name[x], branch=id_branch[x];
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

void Students::update_Students(std::unordered_set<int> &selected, std::string name, std::string branch, int year){
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

void Students::display_Students(std::unordered_set<int> &selected){
    std::cout<<std::setw(12)<<"StudentID"<<std::setw(20)<<"Name"<<std::setw(10)<<"Branch"<<std::setw(8)<<"Year"<<"\n";
    for(auto &x:selected){
        std::cout<<std::setw(12)<<x<<std::setw(20)<<id_name[x]<<std::setw(10)<<id_branch[x]<<std::setw(8)<<id_year[x]<<"\n";
    }
    std::cout<<"\nFetched "<<selected.size()<<" rows Successfully\n";
}

std::unordered_set<int> Students::select_Students(int id, std::string name, std::string branch, int year, std::string optr){
    std::unordered_map<int,int> selected_id_freq;
    std::unordered_set<int> selected;

    int attributes=0;

    if(id!=0){
        if(Students::studentID.find(id)!=Students::studentID.end()) selected_id_freq[id]++;
        attributes++;
    }
    if(name!="0"){
        for(auto &x:Students::name_id[name]){
            selected_id_freq[x]++;
        }
        attributes++;
    }
    if(branch!="0"){
        for(auto &x:Students::branch_id[branch]){
            selected_id_freq[x]++;
        }
        attributes++;
    }
    if(year!=0){
        for(auto &x:Students::year_id[year]){
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