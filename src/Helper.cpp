#include "headers.hpp"
#include "structure.hpp"

bool Helper::check_number(std::string &number){
    for(int i=0;i<number.length();i++){
        if(!isdigit(number[i])) return false;
    }   
    return true;
}

bool Helper::process_brackets(std::string &line){
    if(line[0]!='(' or line.back()!=')'){
        std::cout<<"Missing Brackets\n";
        return false;
    }
    line.erase(line.begin());
    line.pop_back();
    return true;
}

bool Helper::process_attributes(std::string &line, std::vector<std::string> &processed, char seperator, std::vector<std::string> &typeInfo, std::vector<std::string> &attributes, std::vector<std::string> &type, int end, int offset, bool state){
    std::stringstream ss(line);
    std::string vals;

    int i=offset;
    while(i<end and std::getline(ss,vals,seperator)){
        if(vals[0]=='\'' and vals.back()=='\''){
            vals.erase(vals.begin());
            vals.pop_back();
            if(vals.length() and typeInfo[i]==typeid(std::string).name()) processed.push_back(vals);
            else{
                std::cout<<attributes[i]<<" must be of type "<<type[i]<<"\n";
                return false;
            }
        }
        else if(check_number(vals)){
            if(vals.length() and typeInfo[i]==typeid(int).name()) processed.push_back(vals);
            else{
                std::cout<<attributes[i]<<" must be of type "<<type[i]<<"\n";
                return false;
            }
        }
        else{
            std::cout<<attributes[i]<<" must be of type "<<type[i]<<"\n";
            return false;
        }
        i++;
    }

    if(state and getline(ss,vals,seperator)){
        if(vals=="0" or vals=="AND" or vals=="OR" or vals=="ON" or vals=="BEFORE" or "AFTER") processed.push_back(vals);
    }
    else if(state){
        std::cout<<"Missing operator\n";
        return false;
    }

    if(processed.size()>=end-offset) return true;
    else{
        std::cout<<"Missing Attributes\n";
        return false;
    }
}

bool Helper::validate_date(std::string &date){
    if((isdigit(date[0]) and isdigit(date[1]) and date[2]=='-' and isdigit(date[3]) and isdigit(date[4]) and date[5]=='-' and isdigit(date[6]) and isdigit(date[7]) and isdigit(date[8]) and isdigit(date[9])) or date=="Overdue") return true;
    else{
        std::cout<<"Wrong Date\n";
        return false;
    }
}

void Helper::reverse_date(std::string &date){
    if(date=="Overdue" or date=="0") return;
    std::stringstream ss(date);
    std::string day,month,year;
    std::getline(ss,day,'-'); getline(ss,month,'-'); getline(ss,year,'-');
    date=year+'-'+month+'-'+day;
}