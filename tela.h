#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y){
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){
		x, y
	});
}

void tela(){
	int i;
	
	printf("_________________________________________________\n");
	printf("|                ==========      º    <O>       |\n");
	printf("|-----------------------------------------------|\n");
	for(i=0; i<26; i++){
		printf("|\t\t\t\t\t\t|\n");
	}
	printf("|-----------------------------------------------|\n");
	printf("|\t\t\t\t\t\t|\n");
	printf("|_______________________________________________|\n");
	printf("|\t\t\t\t\t\t|\n");
	printf("|        <             O             []         |\n");
	printf("|_______________________________________________|\n");
}

int menuDeOpcoes(){
	int resp;
	
	gotoxy(0, 30);
	printf("|   1 - Store     2 - My Apps      3 - Run      |\n");
	gotoxy(1,32);
	scanf("%d", &resp);
	
	return resp;
}

void telaBuscar(){
	int i;
	
	gotoxy(5,7);
	printf("____________________________________\n");
	for(i=8; i<16; i++){
		gotoxy(5,i);
		printf("|                                  |\n");
	}
	gotoxy(5, 16);
	printf("|__________________________________|\n");
}

void telaCarregamento(char nomeApp[50], char mensagem1[50], char mensagem2[50]){
	int i;
	
	gotoxy(5,10);
	printf("____________________________________\n");
	for(i=11; i<20; i++){
		gotoxy(5,i);
		printf("|                                  |\n");
	}
	gotoxy(5, 20);
	printf("|__________________________________|\n");
	
	gotoxy(11, 12);
  	printf("%s... Aguarde", mensagem1);

	gotoxy(7, 15);
	for(i=0; i<29; i++){
		Sleep(10);
		printf("%c", 411);	//178
	}
	gotoxy(9, 18);
	printf("   App %s %s.", nomeApp, mensagem2);
//	Sleep(1000);
	return;
}