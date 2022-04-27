#include "iostream"
bool isInteractive=true;
unsigned short width=6, height=7;

//program de afisare a textului in terminal
void output(char const *text){
    std::cout<<text;  
}

//program de afisare a textului in terminal daca este interactiv
void conditionalOutput(char const *text){
    if(isInteractive)
        std::cout<<text;
}

//se citesc setarile jocului aici
void init(const char *setting){
    std::cout<<(setting[0]=='0')<<setting;
    if(setting[0]=='0')
        isInteractive=false;
    output("E");
    conditionalOutput("\nE");
}