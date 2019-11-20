/****************************************************
*  	Universidade Federal do Piau� - UFPI			*
*	Curso: Bacharelado em Ci�ncia da Computa��o		*
*	Disciplina: Estrutura de Dados 1 - ED1      	*
*	Discente: Andr� Macedo							*
*	Aluno: Hilderlan da Silva Almeida				*
* 													*
* 				Trabalho final						*
*													*
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
#include "funcoes.h"

int main(int argc, char** argv)
{
	system("mode con: cols=49 lines=38");	//Comando para escolher um tamanho padr�o da tela do promt de comando
	setlocale(LC_ALL, "portuguese");		//Comando para poder utilizar acentua��o
	inicializarVetores();
	loadStoreApps();
	int resp;								//Vari�vel de controla do menu

	do{
		system("cls");
		tela();
		desktop();
		resp = menuDeOpcoes();
		switch(resp){
			case 1: myStore(); break;
			case 2: myApps(); break;
			case 3: myRun(); break;
			case 4: imprimirVetores(); break;
			case 0:{
				system("cls");
				tela();
				gotoxy(3,13);
				printf("    Obrigado por utilizar o Mobile ED!\n\n\n\n\n");
				printf("\t\t  Goodbye!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); return 0;
			}
			default: printf("\nOp��o inv�lida!!!\n"); break;
		}
	}while(resp != 0);

	return 0;
}