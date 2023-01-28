#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <windows.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

/*MODULO I - STRUCTS*/

#pragma region STRUCTS
struct Box{
	bool isFilled, isValid;
	int playerID;
	char token;
};

struct Color{
	int idColor;
    string colorName;
};

struct Player{
	int id, corner;
	string name;
	int colorID;
	char token;
    int score;
};

struct Coordinates //Coordenadas de una casilla en el tablero
{
    int x, y;
};

#pragma endregion
/*MODULO II - ARREGLOS*/

void SetColor(int ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle (STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute (hStdOut, wColor); 
    }
}

void ShowAvailableMoves(Box** board, int* x, int* y, int** availables, Player* players){

    cout << "    0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16" << endl;
	for (int i = 0; i < 17; i++)
    {
        if(i < 10) cout << " ";
        cout << i;

        if((i + 1) % 2 == 0) cout << "  ";
        for (int j = 0; j < 17; j++)
        {
            cout << "  ";
			if(board[i][j].isValid == false){
				cout << " ";
			}
            else if(board[i][j].isFilled){//Si la casilla esta llena la escribe con el color del jugador
				SetColor (players[board[i][j].playerID].colorID);
                cout << board[i][j].token;
            } else if (i == *x && j == *y){
				SetColor (3);
                cout << board[i][j].token;
			}
            else {
				// Sino si la casilla esta vacia y esta disponible para algun movimiento de la posicion dada, se pinta de verde agua
				if ((i == availables[0][0] && j == availables[1][0])
				 ||(i == availables[0][1] && j == availables[1][1])
				 ||(i == availables[0][2] && j == availables[1][2])
				 ||(i == availables[0][3] && j == availables[1][3])
				 ||(i == availables[0][4] && j == availables[1][4])
				 ||(i == availables[0][5] && j == availables[1][5]))
				{
					SetColor (11); //magenta
				} else
				{
					//Sino, si la casilla esta vacia y no puede moverse el jugador a ella, se pinta de gris
					SetColor (8); //gris oscuro 
				}
                cout << 'O';
            }            
        }
        cout << endl;
    }
	SetColor (7);    
}

void showBoard(Box** board, Player* players){
	cout << "   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16" << endl;
	for (int i = 0; i < 17; i++)
    {
        SetColor(8);
        if(i < 10) cout << " ";
        cout << i;

        if((i+ 1) % 2 == 0) cout << "  ";
        for (int j = 0; j < 17; j++)
        {
            SetColor(8);
            cout << "  ";
			if(board[i][j].isValid == false){
				cout << " ";    
			}
            else if(board[i][j].isFilled){//Si la casilla esta llena la escribe con el color del jugador
				SetColor (players[board[i][j].playerID].colorID); //id 1 o 2
                cout << board[i][j].token;
            } else
            {
				cout << 'O';
            }            
        }
        cout << endl;
    }
	SetColor (7);   
}

/*MODULO I - INTERFACES*/
#pragma region INTERFACES

void gotoxy(int x,int y)
{
HANDLE hcon;
hcon = GetStdHandle(STD_OUTPUT_HANDLE);
COORD dwPos;
dwPos.X = x;
dwPos.Y= y;
SetConsoleCursorPosition(hcon,dwPos);
}

void NewFileInstructions (){
    ofstream Instructions;
    Instructions.open("Instructions.txt", ios::out);
    if (Instructions.is_open()){
        Instructions<<"INSTRUCCIONES DEL JUEGO"<<endl<<endl;
        Instructions<<"Se juega en un tablero con 121 casillas en forma de estrella de David, (de seis puntas)."<<endl<<
                        "Cada jugador consta de diez fichas. Al empezar el juego, estas diez fichas de un mismo jugador"<<endl<<
                        "estan juntas, en uno de los triangulos que forman las puntas de la estrella."<<endl<<
                        "Cada jugador tiene un color diferente o una caracteristica que las distinga de las del otro jugador."<<endl<<
                        "El objetivo del juego es llevar sus 10 fichas al extremo opuesto de su posicion inicial"<<endl<<
                        "Los jugadores lanzaran un dado para definir su turno."<<endl<< 
                        "El que tenga mayor puntaje, elige su posicion y comienza la partida"<<endl<<
                        "El primero que logre llevar todas sus fichas al extremo opuesto, ¡GANA!"<<endl<<endl;
        Instructions<<"REGLAS A CONSIDERAR:"<<endl<<endl;
        Instructions<<"1. Cada jugador solo mueve una ficha por turno"<<endl<<
                        "2. Un movimiento valido es: "<<endl<<
                        "    - A una casilla adyacente libre"<<endl<<
                        "    - Saltando una casilla adyacente ocupada por otra ficha (sea una ficha propia o no)"<<endl<<
                        "      y posandola en la casilla siguiente (en la misma direccion) si esta libre."<<endl<<endl<<
                        "Esta segunda opcion permite volver a mover su ficha si encuentra otra oportunidad de salto."<<endl;
        Instructions<<"\nPresione una tecla para continuar...";
        Instructions.close();
    }
    else
        cout<<"Error de apertura de archivo"<<endl;
}
void OutputInstructions(){ //leer archivo
    ifstream Instructions;
    string text;
    Instructions.open("Instructions.txt", ios::in);
    if (Instructions.fail()){
        cout<<"No se pudo abrir el archivo";
        exit(0);
    }
    while (!Instructions.eof()){
        getline(Instructions, text);

        cout<<text<<endl;
    }
    Instructions.close();
    getch();
}

void showBanner(){
	int x, y;
	for (x=25; x<94; x++){
		for (y=2; y<20; y++){
			gotoxy (x,y);
			SetColor (2);
			if (x==25 || x==93 || y==2 || y==19) cout<<"*";
		}
	}
	x = 30; y=7;
	SetColor (1);
	gotoxy(x,y);cout<< "*******  ";y++;
    gotoxy(x,y);cout<< "******** ";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "******** ";y++;
    gotoxy(x,y);cout<< "*******  ";y++;
    x = 40; y = 7;
    SetColor (4);
    gotoxy(x,y);cout<< "  *****  ";y++;
    gotoxy(x,y);cout<< " ******* ";y++;
    gotoxy(x,y);cout<< "**** ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    x = 50; y = 7;
    SetColor (1);
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "**** ****";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** * ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    x = 60; y = 7;
    SetColor (4);
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    x = 70; y = 7;
    SetColor (1);
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    x = 80; y = 7;
    SetColor (4);
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    x = 39; y = 17;
    gotoxy(x,y);
    SetColor (2);
    cout<<"***PRESIONE ENTER PARA CONTINUAR...***";
    SetColor (7);
    getch();
    system("cls");
}

