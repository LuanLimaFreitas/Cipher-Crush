#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;

// Tabuleiro, tamanho e pontuacao da rodada.
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
	int check_playability();
	void gravity(int x, int y);
	void switch_cipher(int ax, int ay, int bx, int by);
	
};

void screen :: print()
{    
    int x,y;
    
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
    
    cout << "\n";
    
};

// Funcao recursiva em arvore para encontrar os algarismos pontuantes, partindo do primeiro de uma linha de 3 ja identificada.
// Basicamente, ele parte de uma casa, analisando as circundantes uma a uma. Ao encontrar mais uma casa pontuante, chama-se novamente a funcao. 
void screen :: deep_search (int cipher, int x, int y, int dir)
{
	int x0,y0,dir_new;
	
	casa[x][y]=9;
	// "9" quer dizer: essa casa ja foi visitada.
	dir_new=(dir+3)%4;
	// "dir" eh o sentido da casa anterior para essa, "dir_new" eh o sentido dessa casa pra proxima.
	// dir_new assume os valores dir-1,dir e dir+1. Em dir_new=dir+2, encerra-se esta ramificação do processo e volta-se um nivel.
		
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

// Analisa o tabuleiro inteiro e da a pontuação da rodada.
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

int screen :: check_playability()
{
	
	int ax,ay,bx,by,dir;

	for( ax=0 ; ax<size ; ax++ )
		for( ay=0 ; ay<size ; ay++ )
			for( dir=0 ; dir<4 ; dir++ )
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
				
				switch_cipher(ax,ay,bx,by);
				
				if ( quick_search(ax,ay,(dir+2)%4) || quick_search(bx,by,dir) )
				{
					switch_cipher(ax,ay,bx,by);
					return 1;
				};
				
				switch_cipher(ax,ay,bx,by);
					
			}
			
	return 2;
};

// Realiza ate a exaustao a gravidade de uma coluna.
// Se uma casa esta vazia, caem os digitos (e vazios) de todas as casas acima, e checa-se novamente. Se nao esta vazia, verifica-se a casa acima.
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

// Esta funcao imprime um tabuleiro (from) sobre outro (to).
//NAO ESTA FUNCIONANDO?!
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
	
	int x,y,z,buffer;
	screen screen0,screen1;

	// Definicoes iniciais
	for(x=0;x<10;x++)
		for(y=0;y<10;y++)
			screen0.casa[x][y]=111;	
	
	srand(time(NULL));

	// Gera os valores das casas, garantindo que começa num estado jogavel e nao pontuante.	
	do
	{
		for(x=0;x<size;x++)
			for(y=0;y<size;y++)
				screen0.casa[x][y]=rand()%ciphermax;

		do
		{
			buffer=screen0.search_all();
			for (x=0;x<size;x++)
				screen0.gravity(x,size);
		} while (buffer!=0);
	} 
	while (screen0.check_playability()!=1);
	
	
	



	int ax,ay,bx,by,dir;
	int score=0;
	int turn=0;
	int score_turn;
	int breaker;
	//CICLO DO JOGO:
	do
	{
		turn++;
		cout << "turn:" << turn << " score:" << score << "\n\n";
		
		// Recebe jogadas e verifica sua validade ate receber uma jogoda valida.
		do
		{
			
			screen0.print();
			
			//COPYSCREEN
			for (x=0;x<size;x++)
				for (y=0;y<size;y++)
					screen1.casa[x][y]=screen0.casa[x][y];
					
	
			cout << "x:";
			cin >> ax;
			cin.ignore();
			cout << "y:";
			cin >> ay;
			cin.ignore();
			cout << "dir:";
			cin >> dir;
			cin.ignore();
			cout << "\n";
			
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
			
		} while( screen1.search_all()<3 );
		
		screen0.switch_cipher(ax,ay,bx,by);
		
		//Renova o tabuleiro, testando renovacoes ate que alguma tenha algum movimento valido.
		breaker=0;
		do
		{
			if(breaker==2)
				cout << "\nOps, nenhum movimento valido disponivel! Geraremos novos digitos.\n";
				
			//COPYSCREEN
			for (x=0;x<size;x++)
				for (y=0;y<size;y++)
					screen1.casa[x][y]=screen0.casa[x][y];
					
			score_turn=0;
			do
			{
				buffer=screen1.search_all();
				if(buffer!=0)
					screen1.print();
				score_turn=score_turn+buffer;
				for (x=0;x<size;x++)
					screen1.gravity(x,size);
				if(buffer!=0)
					screen1.print();

			} while (buffer!=0);
					
			breaker=screen1.check_playability();
				
		} while (breaker==2);
		
		//COPYSCREEN	
		for (x=0;x<size;x++)
			for (y=0;y<size;y++)
				screen0.casa[x][y]=screen1.casa[x][y];
				
				
		score=score+score_turn;	
		
	} while (dir<5);
		
	return 0;	
};
