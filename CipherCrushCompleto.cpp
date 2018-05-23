#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;

// Tabuleiro, tamanho e pontuação da rodada.
int size, ciphermax;
int score_turn;
int fancyscreen = 0;    // 0 = nao,  1 =  sim

int edgecorrection(int n)
{ 
    if (n>=size || n<0) 
        n = max(0,min(n,size-1));
        
    return n;
};

class screen
{

public:
		
	int casa[10][10];
	int score_in_search_all;
	
	void print();
	void deep_search(int cipher, int x, int y, int dir);
	bool quick_search(int x, int y, int dir);
	int search_all();
	void gravity(int x, int y);
	void switch_cipher(int ax, int ay, int bx, int by);
	
};

void screen :: print()
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
                cout << casa[x][y] << "\t|\t";
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
            cout << casa[x][y];
            
            cout << "\n";

        }
        
    }
    
};

// Função recursiva em árvore para encontrar os algarismos pontuantes, partindo do primeiro de uma linha de 3 já identificada.
// Basicamente, ele parte de uma casa, analisando as circundantes uma à uma. Ao encontrar mais uma casa pontuante, chama-se novamente a função. 
void screen :: deep_search (int cipher, int x, int y, int dir)
{
	int x0,y0,dir_new;
	
	casa[x][y]=9;
	// "9" quer dizer: essa casa já foi visitada. futuramente substituir por "999".
	dir_new=(dir+3)%4;
	// "dir" é o sentido da casa anterior para essa, "dir_new" é o sentido dessa casa pra próxima.
	// dir_new assume os valores dir-1,dir e dir+1. em dir_new=dir+2, encerra-se esta ramificação do processo e volta-se uma camada.
		
	while(dir_new!=(dir+2)%4)
	{
		switch(dir_new)
		{
			case 0:
			x0=1;
			y0=0;
			break;
			
			case 1:
			x0=0;
			y0=1;
			break;
			
			case 2:
			x0=-1;
			y0=0;
			break;
			
			case 3:
			x0=0;
			y0=-1;
			break;
		};
		
		if (x+x0>=0 && x+x0<size && y+y0>=0 && y+y0<size)
			if (casa[x+x0][y+y0]==cipher)
			{
				score_in_search_all++;
				deep_search(cipher,x+x0,y+y0,dir_new);	
			};
		
		dir_new=(dir_new+1)%4;
	};
	
}

bool screen :: quick_search (int x, int y, int dir)
{
	int dir_new=(dir+3)%4;
	int x0,y0;
	
	while(dir_new!=(dir+2)%4)
	{
		switch(dir_new)
		{
			case 0:
			x0=1;
			y0=0;
			break;
			
			case 1:
			x0=0;
			y0=1;
			break;
			
			case 2:
			x0=-1;
			y0=0;
			break;
			
			case 3:
			x0=0;
			y0=-1;
			break;
		};
		
		if (x+2*x0>=0 && x+2*x0<size && y+2*y0>=0 && y+2*y0<size)
			if (casa[x][y]==casa[x+x0][y+y0] && casa[x][y]==casa[x+2*x0][y+2*y0] )
				return 1;
		
		dir_new=(dir_new+1)%4;
	};
	
	return 0;
};

// Analisa o tabuleiro inteiro e dá a pontuação da rodada.
int screen :: search_all ()
{
	
	int x,y;
	score_in_search_all=0;
	
	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			if (casa[x][y]!=9)
			{
				if (casa[x][y]==casa[x+1][y] && casa[x+1][y]==casa[x+2][y] && x+2<size)
					{
						score_in_search_all++;
						deep_search(casa[x][y],x,y,0);
					};
				if (casa[x][y]==casa[x][y+1] && casa[x][y+1]==casa[x][y+2] && y+2<size)
					{
						score_in_search_all++;
						deep_search(casa[x][y],x,y,1);
					};
			};
			
	return score_in_search_all;
};

