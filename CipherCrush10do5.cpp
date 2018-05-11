#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;

//tabuleiro, tamanho e pontuação
int screen [10][10];
int size, ciphermax;
int score_turn;

int fancyscreen = 0;    // 0 = nao,  1 =  sim
int print(void)
{    
    int x,y;
    cout << "\n";
    for(y=0;y<size;y++)
    {
        if (fancyscreen == 1){
            
            for(x=0;x<size;x++){
                cout << " " << "\t|\t";
            }
            
            cout << "\n";
            
            for(x=0;x<size;x++){
                cout << screen[x][y] << "\t|\t";
            }
            cout << "\n";
            for(x=0;x<size;x++){
                cout << " " << "\t|\t";
            }
            
            cout << "\n";
            for(x=0;x<size;x++){
                cout << "_______________";
            }
            cout << "\n";
        }
        else{
            for(x=0;x<size;x++)
            cout << screen[x][y];
            
            cout << "\n";

        }
        
    }
    
    return 0;
};

// função recursiva para encontrar os algarismos pontuantes, partindo do primeiro de uma linha de 3 já identificada.
// basicamente, ele parte de uma casa, analisando as circundantes uma à uma. ao encontrar mais uma casa pontuante, chama-se novamente a função. 
int deepsearch (int cipher, int cx, int cy, int d)
{
	int x,y,di;
	
	screen[cx][cy]=9;
	// "9" quer dizer: essa casa já foi visitada. futuramente substituir por "999".
	di=(d+3)%4;
	// "d" é o sentido da casa anterior para essa, "di" é o sentido dessa casa pra próxima. di assume os valores d-1,d e d+1 (d+2 seria voltar).
		
	while(di!=(d+2)%4)
	{
		switch(di)
		{
			case 0:
			x=1;
			y=0;
			break;
			
			case 1:
			x=0;
			y=1;
			break;
			
			case 2:
			x=-1;
			y=0;
			break;
			
			case 3:
			x=0;
			y=-1;
			break;
		};
		
		if (cx+x>=0 && cx+x<size && cy+y>=0 && cy+y<size)
			if (screen[cx+x][cy+y]==cipher)
			{
				score_turn++;
				deepsearch(cipher,cx+x,cy+y,di);	
			};
		
		di=(di+1)%4;
	};
	
	return 0;
}

//analisa o tabuleiro inteiro e dá a pontuação da rodada.
int analyse (void)
{
	srand(time(NULL));
	
	int x,y;
	score_turn=0;
	
	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			if (screen[x][y]!=9)
			{
				if (screen[x][y]==screen[x+1][y] && screen[x+1][y]==screen[x+2][y])
					{
						score_turn++;
						deepsearch(screen[x][y],x,y,0);
					};
				if (screen[x][y]==screen[x][y+1] && screen[x][y+1]==screen[x][y+2])
					{
						score_turn++;
						deepsearch(screen[x][y],x,y,1);
					};
			};
			
	return score_turn;
};

// realiza por completo a gravidade de uma coluna.
// se uma casa está vazia, caem uma casa os números de todas as casas acima. se não, verifica-se a casa acima.
int gravity (int x, int y)
{
	int z;
	
	while (screen[x][y]==9)
	{
		for (z=y;z>0;z--)
			screen[x][z]=screen[x][z-1];
			
		screen[x][0]=rand()%ciphermax;
	};
	
	if (y!=0)
		gravity (x,y-1);
	
};



int main (void)
{
	
	srand(time(NULL));
	size=5;
	ciphermax=6;
	// (tamanho do tabuleiro e variedade de entradas)
	
	int x,y,z;
	
	// definições iniciais
	for(x=0;x<10;x++)
		for(y=0;y<10;y++)
			screen[x][y]=111;	

	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			screen[x][y]=rand()%ciphermax;
			
	int buffer;
	
	do
	{
		buffer=analyse();
		for (x=0;x<size;x++)
			gravity(x,size);
	}
	while (buffer!=0);


	int ax,ay,bx,by,s;
	int score=0;
	int turn=0;
	do
	{
		turn++;
		cout << "turn:" << turn << " score:" << score << "\n";
		print();
		
		cout << "\nx:";
		cin >> ax;
		cin.ignore();
		cout << "y:";
		cin >> ay;
		cin.ignore();
		cout << "sentido:";
		cin >> s;
		cin.ignore();
		switch(s)
		{
			case 0:
			bx=ax+1;
			by=ay;
			break;
			
			case 1:
			bx=ax;
			by=ay+1;
			break;
			
			case 2:
			bx=ax-1;
			by=ay;
			break;
			
			case 3:
			bx=ax;
			by=ay-1;
			break;
		};
		
		buffer=screen[ax][ay];
		screen[ax][ay]=screen[bx][by];
		screen[bx][by]=buffer;
		
		print();
		getchar();
		cout << "\n";
		
		do
		{
			buffer=analyse();
			score=score+buffer;
			for (x=0;x<size;x++)
				gravity(x,size);
		} while (buffer!=0);
				
				
	} while (s!=4);
		
	return 0;	
};