void showStarting(){
	char op;
	bool Valid = true;
	while (Valid){
	gotoxy (30, 3);
	cout<<"MENU DE INICIO"<<endl;
	cout<<"[A] Jugar"<<endl;
	cout<<"[B] Mostrar Instrucciones"<<endl;
	cout<<"[C] Salir"<<endl;
	cout<<"Seleccione una opcion: "; cin>>op;
	switch (op) {
		case 'A':
		case 'a': 
				Valid = false; 
				system ("cls"); break;
		case 'B':
		case 'b':
                system ("cls");
				NewFileInstructions();
                OutputInstructions();
                system ("cls"); break;
		case 'C':
		case 'c':
				cout<<"\nGRACIAS POR JUGAR :)";
				exit(0);
		default: 
				system("cls");
		}
	}
}
void Credits(){
	int x, y;
	for (x=1; x<119; x++){
		for (y=0; y<30; y++){
			gotoxy (x,y);
			SetColor (7);
			if (x==1 || x==118 || y==0 || y==29) cout<<"*";
		}
	}
	x = 60;
    for (y = 2; y<28; y++){
     	gotoxy (x, y);
     	cout<<"| |";
    }
	Sleep(80);
	x = 6; y=2;
	gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
	gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    Sleep(80);
    x = 17; y = 2;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***    **";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    Sleep(80);
    x = 28; y = 2;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "****  ***";y++;
    gotoxy(x,y);cout<< "***** ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** *****";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    Sleep(80);
    x = 6; y = 11;
    gotoxy(x,y);cout<< "  *****  ";y++;
    gotoxy(x,y);cout<< " ******* ";y++;
    gotoxy(x,y);cout<< "**** ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    Sleep(80);
     x = 17; y = 11;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "****  ***";y++;
    gotoxy(x,y);cout<< "***** ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** *****";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    Sleep(80);
     x = 28; y = 11;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "****  ***";y++;
    gotoxy(x,y);cout<< "***** ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** *****";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    Sleep(80);
     x = 39; y = 11;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "**** ****";y++;
    gotoxy(x,y);cout<< " ******* ";y++;
    gotoxy(x,y);cout<< "  *****  ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    Sleep(80);
     x = 6; y = 20;
    gotoxy(x,y);cout<< " ******* ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< " ******* ";y++;
    Sleep(80);
     x = 17; y = 20;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "**** ****";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** * ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    Sleep(80);
     x = 28; y = 20;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    Sleep(80);
     x = 39; y = 20;
    gotoxy(x,y);cout<< "   ******";y++;
    gotoxy(x,y);cout<< " ********";y++;
    gotoxy(x,y);cout<< "****     ";y++;
    gotoxy(x,y);cout<< "*******  ";y++;
    gotoxy(x,y);cout<< "   ***** ";y++;
    gotoxy(x,y);cout<< "     ****";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*******  ";y++;
    Sleep(80);
	 x = 80; y = 8;
	gotoxy(x,y);cout<<"EQUIPO DE DESARROLLADORES";
	 x = 75; y = 11; gotoxy(x,y); cout<<"- HECTOR HUARIPATA - TOR";
	 x = 75; y = 13; gotoxy(x,y); cout<<"- ALEXANDER ROJAS - AVISORX";
	 x = 75; y = 15; gotoxy(x,y); cout<<"- STEFANO HERNANDEZ - SAHB";
	 x = 75; y = 20;
    gotoxy(x,y);cout<<" **** ";y++;
    gotoxy(x,y);cout<<"**  **";y++;
    gotoxy(x,y);cout<<"   ***";y++;
    gotoxy(x,y);cout<<"  *** ";y++;
    gotoxy(x,y);cout<<" **   ";y++;
    gotoxy(x,y);cout<<"******";y++;
     x = 83; y =20;
    gotoxy(x,y);cout<<"  ***  ";y++;
    gotoxy(x,y);cout<<" ** ** ";y++;
    gotoxy(x,y);cout<<"*** ***";y++;
    gotoxy(x,y);cout<<"*** ***";y++;
    gotoxy(x,y);cout<<" ** ** ";y++;
    gotoxy(x,y);cout<<"  ***  ";y++;
    x = 93; y = 20;
  	gotoxy(x,y);cout<<" **** ";y++;
    gotoxy(x,y);cout<<"**  **";y++;
    gotoxy(x,y);cout<<"   ***";y++;
    gotoxy(x,y);cout<<"  *** ";y++;
    gotoxy(x,y);cout<<" **   ";y++;
    gotoxy(x,y);cout<<"******";y++;
    x = 103; y = 20;
    gotoxy(x,y);cout<<" ****  ";y++;
    gotoxy(x,y);cout<<"** **  ";y++;
    gotoxy(x,y);cout<<"   **  ";y++;
    gotoxy(x,y);cout<<"   **  ";y++;
    gotoxy(x,y);cout<<"*******";y++;
    gotoxy(x,y);cout<<"*******";y++;
	getch();
	system("cls");
}

void showWinner(Player winner){
	int x, y, i=0;
	do {
	system("cls");
	Sleep(800);
	x = 30; y=7;
	SetColor (12);
	gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*** * ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "**** ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    x = 40; y = 7;
    SetColor (12);
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "   ***   ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    x = 50; y = 7;
    SetColor (12);
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "****  ***";y++;
    gotoxy(x,y);cout<< "***** ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** *****";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    x = 60; y = 7;
    SetColor (12);
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "****  ***";y++;
    gotoxy(x,y);cout<< "***** ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*** *****";y++;
    gotoxy(x,y);cout<< "***  ****";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    x = 70; y = 7;
    SetColor (12);
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***      ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*********";y++;
    x = 80; y = 7;
    SetColor (12);
    gotoxy(x,y);cout<< "******** ";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    gotoxy(x,y);cout<< "*********";y++;
    gotoxy(x,y);cout<< "*******  ";y++;
    gotoxy(x,y);cout<< "***  *** ";y++;
    gotoxy(x,y);cout<< "***   ***";y++;
    i++;
    Sleep(80);
	} while (i<4);
	SetColor (7);
    x = 39; y = 17;
    gotoxy(x,y);
    cout<<"CONGRATULATIONS "<<winner.name<<"!";
    getch();
}
#pragma endregion

