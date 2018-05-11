#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;

//tabuleiro, tamanho e pontuação
int screen [10][10];
int size;
int score;

int print(void)
{
	cout << "\n";
	
	int x,y;
	for(y=0;y<size;y++)
	{
		for(x=0;x<size;x++)
			cout << screen[x][y];
			
		cout << "\n";
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
		cout << "\n estamos em (" << cx << "," << cy << "), d= " << d << ", testando di=" << di <<"\n";
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
				score++;
				cout << "\nsorte! score=" << score << ", entramos em x=" << cx+x << " y=" << cy+y <<"\n";
				deepsearch(cipher,cx+x,cy+y,di);	
			};
		
		di=(di+1)%4;
	};
	
	return 0;
}

int main (void)
{
	
	srand(time(NULL));
	
	size=5;
	int ciphermax=6;
	// (tamanho do tabuleiro e variedade de entradas)
	
	int x,y,z;
	
	// definições iniciais
	for(x=0;x<10;x++)
		for(y=0;y<10;y++)
			screen[x][y]=111;	

	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			screen[x][y]=rand()%ciphermax;	

	print();
	
	// operação de troca de valores entre duas casas:
	int ax,ay,bx,by,s;
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
	int buffer;
	buffer=screen[ax][ay];
	screen[ax][ay]=screen[bx][by];
	screen[bx][by]=buffer;
	
	print();
	getchar();
	
	// análise do tabuleiro:
	score=0;
	for(x=0;x<size;x++)
		for(y=0;y<size;y++)
			if (screen[x][y]!=9)
			{
				if (screen[x][y]==screen[x+1][y] && screen[x+1][y]==screen[x+2][y])
					{
						cout << "\nlinha horizontal identificada! x:" << x << " y:" << y << "\n"; 
						score++;
						deepsearch(screen[x][y],x,y,0);
					};
				if (screen[x][y]==screen[x][y+1] && screen[x][y+1]==screen[x][y+2])
					{
						cout << "\nlinha vertical identificada! x:" << x << " y:" << y << "\n"; 
						score++;
						deepsearch(screen[x][y],x,y,1);
					};
			};
			
	print();
	cout << "\nscore=" << score;	
	getchar();
	return 0;	
};
