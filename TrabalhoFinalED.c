/****************************************************
*  	Universidade Federal do Piauí - UFPI			*
*	Curso: Bacharelado em Ciência da Computação		*
*	Disciplina: Estrutura de Dados 1 - ED1      	*
*	Discente: André Macedo							*
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
	system("mode con: cols=49 lines=38");	//Comando para escolher um tamanho padrão da tela do promt de comando
	setlocale(LC_ALL, "portuguese");		//Comando para poder utilizar acentuação
	inicializarVetores();
	loadStoreApps();
	int resp;								//Variável de controla do menu

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
			default: printf("\nOpção inválida!!!\n"); break;
		}
	}while(resp != 0);

	return 0;
}