/*MODULO II - MECANICAS*/
#pragma region MECANICAS
void createBoard(Box** board){
    
	char model[] = {'*','*','*','*','*','*','*','*','#','*','*','*','*','*','*','*','*',
                      	  '*','*','*','*','*','*','*','#','#','*','*','*','*','*','*','*','*',
                    	'*','*','*','*','*','*','*','#','#','#','*','*','*','*','*','*','*',
                      	  '*','*','*','*','*','*','#','#','#','#','*','*','*','*','*','*','*',
                    	'*','*','#','#','#','#','#','#','#','#','#','#','#','#','#','*','*',
                      	  '*','*','#','#','#','#','#','#','#','#','#','#','#','#','*','*','*',
                    	'*','*','*','#','#','#','#','#','#','#','#','#','#','#','*','*','*',
                          '*','*','*','#','#','#','#','#','#','#','#','#','#','*','*','*','*',
                    	'*','*','*','*','#','#','#','#','#','#','#','#','#','*','*','*','*',
                      	  '*','*','*','#','#','#','#','#','#','#','#','#','#','*','*','*','*',
                    	'*','*','*','#','#','#','#','#','#','#','#','#','#','#','*','*','*',
                      	  '*','*','#','#','#','#','#','#','#','#','#','#','#','#','*','*','*',
                    	'*','*','#','#','#','#','#','#','#','#','#','#','#','#','#','*','*',
                      	  '*','*','*','*','*','*','#','#','#','#','*','*','*','*','*','*','*',
                    	'*','*','*','*','*','*','*','#','#','#','*','*','*','*','*','*','*',
                      	  '*','*','*','*','*','*','*','#','#','*','*','*','*','*','*','*','*',
                    	'*','*','*','*','*','*','*','*','#','*','*','*','*','*','*','*','*'};

	for (int i = 0; i < 17; i++)
	{
    	for (int j = 0; j < 17; j++)
        {
            board[i][j].isFilled = false;
        	board[i][j].playerID = -1;
        	if (model[17*i + j] == '#')
        	{
            	board[i][j].isValid = true;
            	board[i][j].token = 'O';       	 
        	}else{//para cuando plantilla[17*i + j] == '*'
            	board[i][j].isValid = false;           	 
            	board[i][j].token = ' ';
        	} 
        }
        	       	 
	}    
}

bool isThereWinner(Box** board, Coordinates** SymmetricPositions, Player* player, int idPlayer){

	/*IMPORTANTE:
	SymmetricPositions[0] --> Posiciones de las casillas que tiene que llegar el jugador 1
	SymmetricPositions[1] --> Posiciones de las casillas que tiene que llegar el jugador 2*/
	bool possibleWinner = true;

	int posX, posY, i = 0;

    while (possibleWinner)
    {
        posX = SymmetricPositions[player[idPlayer].id][i].x;
        posY = SymmetricPositions[player[idPlayer].id][i].y;
        if (board[posX][posY].isFilled == false || board[posX][posY].playerID != player[idPlayer].id)
        {
            possibleWinner = false;
        }
        i++;
    }
    
    return possibleWinner;
}

#pragma endregion

/*MODULO III - ACCIONES NO RELACIOANDAS*/
#pragma region ACCIONES NO RELACIONADAS
void registerPlayer(Player* arrayPlayers){

    for(int i = 0; i < 2; i++){
        cout << "Ingresar nombre de jugador " << i + 1 << ": " << endl; //Hacer que reconozca espacios
        cin >> arrayPlayers[i].name;
		
        cout << endl;
		fflush(stdin);
    }
}

void throwDice(Player* arrayPlayers){
    
    int currentScore = 0;
    int i = 0;

    srand(time(NULL));

    do
    {
        cout << "Jugador " << arrayPlayers[i].name << " lanzando dado..." << endl;
        Sleep(2000);
        currentScore = 1 + rand() % (6 + 1 - 1);
        cout << "El jugador " << arrayPlayers[i].name << " ha sacado un: " << currentScore;
        arrayPlayers[i].score = currentScore;
        cout << endl;
        i++;

        if(arrayPlayers[0].score == arrayPlayers[1].score){
             i = 0; //Reiniciar bucle
             system("cls");
             cout << "Los puntajes obtenidos de ambos jugadores fueron iguales. Proceden a lanzar nuevamente" << endl;
             Sleep(3000);
        }

    } while (i < 2 || (arrayPlayers[0].score == arrayPlayers[1].score));
}

void fillArrayColors(Color* arrayColors){
    arrayColors[0].idColor = 0;
    arrayColors[0].colorName = "azul";

    arrayColors[1].idColor = 1;
    arrayColors[1].colorName = "verde";

    arrayColors[2].idColor = 2;
    arrayColors[2].colorName = "amarillo";

    arrayColors[3].idColor = 3;
    arrayColors[3].colorName = "blanco";

    arrayColors[4].idColor = 4;
    arrayColors[4].colorName = "rojo";

    arrayColors[5].idColor = 5;
    arrayColors[5].colorName = "morado";
}

