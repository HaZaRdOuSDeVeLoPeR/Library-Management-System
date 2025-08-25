#include "headers.hpp"
#include "structure.hpp"

Books::Books(std::string file){
    file_name=file;
}

void Books::load_Table(){
    std::ifstream input(file_name);

    if (!input.is_open()) {
        std::cout<<"Failed to Open Books table.\n";
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
        int id=std::stoi(row[0]);
        std::string title=row[1], author=row[2], genre=row[3], available=row[4];    // getting column values
        if(available=="Yes" or available=="No");
        else{
            std::cout<<"Availability column can accept only Yes/No\n";
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

void Books::save_Table(){
    std::ofstream output(file_name);

    if (!output.is_open()) {
        std::cout<<"Failed to Save Books table.\n";
        exit(0);
    }

    output<<"BookID,Title,Author,Genre,Available\n";
    for(auto &x:bookID){
        std::string row="";
        row+=std::to_string(x); row+=','; row+=id_title[x]; row+=','; row+=id_author[x]; row+=','; row+=id_genre[x]; row+=',';row+=id_available[x]; row+='\n';
        output<<row;
    }

    output.close();
}

void Books::insert_Book(){
    std::string line;
    std::cout<<"Enter (BookID, Title, Author, Genre) : ";
    std::cin.ignore();
    std::getline(std::cin,line);

    if(Helper::process_brackets(line));
    else return;

    std::vector<std::string> input;
    if(Helper::process_attributes(line,input,',',typeInfo,attributes,type,4,0,false));
    else return;

    int id=std::stoi(input[0]);
    std::string title=input[1], author=input[2], genre=input[3], available="Yes";
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
        std::cout<<"Book added Successfully\n";
    }
    else{
        std::cout<<"Book with duplicate IDs is not allowed."<<"\n";
    }
}

void Books::delete_Books(std::unordered_set<int> &selected){
    for(auto &x:selected){
        bookID.erase(x);

        std::string title=id_title[x], author=id_author[x], genre=id_genre[x], available=id_available[x];

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

void Books::update_Books(std::unordered_set<int> &selected, std::string title, std::string author, std::string genre){
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

void Books::display_Books(std::unordered_set<int> &selected){
    std::cout<<"\n";
    std::cout<<std::setw(6)<<"BookID"<<std::setw(80)<<"Title"<<std::setw(40)<<"Author"<<std::setw(10)<<"Genre"<<std::setw(12)<<"Available"<<"\n";
    for(auto &x:selected){
        std::cout<<std::setw(6)<<x<<std::setw(80)<<id_title[x]<<std::setw(40)<<id_author[x]<<std::setw(10)<<id_genre[x]<<std::setw(12)<<id_available[x]<<"\n";
    }
    std::cout<<"\nFetched "<<selected.size()<<" rows Successfully\n";
}

std::unordered_set<int> Books::select_Books(int id, std::string title, std::string author, std::string genre, std::string available, std::string optr){
    std::unordered_map<int,int> selected_id_freq;
    std::unordered_set<int> selected;

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
        std::cout<<"Missing Operator\n";
        return {};
    }

    return selected;
}