// Realiza por completo a gravidade de uma coluna.
// Se uma casa está vazia, caem os dígitos (e vazios) de todas as casas acima, e checa-se novamente. Se não está vazia, verifica-se a casa acima.
void screen :: gravity (int x, int y)
{
	int z;
	
	while (casa[x][y]==9)
	{
		for (z=y;z>0;z--)
			casa[x][z]=casa[x][z-1];
			
		casa[x][0]=rand()%ciphermax;
	};
	
	if (y!=0)
		gravity (x,y-1);
	
};

void screen :: switch_cipher (int ax, int ay, int bx, int by)
{
	int buffer;
	buffer=casa[ax][ay];
	bx=edgecorrection(bx);
    by=edgecorrection(by);
	casa[ax][ay]=casa[bx][by];
	casa[bx][by]=buffer;	
};

// Esta função imprime um tabuleiro (from) sobre o outro (to).
void copy_screen (screen s_from, screen s_to)
{
	int x,y;
	for (x=0;x<size;x++)
		for (y=0;y<size;y++)
			s_to.casa[x][y]=s_from.casa[x][y];
}

int main (void)
{
	
	size=5;
	ciphermax=6;
	// Tamanho do tabuleiro e variedade de entradas.
	
	int x,y,z;
	screen screen0,screen1;

	// Definições iniciais
	for(x=0;x<10;x++)
		for(y=0;y<10;y++)
			screen0.casa[x][y]=111;	
	
	srand(time(NULL));
	
	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			screen0.casa[x][y]=rand()%ciphermax;
			
	int buffer,buffer2,buffer3;
	
	// Para garantir que não começa em um estado pontuante:	
	do
	{
		buffer=screen0.search_all();
		for (x=0;x<size;x++)
			screen0.gravity(x,size);
	} while (buffer!=0);


	int ax,ay,bx,by,dir;
	int score=0;
	int turn=0;
	int score_turn;
	int breaker;
	//CICLO DO JOGO:
	do
	{
		turn++;
		cout << "turn:" << turn << " score:" << score << "\n";
		
		// Recebe jogadas e verifica sua validade até receber uma jogoda válida.
		do
		{
			
			screen0.print();
			
			//COPYSCREEN
			for (x=0;x<size;x++)
				for (y=0;y<size;y++)
					screen1.casa[x][y]=screen0.casa[x][y];
					
	
			cout << "\nx:";
			cin >> ax;
			cin.ignore();
			cout << "y:";
			cin >> ay;
			cin.ignore();
			cout << "sentido:";
			cin >> dir;
			cin.ignore();
			
			switch(dir)
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
			
			screen1.switch_cipher(ax,ay,bx,by);
			screen1.print();
			getchar();
			
		} while( screen1.search_all()<3 );
		
		screen0.switch_cipher(ax,ay,bx,by);
		
		//Renova o tabuleiro, testando renovações até que alguma tenha algum movimento válido.
		breaker=0;
		do
		{
			//COPYSCREEN
			for (x=0;x<size;x++)
				for (y=0;y<size;y++)
					screen1.casa[x][y]=screen0.casa[x][y];
					
			score_turn=0;
			do
			{
				buffer=screen1.search_all();
				score_turn=score_turn+buffer;
				for (x=0;x<size;x++)
					screen1.gravity(x,size);

			} while (buffer!=0);
			
			for( ax=0 ; ax<size && breaker==0 ; ax++ )
				for( ay=0 ; ay<size && breaker==0 ; ay++ )
					for( dir=0 ; dir<4 && breaker==0 ; dir++ )
					{
						switch(dir)
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
						
						screen1.switch_cipher(ax,ay,bx,by);
						
						if ( screen1.quick_search(ax,ay,(dir+2)%4) || screen1.quick_search(bx,by,dir) )
							breaker=1;
							
						screen1.switch_cipher(ax,ay,bx,by);
					}
				
		} while (breaker==0);
		
		//COPYSCREEN	
		for (x=0;x<size;x++)
			for (y=0;y<size;y++)
				screen0.casa[x][y]=screen1.casa[x][y];
				
				
		score=score+score_turn;	
		
	} while (dir<5);
		
	return 0;	
};