void setCornerTriangularPositions(Coordinates** arrayAllTriangularGroupPositions){
    //coordinates[n][0] --> Coord X, coordinates[n][1] --> Coord Y. (X,Y) coordenada de la casilla en el tablero
    //coordinates[6][10] COLOR(fila) x NUMERO DE COORDENADA (columna)
    //Para Esquina 0 -->
    arrayAllTriangularGroupPositions[0][0].x = 8;//Coord X de ficha 0
    arrayAllTriangularGroupPositions[0][0].y = 0;//Coord Y de ficha 0

    arrayAllTriangularGroupPositions[0][1].x = 7;//Coord X de ficha 1
    arrayAllTriangularGroupPositions[0][1].y = 1;//Coord Y de ficha 1

    arrayAllTriangularGroupPositions[0][2].x = 8;
    arrayAllTriangularGroupPositions[0][2].y = 1;

    arrayAllTriangularGroupPositions[0][3].x = 7;
    arrayAllTriangularGroupPositions[0][3].y = 2;

    arrayAllTriangularGroupPositions[0][4].x = 8;
    arrayAllTriangularGroupPositions[0][4].y = 2;

    arrayAllTriangularGroupPositions[0][5].x = 9;
    arrayAllTriangularGroupPositions[0][5].y = 2;

    arrayAllTriangularGroupPositions[0][6].x = 6;
    arrayAllTriangularGroupPositions[0][6].y = 3;

    arrayAllTriangularGroupPositions[0][7].x = 7;
    arrayAllTriangularGroupPositions[0][7].y = 3;

    arrayAllTriangularGroupPositions[0][8].x = 8;
    arrayAllTriangularGroupPositions[0][8].y = 3;

    arrayAllTriangularGroupPositions[0][9].x = 9;
    arrayAllTriangularGroupPositions[0][9].y = 3;

    //Para esquina 1 

    arrayAllTriangularGroupPositions[1][0].x = 11;
    arrayAllTriangularGroupPositions[1][0].y = 4;

    arrayAllTriangularGroupPositions[1][1].x = 12;
    arrayAllTriangularGroupPositions[1][1].y = 4;

    arrayAllTriangularGroupPositions[1][2].x = 14;
    arrayAllTriangularGroupPositions[1][2].y = 4;

    arrayAllTriangularGroupPositions[1][3].x = 13;
    arrayAllTriangularGroupPositions[1][3].y = 4;

    arrayAllTriangularGroupPositions[1][4].x = 11;
    arrayAllTriangularGroupPositions[1][4].y = 5;

    arrayAllTriangularGroupPositions[1][5].x = 12;
    arrayAllTriangularGroupPositions[1][5].y = 5;

    arrayAllTriangularGroupPositions[1][6].x = 13;
    arrayAllTriangularGroupPositions[1][6].y = 5;

    arrayAllTriangularGroupPositions[1][7].x = 12;
    arrayAllTriangularGroupPositions[1][7].y = 6;

    arrayAllTriangularGroupPositions[1][8].x = 13;
    arrayAllTriangularGroupPositions[1][8].y = 6;

    arrayAllTriangularGroupPositions[1][9].x = 12;
    arrayAllTriangularGroupPositions[1][9].y = 7;

    //Para esquina 2 

    arrayAllTriangularGroupPositions[2][0].x = 12;
    arrayAllTriangularGroupPositions[2][0].y = 9;

    arrayAllTriangularGroupPositions[2][1].x = 12;
    arrayAllTriangularGroupPositions[2][1].y = 10;

    arrayAllTriangularGroupPositions[2][2].x = 13;
    arrayAllTriangularGroupPositions[2][2].y = 10;

    arrayAllTriangularGroupPositions[2][3].x = 11;
    arrayAllTriangularGroupPositions[2][3].y = 11;

    arrayAllTriangularGroupPositions[2][4].x = 12;
    arrayAllTriangularGroupPositions[2][4].y = 11;

    arrayAllTriangularGroupPositions[2][5].x = 13;
    arrayAllTriangularGroupPositions[2][5].y = 11;

    arrayAllTriangularGroupPositions[2][6].x = 11;
    arrayAllTriangularGroupPositions[2][6].y = 12;

    arrayAllTriangularGroupPositions[2][7].x = 12;
    arrayAllTriangularGroupPositions[2][7].y = 12;

    arrayAllTriangularGroupPositions[2][8].x = 13;
    arrayAllTriangularGroupPositions[2][8].y = 12;

    arrayAllTriangularGroupPositions[2][9].x = 14;
    arrayAllTriangularGroupPositions[2][9].y = 12;

    //Para esquina 3 
    arrayAllTriangularGroupPositions[3][0].x = 6;
    arrayAllTriangularGroupPositions[3][0].y = 13;

    arrayAllTriangularGroupPositions[3][1].x = 7;
    arrayAllTriangularGroupPositions[3][1].y = 13;

    arrayAllTriangularGroupPositions[3][2].x = 8;
    arrayAllTriangularGroupPositions[3][2].y = 13;

    arrayAllTriangularGroupPositions[3][3].x = 9;
    arrayAllTriangularGroupPositions[3][3].y = 13;

    arrayAllTriangularGroupPositions[3][4].x = 7;
    arrayAllTriangularGroupPositions[3][4].y = 14;

    arrayAllTriangularGroupPositions[3][5].x = 8;
    arrayAllTriangularGroupPositions[3][5].y = 14;


    arrayAllTriangularGroupPositions[3][6].x = 9;
    arrayAllTriangularGroupPositions[3][6].y = 14;


    arrayAllTriangularGroupPositions[3][7].x = 7;
    arrayAllTriangularGroupPositions[3][7].y = 15;

    arrayAllTriangularGroupPositions[3][8].x = 8;
    arrayAllTriangularGroupPositions[3][8].y = 15;

    arrayAllTriangularGroupPositions[3][9].x = 8;
    arrayAllTriangularGroupPositions[3][9].y = 16;

    //Para esquina 4
    arrayAllTriangularGroupPositions[4][0].x = 3;
    arrayAllTriangularGroupPositions[4][0].y = 9;

    arrayAllTriangularGroupPositions[4][1].x = 3;
    arrayAllTriangularGroupPositions[4][1].y = 10;

    arrayAllTriangularGroupPositions[4][2].x = 4;
    arrayAllTriangularGroupPositions[4][2].y = 10;

    arrayAllTriangularGroupPositions[4][3].x = 2;
    arrayAllTriangularGroupPositions[4][3].y = 11;

    arrayAllTriangularGroupPositions[4][4].x = 3;
    arrayAllTriangularGroupPositions[4][4].y = 11;

    arrayAllTriangularGroupPositions[4][5].x = 4;
    arrayAllTriangularGroupPositions[4][5].y = 11;


    arrayAllTriangularGroupPositions[4][6].x = 2;
    arrayAllTriangularGroupPositions[4][6].y = 12;


    arrayAllTriangularGroupPositions[4][7].x = 3;
    arrayAllTriangularGroupPositions[4][7].y = 12;

    arrayAllTriangularGroupPositions[4][8].x = 4;
    arrayAllTriangularGroupPositions[4][8].y = 12;

    arrayAllTriangularGroupPositions[4][9].x = 5;
    arrayAllTriangularGroupPositions[4][9].y = 12;

    //Para esquina 5 -- morado
    arrayAllTriangularGroupPositions[5][0].x = 2;
    arrayAllTriangularGroupPositions[5][0].y = 4;

    arrayAllTriangularGroupPositions[5][1].x = 3;
    arrayAllTriangularGroupPositions[5][1].y = 4;

    arrayAllTriangularGroupPositions[5][2].x = 4;
    arrayAllTriangularGroupPositions[5][2].y = 4;

    arrayAllTriangularGroupPositions[5][3].x = 5;
    arrayAllTriangularGroupPositions[5][3].y = 4;

    arrayAllTriangularGroupPositions[5][4].x = 2;
    arrayAllTriangularGroupPositions[5][4].y = 5;

    arrayAllTriangularGroupPositions[5][5].x = 3;
    arrayAllTriangularGroupPositions[5][5].y = 5;

    arrayAllTriangularGroupPositions[5][6].x = 4;
    arrayAllTriangularGroupPositions[5][6].y = 5;

    arrayAllTriangularGroupPositions[5][7].x = 3;
    arrayAllTriangularGroupPositions[5][7].y = 6;

    arrayAllTriangularGroupPositions[5][8].x = 4;
    arrayAllTriangularGroupPositions[5][8].y = 6;

    arrayAllTriangularGroupPositions[5][9].x = 3;
    arrayAllTriangularGroupPositions[5][9].y = 7;
}

