#include "iostream"
using namespace std;

bool isInteractive=true;
short p1Points=0, p2Points=0, winningPlayer=0, p1PointsCopy, p2PointsCopy;

//infinit
const float inf = 1.0/0.0;

//adâncimea maximă de căutare
int maxDepth=1;
 
//<basics>
#pragma region
    //datele stocate in fiecare căsuță
    struct Cell{
        short player=0; //valori: -1 = perete, 1 = player 1, 2= player 2
        unsigned short parentX=-1,parentY=-1; //pozitia parintelui
        int childrenCount=0; //numarul de copii
    } **tableCopy;

    //datele stocate in tablă
    struct Table{
        unsigned short width=6, height=7; //dimensiunile tablei
        Cell **table; //tabla (neinițializată)
        unsigned short *heights; //lista de înălțimi curente (neinițializată)
    }game;

    unsigned short *tableCopyHeights;

    //program de afisare a textului in terminal pentru gui (modul neinteractiv)
    template<class T>
    void guiOutput(T text){
        if(!isInteractive)
            std::cout<<text;  
    }

    //program de afisare a textului in terminal pentru player (modul interactiv)
    template<class T>
    void output(T text){
        if(isInteractive)
            std::cout<<text;
    }

    void clear(){
        if(isInteractive){
            #if defined _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }
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
        //crearea copiei matricii
        tableCopy = new Cell *[game.height];
        for(int i=0;i<game.height;i++)
            tableCopy[i]=new Cell[game.width];
        //crearea array-ului de înălțimi
        game.heights = new unsigned short[game.width];
        tableCopyHeights = new unsigned short[game.width];
        maxDepth=5;//oof 
    }

    void showWinner(){
        output("\n");
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

    void printBoard(){
        clear();
        const char *h ="═", *v ="║", *tr="╗", *tl="╔", *br="╝", *bl="╚", *p1="□", *p2="■", *b="╳", *e=" ";
        //marginea de sus
        output(tl);
        for(int i=0;i<game.width;i++)
            output(h);
        output(tr);
        output("\n");

        //rândurile
        for(int i=game.height-1;i>=0;i--){
            output(v);
            for(int j=0;j<game.width;j++)
                switch (game.table[i][j].player){
                case 0:
                    output(e);
                    break;
                case -1:
                    output(b);
                    break;
                case 1:
                    output(p1);
                    break;
                case 2:
                    output(p2);
                    break;
                default:
                    break;
                }
            output(v);
            output("\n");
        }

        //marginea de jos
        output(bl);
        for(int i=0;i<game.width;i++)
            output(h);
        output(br);
        output("\n");
        output("P1:");
        output(p1Points);
        output(" P2:");
        output(p2Points);
    }

    pair<unsigned short, unsigned short> getParent(unsigned short x, unsigned short y){
        while(game.table[x][y].parentX!=(unsigned short)-1){
            Cell c=game.table[x][y];
            x=c.parentX;
            y=c.parentY;
        }
        return pair<unsigned short, unsigned short>(x, y);
    }

    void connectPieces(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2){
        if(
            x1<game.height&&
            y1<game.width&&
            x2<game.height&&
            y2<game.width&&
            game.table[x1][y1].player==game.table[x2][y2].player&&(
                game.table[x1][y1].player==1||
                game.table[x1][y1].player==2
            )
        ){
            auto c=getParent(x1,y1);
            x1=c.first;
            y1=c.second;
            c=getParent(x2,y2);
            x2=c.first;
            y2=c.second;
            if(x1!=x2||y1!=y2){
                game.table[x1][y1].parentX=x2;
                game.table[x1][y1].parentY=y2;
                game.table[x2][y2].childrenCount+=game.table[x1][y1].childrenCount;
            }
        }
    }
#pragma endregion
//</basics>

//<gameTools>
#pragma region
    //transforma gruparile de 5 in pereti
    void processWall(unsigned short x, unsigned short y, short player){
        if(x<game.height&&y<game.width&&game.table[x][y].player==player){
            game.table[x][y].player=-1;
            //procesatul peretilor
            processWall(x, y+1, player);
            processWall(x, y-1, player);
            processWall(x+1, y, player);
            processWall(x-1, y, player);
        }
    }

    void placePiece(unsigned short pos, short player){
        unsigned short height=game.heights[pos]++;
        //plasarea piesei
        game.table[height][pos].player=player;
        game.table[height][pos].childrenCount=1;
        //verificarea tablei dacă este plină
        bool allMaxed=true;
        for(int i=0;i<game.width;i++)
            if(game.heights[i]<game.height){
                allMaxed=false;
                break;
            }
        if(allMaxed){
            if(p1Points>p2Points)
                winningPlayer=1;
            else if(p1Points<p2Points)
                winningPlayer=2;
            else 
                winningPlayer=-1;
        }
        //conectarea cu vecinii
        connectPieces(height, pos, height+1, pos);
        connectPieces(height, pos, height-1, pos);
        connectPieces(height, pos, height, pos+1);
        connectPieces(height, pos, height, pos-1);
        //verificare numarului de vecini
        auto c=getParent(height, pos);
        if(game.table[c.first][c.second].childrenCount>=5){
            int val=5+(game.table[c.first][c.second].childrenCount-5)*2;
            if(player==1){
                p1Points+=val;
                if(p1Points>=50)
                    winningPlayer=1;
            }else{
                p2Points+=val;
                if(p2Points>=50)
                    winningPlayer=2;
            }
            processWall(height, pos, player);
        }
    }
#pragma endregion
//</gameTools>

//<AI>
#pragma region
    pair<unsigned short, unsigned short> getAIParent(unsigned short x, unsigned short y){
        while(tableCopy[x][y].parentX!=(unsigned short)-1&&(tableCopy[x][y].parentX!=x&&tableCopy[x][y].parentY!=y)){
            Cell c=tableCopy[x][y];
            x=c.parentX;
            y=c.parentY;
        }
        return pair<unsigned short, unsigned short>(x, y);
    }

    float getMoveValue(int y){
        float val=0;
        if(tableCopyHeights[y]==game.height)
            return -inf;
        int x=tableCopyHeights[y];
        if(x>0&&tableCopy[x-1][y].player!=-1)
            val+=tableCopy[x-1][y].player==2?2:1;
        if(y>0&&tableCopy[x][y-1].player!=-1&&tableCopy[x][y-1].player!=0)
            val+=tableCopy[x][y-1].player==2?2:1;
        if(y<game.width-1&&tableCopy[x][y+1].player!=-1&&tableCopy[x][y+1].player!=0)
            val+=tableCopy[x][y+1].player==2?2:1;
        return val;
    }

    int getBestMove(){
        for(int i=0;i<game.height;i++)
            for(int j=0;j<game.width;j++)
                tableCopy[i][j]=game.table[i][j];
        for(int i=0;i<game.width;i++)
            tableCopyHeights[i]=game.heights[i];
        p1PointsCopy=p1Points;
        p2PointsCopy=p2Points;
        int bestMove=0;
        float bestMoveValue=getMoveValue(0);
        for(int i=1;i<game.width;i++){
            float val=getMoveValue(i);
            if(val>bestMoveValue){
                bestMove=i;
                bestMoveValue=val;
            }
        }
        if(bestMoveValue==-inf)
            return -1;
        return bestMove;
    }

    void runAI(){
        int pos=getBestMove();
        guiOutput(pos);
        guiOutput("\n");
        if(pos!=-1)
            placePiece(pos, 2);
    }
#pragma endregion
//</AI>

//<game>
#pragma region
    void runGame(){
        int pos=-1;
        printBoard();
        output("\n\n(P1) Input column number:");
        cin>>pos;
        while((unsigned short)(pos-1)>=game.width||game.heights[pos-1]==game.height){
            output("\nInput VALID column number:");
            cin>>pos;
        }
        placePiece(pos-1, 1);
        if(winningPlayer==0)
            runAI();
    }
#pragma endregion
//</game>

//<main>
#pragma region
    int main(int argc, char **argv){
        //daca ruleaza in gui programul este lansat cu argumentul "0" 
        //pentru a nu arata informatii inutile programului gui
        if(argc>1)
            if(argv[1][0]=='0')
                isInteractive=false;
        init();
        while(winningPlayer==0)
            runGame();
        showWinner();
        return 0;
    }
#pragma endregion
//<main>