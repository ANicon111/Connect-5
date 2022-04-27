#include "iostream"
using namespace std;

bool isInteractive=true;
short winningPlayer=0;

//<basics>
#pragma region
    //datele stocate in fiecare căsuță
    struct Cell{
        short player=0; //valori: -1 = perete, 1 = player 1, 2= player 2
        short parentX=-1,parentY=-1; //pozitia parintelui
        int childrenCount=0; //numarul de copii
    };

    //datele stocate in tablă
    struct Table{
        short width=6, height=7; //dimensiunile tablei
        Cell **table; //tabla (neinițializată)
    }game;

    //program de afisare a textului in terminal pentru gui (modul neinteractiv)
    void guiOutput(char const *text){
        if(!isInteractive)
            std::cout<<text;  
    }

    //program de afisare a textului in terminal pentru player (modul interactiv)
    void output(char const *text){
        if(isInteractive)
            std::cout<<text;
    }

    //se citesc setarile jocului aici
    void init(){
        output("Lățimea tablei:");
        cin>>game.width;
        output("Înălțimea tablei:");
        cin>>game.height;
        //crearea matricii tablei
        game.table = new Cell *[game.height];
        for(int i=0;i<game.height;i++)
            game.table[i]=new Cell[game.width];
    }

    void showWinner(){
        switch (winningPlayer){
        case 1:
            output("Player 1 won");
            break;
        case 2:
            output("Player 2 won");
            break;
        case -1:
            output("Tied");
            break;
        default:
            break;
        }
    }
#pragma endregion
//</basics>

//<AI>
#pragma region

#pragma endregion
//</AI>

//<game>
#pragma region
    void runGame(){

    }
#pragma endregion
//</game>

//<main>
#pragma region
    int main(int argc, char **argv){
        //daca ruleaza in gui programul este lansat "connect5.exe 0" 
        //pentru a nu arata informatii inutile programului gui
        if(argc>1)
            if(argv[1][0]=='0')
                isInteractive=false;
        init();
        while(winningPlayer=0)
            runGame();
        showWinner();
        return 0;
    }
#pragma endregion
//<main>