void setFirstPositionsOnBoard(Box** board, Coordinates** arrayPlayerStartingPositions, Player player){
    int posX, posY;
    int idPlayer;
    idPlayer = player.id;

    //Actualizar tablero con fichas y casillas que pertenecen a cada jugador
    for(int i = 0; i < 10; i++){
        posX = arrayPlayerStartingPositions[idPlayer][i].y;
        posY = arrayPlayerStartingPositions[idPlayer][i].x;

        board[posX][posY].isFilled = true;
        board[posX][posY].playerID = idPlayer;
        board[posX][posY].token = player.token;
    }
}

void setStartingPositions(Coordinates** arrayAllTriangularGroupPositions, Player player, Coordinates** arrayPlayerStartingPositions){

    int idPlayer;
    idPlayer = player.id;
    int selectedCornerId;
    selectedCornerId = player.corner;
    //Almacenar o registrar posiciones iniciales de fichas de jugador X
    for(int i = 0; i < 10; i++){

        arrayPlayerStartingPositions[idPlayer][i].x = 
                     arrayAllTriangularGroupPositions[selectedCornerId][i].x;
        arrayPlayerStartingPositions[idPlayer][i].y = 
                      arrayAllTriangularGroupPositions[selectedCornerId][i].y;
    }
}

void setBothSymmetricPositions(Coordinates** arrayAllTriangularGroupPositions, Player* player, Coordinates** arraySymmetricPositions){
    //guarda la ID del jugador en una variable local
    int idPlayer;
    //guarda la esquina del jugador en una variable local
    int selectedCornerId;
    //crea una variable para guardar la id de la esquina opuesta a la elegida por el jugador
    int idSymmetricTriangularCorner;
    

    for (int j = 0; j < 2; j++)
    {
        idPlayer = player[j].id;    
        selectedCornerId = player[j].corner;
        
        //guarda la id de la esquina opuesta al jugador en la variable idSymmetricTriangularCorner
        if(selectedCornerId < 3){
            idSymmetricTriangularCorner = selectedCornerId + 3;
        }
        else{
            idSymmetricTriangularCorner = selectedCornerId % 3;
        }

        //Registrar posiciones simetricas de las fichas de jugador X
        for(int i = 0; i < 10; i++){
                arraySymmetricPositions[idPlayer][i].x = arrayAllTriangularGroupPositions[idSymmetricTriangularCorner][i].x;
                arraySymmetricPositions[idPlayer][i].y = arrayAllTriangularGroupPositions[idSymmetricTriangularCorner][i].y;
        }
    }
}

void chooseTokenColor(Player *arrayPlayers, Color *arrayColors){
    int firstPlayer = 1; //Para identificar al jugador que obtuvo el mayor puntaje
    int secondPlayer = 2;

    //Para asegurar el orden de los turnos
    if(arrayPlayers[0].score < arrayPlayers[1].score){
        firstPlayer = 2;
        secondPlayer = 1;
    }

    //Seleccion de color de ficha del jugador del PRIMER turno
    cout << "Jugador " << arrayPlayers[firstPlayer - 1].name << " elige primero" << endl;

    cout << endl;

    //Listar opciones para seleccionar color de ficha
    for(int i = 0; i < 6; i++){
        cout << "Color " << arrayColors[i].colorName << ". Debera seleccionar el numero [" << arrayColors[i].idColor << "]" << endl;
    }
    
    //Seleccionando el color segun su ID y asignando ID esquina
    do
    {
        cout << "Digitar el numero segun el color con el que quiere empezar la partida: ";
        cin >> arrayPlayers[firstPlayer - 1].colorID;
    } while (!(arrayPlayers[firstPlayer - 1].colorID <= 5 &&  arrayPlayers[firstPlayer - 1].colorID >= 0));

    arrayPlayers[firstPlayer - 1].corner = arrayPlayers[firstPlayer - 1].colorID;
    

    //Seleccion de color de ficha del jugador de SEGUNDO turno
    cout << endl;
    cout << "Ahora le toca elegir a jugador " << arrayPlayers[secondPlayer - 1].name << ": " << endl;
    cout << endl;
    
    //Listar opciones para seleccionar color de ficha
    for(int i = 0; i < 6; i++){
        if(i != arrayPlayers[firstPlayer - 1].colorID){
            cout << "Color " << arrayColors[i].colorName << ". Debera seleccionar el numero [" << arrayColors[i].idColor << "]" << endl;
        }
    }
    
    do
    {
        cout << "Digitar el numero segun el color con el que quiere empezar la partida: ";
        cin >> arrayPlayers[secondPlayer - 1].colorID;
    } while (!(arrayPlayers[secondPlayer - 1].colorID <= 5 &&  arrayPlayers[secondPlayer - 1].colorID >= 0)/* || arrayPlayers[primerJugador - 1].colorID == arrayPlayers[segundoJugador - 1].colorID*/);

    arrayPlayers[secondPlayer - 1].corner = arrayPlayers[secondPlayer - 1].colorID;
}

