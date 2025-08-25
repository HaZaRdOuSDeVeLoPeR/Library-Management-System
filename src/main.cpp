#include "structure.hpp"
#include "headers.hpp"

int main(){
    DatabaseTools my_library;
    my_library.NewDatabase("Tables/Students.csv","Tables/Books.csv","Tables/Issues.csv");
    my_library.Menu();
}