void miniBoard(){
	int x, y;
	x = 70; y=9;
	SetColor (1);
	gotoxy(x,y); cout<<"  *  "; y++;
	gotoxy(x,y); cout<<" * * "; y++;
	gotoxy(x,y); cout<<"* * *"; y++;
	x = 64; y = 12;
	SetColor (5);
	gotoxy(x,y); cout<<"* * *"; y++;
	gotoxy(x,y); cout<<"* *"; y++;
	gotoxy(x,y); cout<<"  *"; y++;
	x = 65; y = 15;
	SetColor (4);
	gotoxy(x,y); cout<<"  *"; y++;
	gotoxy(x,y); cout<<" * *"; y++;
	gotoxy(x,y); cout<<"* * *"; y++;
	x = 70; y = 18;
	SetColor (15);
	gotoxy(x,y); cout<<"* * *"; y++;
	gotoxy(x,y); cout<<" * * "; y++;
	gotoxy(x,y); cout<<"  *  "; y++;
	x = 75; y = 15;
	SetColor (6);
	gotoxy(x,y); cout<<"  *"; y++;
	gotoxy(x,y); cout<<" * *"; y++;
	gotoxy(x,y); cout<<"* * *"; y++;
	x = 76; y = 12;
	SetColor (2);
	gotoxy(x,y); cout<<"* * *"; y++;
	gotoxy(x,y); cout<<" * *"; y++;
	gotoxy(x,y); cout<<"  *"; y++;
	SetColor (7);
	x = 69; y = 14;
	gotoxy(x,y); cout<<"CHOOSE!";
	x = 0; y = 18; gotoxy(x,y);
}
#pragma endregion


/*MODULO IV - MOVIMIENTOS*/
#pragma region Movimiento

int InputNumber(){
    int i;
    do
    {
        cin >> i;
    } while (i > 17 || i <= 0);
    return i;
}

bool CheckPossesion(Box** board, int x, int y, int idPlayer){
    if (board[y][x].playerID == idPlayer) return true;
    else return false;    
}

void ChooseToken(Box** board, int idPlayer, int* receiveX, int* receiveY){
    int x,y;
    bool belongsPlayer;
    do
    {
        cout << "Digite la FILA de la ficha que desea mover:"; y = InputNumber();
        cout << "Digite la COLUMNA de la ficha que desea mover:"; x = InputNumber();
        belongsPlayer = CheckPossesion(board,x,y,idPlayer);
        if (!belongsPlayer) cout << "No hay una ficha suya en esa posicion." << endl;        
    } while (!belongsPlayer);
    
    *receiveX = x;
    *receiveY = y;
}

void selectMovement(Box** board, int* receiveX, int* receiveY, int** availables){
    int x, y;//nuevas coordenadas
    bool isEqual;
    do{
        isEqual = false;
        cout << "Digite la FILA de la casilla a la que se desea mover:"; y = InputNumber();
        cout << "Digite la COLUMNA de la casilla a la que se desea mover:"; x = InputNumber();
        for (int i = 0; i < 6; i++)
        {
            if (y == availables[0][i] && x == availables[1][i]) isEqual = true;
        }        
    } while(!isEqual);
        
    //Desplazar Ficha
    
    //Se da a la casilla seleccionada las propiedades de la original
    board[y][x].isFilled = board[*receiveY][*receiveX].isFilled;
    board[y][x].playerID = board[*receiveY][*receiveX].playerID;
    board[y][x].token = board[*receiveY][*receiveX].token;

    //A la original se le da las propiedas de una vacía
    board[*receiveY][*receiveX].isFilled = false;
    board[*receiveY][*receiveX].playerID = -1;
    board[*receiveY][*receiveX].token = 'O';

    //Se cambia las coordenadas a la posicion actual de la ficha seleccionada
    *receiveX = x;
    *receiveY = y;
}

void DetectAvailableMoves(Box** board, int* x, int* y, int** NearAvailable){
    //    b  O  c              b   O   c
    // O  1  2  O            O   1   2   O
    // a  0  X  3  d        a  0   X   3   d
    // O  5  4  O            O   5   4   O
    //    f  O  e  O           f   O   e   O

    if (*y % 2 == 0)
    {
        //Detectar casillas libres alrededor para fila sin fase
        //Para 0
        if (board[*y][*x - 1].isValid){
            if (!(board[*y][*x - 1].isFilled)){
                NearAvailable[0][0] = *y;
                NearAvailable[1][0] = *x - 1;
            }else{
                //Para a
                if (board[*y][*x - 2].isValid && board[*y][*x - 2].isFilled == false){
                    NearAvailable[0][0] = *y;
                    NearAvailable[1][0] = *x - 2;
                }
            }
        }
        //Para 1
        if (board[*y - 1][*x - 1].isValid){
            if (board[*y - 1][*x - 1].isFilled == false){
                NearAvailable[0][1] = *y - 1;
                NearAvailable[1][1] = *x - 1;
            }else{
                //Para b
                if (board[*y - 2][*x - 1].isValid && board[*y - 2][*x - 1].isFilled == false){
                    NearAvailable[0][1] = *y - 2;
                    NearAvailable[1][1] = *x - 1;
                }
            }
        }
        //Para 2
        if (board[*y - 1][*x].isValid){
            if (board[*y - 1][*x].isFilled == false){
                NearAvailable[0][2] = *y - 1;
                NearAvailable[1][2] = *x;
            }else
            {
                //Para c
                if (board[*y - 2][*x + 1].isValid && board[*y - 2][*x + 1].isFilled == false){
                    NearAvailable[0][2] = *y - 2;
                    NearAvailable[1][2] = *x + 1;
                }
            }
        }
        //Para 3
        if (board[*y][*x + 1].isValid){
            if (board[*y][*x + 1].isFilled == false){
                NearAvailable[0][3] = *y;
                NearAvailable[1][3] = *x + 1;
            }
            else
            {
                //Para d
                if (board[*y][*x + 2].isValid && board[*y][*x + 2].isFilled == false){
                    NearAvailable[0][3] = *y;
                    NearAvailable[1][3] = *x + 2;
                }
            }
        }
        //Para 4
        if (board[*y + 1][*x].isValid){
            if (board[*y + 1][*x].isFilled == false){
                NearAvailable[0][4] = *y + 1;
                NearAvailable[1][4] = *x;
            }else
            {
                //Para e
                if (board[*y + 2][*x + 1].isValid && board[*y + 2][*x + 1].isFilled == false){
                    NearAvailable[0][4] = *y + 2;
                    NearAvailable[1][4] = *x + 1;
                }
            }
        }
        //Para 5
        if (board[*y + 1][*x - 1].isValid){
            if (board[*y + 1][*x - 1].isFilled == false){
                NearAvailable[0][5] = *y + 1;
                NearAvailable[1][5] = *x - 1;
            }else{
                //Para f
                if (board[*y + 2][*x - 1].isValid && board[*y + 2][*x - 1].isFilled == false){
                    NearAvailable[0][5] = *y + 2;
                    NearAvailable[1][5] = *x - 1;
                }
            }
        }
    } else
    {
        //Detectar casillas libres alrededor para fila con desfase
        //Para 0
        if (board[*y][*x - 1].isValid){
            if (!(board[*y][*x - 1].isFilled)){
                NearAvailable[0][0] = *y;
                NearAvailable[1][0] = *x - 1;
            }else{
                //Para a
                if (board[*y][*x - 2].isValid && board[*y][*x - 2].isFilled == false){
                    NearAvailable[0][0] = *y;
                    NearAvailable[1][0] = *x - 2;
                }
            }
        }
        //Para 1
        if (board[*y - 1][*x].isValid){
            if (board[*y - 1][*x].isFilled == false){
                NearAvailable[0][1] = *y - 1;
                NearAvailable[1][1] = *x;
            }else{
                //Para b
                if (board[*y - 2][*x - 1].isValid && board[*y - 2][*x - 1].isFilled == false){
                    NearAvailable[0][1] = *y - 2;
                    NearAvailable[1][1] = *x - 1;
                }
            }
        }
        //Para 2
        if (board[*y - 1][*x + 1].isValid){
            if (board[*y - 1][*x + 1].isFilled == false){
                NearAvailable[0][2] = *y - 1;
                NearAvailable[1][2] = *x + 1;
            }else
            {
                //Para c
                if (board[*y - 2][*x + 1].isValid && board[*y - 2][*x + 1].isFilled == false){
                    NearAvailable[0][2] = *y - 2;
                    NearAvailable[1][2] = *x + 1;
                }
            }
        }
        //Para 3
        if (board[*y][*x + 1].isValid){
            if (board[*y][*x + 1].isFilled == false){
                NearAvailable[0][3] = *y;
                NearAvailable[1][3] = *x + 1;
            }
            else
            {
                //Para d
                if (board[*y][*x + 2].isValid && board[*y][*x + 2].isFilled == false){
                    NearAvailable[0][3] = *y;
                    NearAvailable[1][3] = *x + 2;
                }
            }
        }
        //Para 4
        if (board[*y + 1][*x + 1].isValid){
            if (board[*y + 1][*x + 1].isFilled == false){
                NearAvailable[0][4] = *y + 1;
                NearAvailable[1][4] = *x + 1;
            }else
            {
                //Para e
                if (board[*y + 2][*x + 1].isValid && board[*y + 2][*x + 1].isFilled == false){
                    NearAvailable[0][4] = *y + 2;
                    NearAvailable[1][4] = *x + 1;
                }
            }
        }
        //Para 5
        if (board[*y + 1][*x].isValid){
            if (board[*y + 1][*x].isFilled == false){
                NearAvailable[0][5] = *y + 1;
                NearAvailable[1][5] = *x;
            }else{
                //Para f
                if (board[*y + 2][*x - 1].isValid && board[*y + 2][*x - 1].isFilled == false){
                    NearAvailable[0][5] = *y + 2;
                    NearAvailable[1][5] = *x - 1;
                }
            }
        }
    }
    
    
}

bool WantExtraMov(){
    char answer;
    do
    {
        fflush(stdin);
        cout << "Hay un movimiento extra disponible!\nDesea hacer un movimiento extra? S/N" << endl;
        cin >> answer;
    } while (answer != 'S' && answer != 's' && answer != 'N' && answer != 'n');
    if(answer != 'S' || answer != 's'){
        return true;
    }else{
        return false;
    }
}

void DetectExtraAvailableMoves(Box** board, int* x, int* y, int** NearAvailable){
     //    b  O  c              b   O   c
    // O  1  2  O            O   1   2   O
    // a  0  X  3  d        a  0   X   3   d
    // O  5  4  O            O   5   4   O
    //    f  O  e  O           f   O   e   O

    //Al inicio todas las casillas se consideran no disponibles
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 6; j++){
            NearAvailable[i][j] = -1;
        }      
    }
    
    //Para 0
    if (board[*y][*x - 1].isValid == false && board[*y][*x - 2].isValid == true && board[*y][*x - 2].isFilled == false){
        NearAvailable[0][0] = *y;
        NearAvailable[1][0] = *x - 2;
    }
    //Para 3
    if (board[*y][*x + 1].isValid == false && board[*y][*x + 2].isValid == true && board[*y][*x + 2].isFilled == false){
        NearAvailable[0][3] = *y;
        NearAvailable[1][3] = *x + 2;
    }
    if (*y % 2 == 0)
    {
        //Detectar casillas libres alrededor

        //Para 1
        if (board[*y - 1][*x - 1].isValid == false && board[*y - 2][*x - 1].isValid == true && board[*y - 2][*x - 1].isFilled == false){
            NearAvailable[0][1] = *y - 2;
            NearAvailable[1][1] = *x - 1;
        }
        //Para 2
        if (board[*y - 1][*x].isValid == false && board[*y - 2][*x + 1].isValid == true && board[*y - 2][*x + 1].isFilled == false){
            NearAvailable[0][2] = *y - 2;
            NearAvailable[1][2] = *x + 1;
        }
        //Para 4
        if (board[*y + 1][*x].isValid == false && board[*y + 2][*x + 1].isValid == true && board[*y + 2][*x + 1].isFilled == false){
            NearAvailable[0][4] = *y + 2;
            NearAvailable[1][4] = *x + 1;
        }
        //Para 5
        if (board[*y + 1][*x - 1].isValid == false && board[*y + 2][*x - 1].isValid == true && board[*y + 2][*x - 1].isFilled == false){
            NearAvailable[0][5] = *y + 2;
            NearAvailable[1][5] = *x - 1;
        }
    }else
    {
        //Detectar casillas libres alrededor
        //Para 1
        if (board[*y - 1][*x].isValid == false && board[*y - 2][*x - 1].isValid == true && board[*y - 2][*x - 1].isFilled == false){
            NearAvailable[0][1] = *y - 2;
            NearAvailable[1][1] = *x - 1;
        }
        //Para 2
        if (board[*y - 1][*x + 1].isValid == false && board[*y - 2][*x + 1].isValid == true && board[*y - 2][*x + 1].isFilled == false){
            NearAvailable[0][2] = *y - 2;
            NearAvailable[1][2] = *x + 1;
        }
        //Para 4
        if (board[*y + 1][*x + 1].isValid == false && board[*y + 2][*x + 1].isValid == true && board[*y + 2][*x + 1].isFilled == false){
            NearAvailable[0][4] = *y + 2;
            NearAvailable[1][4] = *x + 1;
        }
        //Para 5
        if (board[*y + 1][*x].isValid == false && board[*y + 2][*x - 1].isValid == true && board[*y + 2][*x - 1].isFilled == false){
            NearAvailable[0][5] = *y + 2;
            NearAvailable[1][5] = *x - 1;
        }
    }
}

bool CheckExtraMovement(int** availables){

    //Devuelve true si hay algun movimiento disponible
    for (int i = 0; i < 6; i++){
        if(availables[0][i] != -1 || availables[1][i] != -1)
        {
            return true;
        }
    }
    return false;
}

#pragma endregion
//void cambiarIDcolor(a su verdadera ID)

void updateIdColors(Player* arrayPlayers){

    for (int i = 0; i < 2; i++)
    {
        switch (arrayPlayers[i].corner)
        {
            case 0:
                arrayPlayers[i].colorID = 9; 
                break;
            case 1:
                arrayPlayers[i].colorID = 2; 
                break;
            case 2:
                arrayPlayers[i].colorID = 14; 
                break;
            case 3:
                arrayPlayers[i].colorID = 15; 
            break;
            case 4:
                arrayPlayers[i].colorID = 12; 
            break;
            case 5:
                arrayPlayers[i].colorID = 5; 
            break;
        }   
    }
}

int main(){
    showBanner();
	Credits();
    int idWinner;
    int counter[3]; // Turno, movJugador1, movsJugador2

    Player* players;
    players = new Player[2];

    players[0].id = 0;
    players[1].id = 1;

    players[0].token = '*';
    players[1].token = '#';

    Color* colors;
    colors = new Color[6];

    Box **board = new Box*[17];
    for(int i = 0; i < 17; i++){
        board[i]=new Box[17];
    }

    Coordinates **allTriangularGroupPositions = new Coordinates*[6];
    for(int i = 0; i < 6; i++){
        allTriangularGroupPositions[i]=new Coordinates[10];
    }

    Coordinates **playerStartingPositions = new Coordinates*[2];
    for(int i = 0; i < 2; i++){
        playerStartingPositions[i]=new Coordinates[10];
    }

    Coordinates **symmetricPositions = new Coordinates*[2];
    for(int i = 0; i < 2; i++){
        symmetricPositions[i]=new Coordinates[10];
    }


    showStarting();
    createBoard(board);
    registerPlayer(players);
    throwDice(players);
    getch();
    fillArrayColors(colors);

    system("cls");
    setCornerTriangularPositions(allTriangularGroupPositions);
    miniBoard();

    chooseTokenColor(players, colors);

    setStartingPositions(allTriangularGroupPositions, players[0], playerStartingPositions);
    setStartingPositions(allTriangularGroupPositions, players[1], playerStartingPositions);

    setBothSymmetricPositions(allTriangularGroupPositions, players, symmetricPositions);

    setFirstPositionsOnBoard(board, playerStartingPositions, players[0]);
    setFirstPositionsOnBoard(board, playerStartingPositions, players[1]);

    updateIdColors(players);
    
    for (int i = 0; i < 3; i++)
    {
        counter[i] = 0;
    }

	do
	{
        int* x;
        int* y;
        int** availables;// Matriz donde se guardan las coordenadas de los movimientos disponibles del jugador
        bool extra = false;// Verifica si hay movimientos extra

        x = new int;
        y = new int; 

    // Muestra el turno, al jugador que le toca mover, y el tablero
        system("cls");

        cout << "Turno " << ++counter[0] << endl;

        idWinner = (counter[0] + 1) % 2;

        cout << players[idWinner].name << ". Movimiento " << counter[idWinner + 1] << endl;

        showBoard(board, players);

        ChooseToken(board, idWinner, x, y);

    //se inicializa la matriz disponibles
        availables = new int* [2];
        for (int i = 0; i < 2; i++){
            availables[i] = new int[6];
            for (int j = 0; j < 6; j++)
            {
                availables[i][j] = -1;
            }        
        }
               
        
        DetectAvailableMoves(board, x, y, availables);
        do
        {
            //aumentar contador movimientos
            if(idWinner == 0){
                counter[1]++;
            }else {
                counter[2]++;
            }

            system("cls");
            
            cout << "Turno " << counter[0] << endl;

            cout << players[idWinner].name << ". Movimiento " << counter[idWinner + 1] << endl;

            ShowAvailableMoves(board, x, y, availables, players);

            selectMovement(board, x, y, availables); //AQUI CAMBIAN LAS COORDENADAS
            
            DetectExtraAvailableMoves(board, x, y, availables);
            extra = CheckExtraMovement(availables);
            if(extra == true){
                extra = WantExtraMov();
            }
        } while (extra);

        delete x;
        delete y;
        for (int i = 0; i < 2; i++)
            delete[] availables[i];  
        delete[] availables;
        
        getch();
	} while (!(isThereWinner(board, symmetricPositions, players, idWinner)));

    showWinner(players[idWinner]);
    return 0;
}