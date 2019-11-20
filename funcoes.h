#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
#include "tela.h"
#define TAM 20

//Não não

//------------------------------------------------> Structs <----------------------------------------------------------------
typedef struct{
	char nome[50];
	int tamanho;
	char status[50];
}tApps;

typedef struct{
	tApps app;
	int prox;		// O campo prox não será utilizado com a estrutura LLV
	int anterior;	// O campo anterior não será utilizado com as estruturas LLV, LLSE e PILHA
} No;

//---------------------------------------------> LLV <------------------------------------------------------------------------
typedef struct{
	No vetor[TAM];
	int IA, FA, IL, FL;
}LLV;

LLV vetStore;

//---------------------------------------------> LLSE <----------------------------------------------------------------------

typedef struct {
	No vetor[TAM];
	int disponivel;
	int IL;
}LLSE;

LLSE vetApps;

//----------------------------------------------------> LLDE <----------------------------------------------------------------

typedef struct{
	No vetor[TAM];
	int disponivel;
	int IL;
} LLDE;

LLDE vetRun;

//----------------------------------------------------> FILA <---------------------------------------------------------------
typedef struct{
	No vetor[3];
	int disponivel;
	int IL;
} FILA;

FILA vetFila;

//----------------------------------------------------> PILHA <--------------------------------------------------------------
typedef struct{
	No vetor[5];
	int disponivel;
	int IL;
	int abas;
	int cont;
} PILHA;

PILHA vetPilha;

//---------------------------------------------------------------------------------------------------------------------------

void ordena(No app, int vetor);
void myStore();
void myApps();
void myRun();
void browser();

void inicializarVetores(){
	int i;
	//LLV
	vetStore.IA = 0; vetStore.FA = TAM-1;		vetStore.IL = -1; vetStore.FL = -1;
	//LLSE
	vetApps.IL = -1; vetApps.disponivel = 0; vetApps.vetor[19].prox = -1;
	//LLDE
	vetRun.IL = -1; vetRun.disponivel = 0; vetRun.vetor[19].prox = -1;
	//FILA
	vetFila.IL = -1; vetFila.disponivel=0; vetFila.vetor[0].prox=1; vetFila.vetor[1].prox=2; vetFila.vetor[2].prox=-1;
	//PILHA
	vetPilha.IL = -1; vetPilha.disponivel = 0; vetPilha.cont=0; vetPilha.vetor[0].prox=1; vetPilha.vetor[1].prox=2; vetPilha.vetor[2].prox=3; vetPilha.vetor[3].prox=4; vetPilha.vetor[4].prox=-1; vetPilha.abas=0;
	
	for(i=0; i<TAM; i++){	//Inicialização dos campos status das structs LLV, LLSE e LLDE
		strcpy(vetStore.vetor[i].app.status, "Não instalado");
		strcpy(vetApps.vetor[i].app.status, "A executar");
		strcpy(vetRun.vetor[i].app.status, " ");				//O vetor LLDE é inicializado com nada pois não será mostrado
	}															//nada referente ao campo status dele				
	
	for(i=0; i<TAM-1; i++){				//Inicialização dos campos prox das structs
		vetApps.vetor[i].prox = i+1;
		vetRun.vetor[i].prox = i+1;
	}
}

void imprimirVetores(){
	int i;
	
	gotoxy(2, 35);
	printf("VetFila:\n\n");
	printf("IL: %d\tDisp: %d\n", vetFila.IL, vetFila.disponivel);
	for(i=vetFila.IL; i != -1; i=vetFila.vetor[i].prox){
		printf("i:(%d) [%d]{%-8s}[%d]\n", i, vetFila.vetor[i].anterior, vetFila.vetor[i].app.nome, vetFila.vetor[i].prox);
	}
	printf("\nVetPilha:\n\n");
	printf("IL: %d\tDisp: %d\n", vetPilha.IL, vetPilha.disponivel);
	for(i=vetPilha.IL; i != -1; i=vetPilha.vetor[i].prox){
		printf("i:(%d) {%-10s} [%d]\n", i, vetPilha.vetor[i].app.nome, vetPilha.vetor[i].prox);
	}
	getchar();
	getchar();
}

/*	Objetivo: carregar os apps para a loja a partir de um arquivo txt
	Parâmetros: void
	Retorno: void
*/
void loadStoreApps(){
	int i;
	FILE *arqApps;		//Ponteiro para o arquivo
	
	arqApps = fopen("Apps.txt", "r");		//Abrindo o arquivo em modo leitura
	
	if(arqApps == NULL){	//Caso ocorra algum erro na abertura do arquivo, encerra-se o programa
		printf("Error loading app store!\n\n");
		return;
	}
	for(i=0; i<TAM; i++){ 			//Carregando e ordenando os arquivos da loja
		fscanf(arqApps, "%s%d", vetStore.vetor[i].app.nome, &vetStore.vetor[i].app.tamanho);
   	   	ordena(vetStore.vetor[i], 1);	//O segundo parâmetro diz respeito ao vetor LLV, VetStore
	}
	
	fclose(arqApps); 		//Fechando o arquivo, pois ele não será mais utilizado
}

/*	Objetivo: Pegar o índice de um determinado app de um determinado vetor
	Parâmetros: Um app do tipo No, um Vetor, que é um int para determinar a partir de qual vetor que quero buscar a posição
	de um app, e uma Situação, que é o caso específico dentro de um vetor 
	Retorno: A função retorna o índice do app no vetor desejado caso esse app exista, ou retorna -1, caso não ache o app
*/
int getIndice(No app, int situacao, int vetor){
	int i;
	
	if(vetor == 1){		//LLV
		if(situacao == 1){	//Utilizada no inserir
			for(i=vetStore.IL; i<=vetStore.FL; i++){
				if(app.app.tamanho > vetStore.vetor[i].app.tamanho)
					continue;
				else
					return i;
			}
		return -1;
		}
		else if(situacao == 2){ 	//Pesquisar na vetStore pelo índice de um App
			for(i=vetStore.IL; i<=vetStore.FL; i++){
				if(strcmp(vetStore.vetor[i].app.nome, app.app.nome) == 0)
					return i;
			}
			return -1;
		}
	}
	else if(vetor == 2){		//LLSE
		if(situacao == 1){		//Pesquisar no vetor de apps instalados, vetApps pelo índice de um App
			for(i=vetApps.IL; i != -1; i = vetApps.vetor[i].prox){
				if(strcmp(vetApps.vetor[i].app.nome, app.app.nome) == 0){
					return i;
				}
			}
			return -1;
		}
		else if(situacao == 2){	//Pegar o último elemento do vetor dos apps instalados
			for(i=vetApps.IL; i != -1; i = vetApps.vetor[i].prox){
				if(vetApps.vetor[i].prox == -1){
					return i;
				}
			}
		}
	}
	else if(vetor == 3){		//LLDE
		if(situacao == 1){		//Pesquisar no vetor de apps em execução, vetR\un pelo índice de um App
			for(i=vetRun.IL; i != -1; i = vetRun.vetor[i].prox){
				if(strcmp(vetRun.vetor[i].app.nome, app.app.nome) == 0){
					return i;
				}
			}
			return -1;
		}
		else if(situacao == 2){		//Pegar o último elemento do vetor dos apps em execução
			for(i=vetRun.IL; i != -1; i = vetRun.vetor[i].prox){
				if(vetRun.vetor[i].prox == -1){
					return i;
				}
			}
		}
	}
	else if(vetor == 4){		//FILA
		if(situacao == 1){		//Pesquisar no vetor de apps que estã na fila de instalação, vetFila pelo índice de um App
			for(i=vetFila.IL; i != -1; i = vetFila.vetor[i].prox){
				if(strcmp(vetFila.vetor[i].app.nome, app.app.nome) == 0){
					return i;
				}
			}
			return -1;
		}
		else if(situacao == 2){		//Pegar o último elemento do vetor dos apps que estão na fila de instalação
			for(i=vetFila.IL; i != -1; i = vetFila.vetor[i].prox){
				if(vetFila.vetor[i].prox == -1){
					return i;
				}
		   	}
		}
	}
	
	return -1;
}

/*	Objetivo: Inserir os apps em um determinado vetor, e em uma determinada situaçãi desse vetor(início, meio ou fim)
	Parâmetros: Um app do tipo No, que representa o app que desejamos inserir em um vetor, um int vetor para determinar
	em qual vetor será inserido o app, e uma situação, que especifica em qual caso dentro do vetor que o app será inserido
	Retorno: void
*/
void inserir(No app, int vetor, int situacao){
	int i, indice;	//Indice é utilizado para a parte de alocação para LLSE, LLDE, FILA

	if(vetor == 1){ //LLV
		if(situacao == 1){ //Inserir no início
			if(vetStore.IL != vetStore.IA){		//Se o app não está no início do arranjo
				vetStore.IL--;
				vetStore.vetor[vetStore.IL] = app;
			}
		   	else{
				vetStore.FL++;
				for(i=vetStore.FL; i != vetStore.IL; i--){	//Após incrementar o FL, só realizar operações vetoriais
					vetStore.vetor[i] = vetStore.vetor[i-1];
				}
				vetStore.vetor[vetStore.IL] = app;
		   	}
		}
		else if(situacao == 2){ //Inserir no meio
			indice = getIndice(app, 1, 1);	//Pegando o índice a partir do vetir VetStore, situação 1
			
			if(((indice - vetStore.IL) < (vetStore.FL - indice)) && (vetStore.IL != vetStore.IA)){
				vetStore.IL--;
				for(i=0; i<indice; i++){
					vetStore.vetor[i] = vetStore.vetor[i+1];
				}
		   	}
		   	else{
				vetStore.FL++;
				for(i=vetStore.FL; i>indice; i--){
					vetStore.vetor[i] = vetStore.vetor[i-1];
				}
		   	}
		   	vetStore.vetor[indice] = app;
		}
		else if(situacao == 3){ //Inserir no fim
			if(vetStore.FL != vetStore.FA){
				vetStore.FL++;
				vetStore.vetor[vetStore.FL] = app;
		   	}
		   	else{
				for(i=vetStore.IL; i <= vetStore.FL; i++){
					vetStore.vetor[i-1] = vetStore.vetor[i-1];
				}
				vetStore.IL--;
				vetStore.vetor[vetStore.FL] = app;
		   	}
		}
	}
	else if(vetor == 2){		//LLSE
		indice = vetApps.disponivel;	//Alocação
		vetApps.disponivel = vetApps.vetor[vetApps.disponivel].prox;
		
		if(situacao == 1){	//Inserir no início
			vetApps.vetor[indice] = app;
			vetApps.vetor[indice].prox = vetApps.IL;
			vetApps.IL = indice;
		}
		else if(situacao == 2){	//Inserir no meio
			int indiceMeio;
	
			for(i=vetApps.IL; i != -1; i = vetApps.vetor[i].prox){
				if(vetApps.vetor[i].app.tamanho <= app.app.tamanho && vetApps.vetor[vetApps.vetor[i].prox].app.tamanho <= app.app.tamanho)
					continue;
				else{
					indiceMeio = i;
					break;
				}
			}
			vetApps.vetor[indice] = app;
			vetApps.vetor[indice].prox = vetApps.vetor[indiceMeio].prox;
			vetApps.vetor[indiceMeio].prox = indice;
		}
		else if(situacao == 3){	//Inserir no fim
			int indiceFim;
			indiceFim = getIndice(app, 2, 2);
			
			vetApps.vetor[indice] = app;
			vetApps.vetor[indice].prox = -1;
			vetApps.vetor[indiceFim].prox = indice;
		}
		strcpy(vetApps.vetor[indice].app.status, "A executar");
	}
	else if(vetor == 3){		//LLDE
		indice = vetRun.disponivel;	//Alocação
		vetRun.disponivel = vetRun.vetor[vetRun.disponivel].prox;
		
		if(situacao == 1){ 			//Inserir no início
			vetRun.vetor[indice] = app;
			vetRun.vetor[indice].prox = vetRun.IL;
			vetRun.vetor[indice].anterior = -1;
			vetRun.vetor[vetRun.IL].anterior = indice;
			vetRun.IL = indice;
		}
		else if(situacao == 2){ 	//Inserir no meio
			int indiceMeio;
		
			for(i=vetRun.IL; i != -1; i = vetRun.vetor[i].prox){
				if(vetRun.vetor[i].app.tamanho <= app.app.tamanho)
					continue;
				else{
					indiceMeio = i;
					break;
				}
			}
	
			vetRun.vetor[indice] = app;
			vetRun.vetor[indice].prox = indiceMeio;
			vetRun.vetor[indice].anterior = vetRun.vetor[indiceMeio].anterior;
			vetRun.vetor[vetRun.vetor[indiceMeio].anterior].prox = indice;
			vetRun.vetor[indiceMeio].anterior = indice;
		}
		else if(situacao == 3){ //Inserir no fim
			int indiceFim;
			
			indiceFim = getIndice(app, 2, 3);	//Pegando o índice do vetor dos instalados, vetRun, na situação 2
			
			vetRun.vetor[indice] = app;
			vetRun.vetor[indice].prox = vetRun.vetor[indiceFim].prox;
			vetRun.vetor[indice].anterior = indiceFim;
			vetRun.vetor[indiceFim].prox = indice;
		}
	}
}

/*	Objetivo: Como o próprio nome diz, ordenar os apps em um determinado vetor. Função responsável por chamar de forma correta
	a função excluir()
	
	Parâmetros: Um app do tipo No, que representa o app que desejamos ordenar em um vetor, um int vetor para determinar
	em qual vetor será inserido o app
	
	Retorno: void
*/
void ordena(No app, int vetor){
	if(vetor == 1){	// LLV
		if(vetStore.IA == vetStore.IL && vetStore.FA == vetStore.FL){ 	//Se o vetor estiver cheio...
			printf("Não é mais possível instalar algum app, pois o vetor está cheio!!!");
			return;
		}
		else if(vetStore.IL == -1 && vetStore.FL == -1){ 	//Se o vetor estiver vazio... Primeira inserção
			vetStore.IL++;
			vetStore.FL++;
			vetStore.vetor[vetStore.IL] = app;
			return;
		}
		if(app.app.tamanho <= (vetStore.vetor[vetStore.IL].app.tamanho)){	//Se o tamanho do app a ser inserido for menor
																		//que o app do início da lista
			inserir(app, 1, 1); //Inserir o app no início do vetor
		}
		else if(app.app.tamanho >= (vetStore.vetor[vetStore.FL].app.tamanho)){	//Se o tamanho do app a ser inserido for maior
																		//que o app do final da lista
			inserir(app, 1, 3); //Inserir o app no fim do vetor
		}
		else{
			inserir(app, 1, 2); //Inserir o app no meio do vetor
		}
	}
	else if(vetor == 2){	//LLSE
		int indiceFim;
				
		if(vetApps.IL == -1){ //Se não existe nenhum elemento no vetor, primeira inserção
			vetApps.IL++;
			vetApps.vetor[vetApps.disponivel] = app;
			vetApps.vetor[vetApps.disponivel].prox = -1;
			vetApps.disponivel++;
			strcpy(vetApps.vetor[vetApps.IL].app.status, "A executar");
			return;
		}
		else if(vetApps.disponivel == -1){ 	// Vetor cheio
			gotoxy(0,30);
			printf("|  Não é mais possível instalar apps    ");
			getchar();
			getchar();
			return;
		}
		
		indiceFim = getIndice(app, 2, 2);	//Pegando o índice do vetor dos apps, instalados, vetStore, na situação 2
		
		if(app.app.tamanho <= (vetApps.vetor[vetApps.IL].app.tamanho)){
			inserir(app, 2, 1); //Inserir o app no início do vetor
		}
		else if(app.app.tamanho >= (vetApps.vetor[indiceFim].app.tamanho)){
			inserir(app, 2, 3); //Inserir o app no fim do vetor
		}
		else{
			inserir(app, 2, 2); //Inserir o app no meio do vetor
		}
	}
	else if(vetor == 3){	//LLDE
		int indiceFim;
		
	   	indiceFim = getIndice(app, 2, 3);	//Pegando o índice do vetor dos apps em execução, vetRun, na situação 2
		
		if(vetRun.disponivel == -1){ 	//Se o vetor estiver cheio...
			gotoxy(0,30);
			printf("|        Vetor cheio!!!          ");
			getchar();
			getchar();
			return;
		}
		else if(vetRun.IL == -1){ 	//Se o vetor estiver vazio... Primeira inserção
			vetRun.IL++;
			vetRun.vetor[vetRun.disponivel] = app;
			vetRun.vetor[vetRun.disponivel].prox = -1;
			vetRun.vetor[vetRun.disponivel].anterior = -1;
			vetRun.disponivel++;
			return;
		}
		if(app.app.tamanho <= (vetRun.vetor[vetRun.IL].app.tamanho)){
			inserir(app, 3, 1); //Inserir o app no início do vetor
		}
		else if(app.app.tamanho >= (vetRun.vetor[indiceFim].app.tamanho)){
			inserir(app, 3, 3); //Inserir o app no fim do vetor
		}
		else{
			inserir(app, 3, 2); //Inserir o app no meio do vetor
		}
	}
}

/*	Objetivo: Como o próprio nome diz, excluir os apps em um determinado vetor, a partir de uma determinada  situação
	
	Parâmetros: Um int vetor para determinar em qual vetor será excluído o app, o índice do app a ser excluído e uma situação,
	que especifica o exclusão do app(início, meio ou fim)
	
	Retorno: void
*/
void excluir(int vetor, int indice, int situacao){
	if(vetor == 1){		//LLSE
		if(situacao == 1){			//Excluir do início		
			if(vetApps.vetor[indice].prox != -1){	//Se o app a ser excluído não for o único elemento no vetor
				vetApps.IL = vetApps.vetor[indice].prox;
				vetApps.vetor[indice].prox = vetApps.disponivel;
		   	   	vetApps.disponivel = indice;
			}
			else{	//Se o app a ser excluído for o único elemento no vetor
				int i;
				
				vetApps.IL = -1;			//Reinicialização dos apps instalados
				vetApps.disponivel = 0;
				vetApps.vetor[TAM-1].prox = -1;
				
				for(i=0; i<TAM-1; i++){
					vetApps.vetor[i].prox = i+1;
				}
			}
		}
		else if(situacao == 2){		//Excluir do meio
			int i, indiceMeio;
		
			for(i=vetApps.IL; i!= -1; i = vetApps.vetor[i].prox){	//For utilizado para pegar o índice do meio entre os apps
				if(vetApps.vetor[i].prox == indice){
					indiceMeio = i;
					break;
				}
			}
			
			vetApps.vetor[indiceMeio].prox = vetApps.vetor[indice].prox;
			vetApps.vetor[indice].prox = vetApps.disponivel;
		   	vetApps.disponivel = indice;
		}
		else if(situacao == 3){		//Excluir do fim
			int i, indiceFim;
		
			for(i=vetApps.IL; i!= -1; i = vetApps.vetor[i].prox){
				if(vetApps.vetor[vetApps.vetor[i].prox].prox == -1){	//Se o próximo do próximo do app a ser excluído for -1
					indiceFim = i;
					break;
				}
			}
			
			vetApps.vetor[indiceFim].prox = -1;
			vetApps.vetor[indice].prox = vetApps.disponivel;
		   	vetApps.disponivel = indice;
		}
	}
	else if(vetor == 2){		//LLDE
		if(situacao == 1){			//Excluir do início
			if(vetRun.vetor[indice].prox != -1){	//Se o app a ser excluído não for o único elemento no vetor
				vetRun.vetor[vetRun.vetor[vetRun.IL].prox].anterior = -1;
				vetRun.IL = vetRun.vetor[indice].prox;
				vetRun.vetor[indice].prox = vetRun.disponivel;
		   	   	vetRun.disponivel = indice;
			}
			else{		//Se o app a ser excluído for o único elemento no vetor
				int i;
				
				vetRun.IL = -1;			//Reinicialização do vetor dos apps em execução
				vetRun.disponivel = 0;
				vetRun.vetor[TAM-1].prox = -1;
				
				for(i=0; i<TAM-1; i++){
					vetRun.vetor[i].prox = i+1;
				}
			}
		}
		else if(situacao == 2){		//Excluir do meio			
			vetRun.vetor[vetRun.vetor[indice].anterior].prox = vetRun.vetor[indice].prox;
			vetRun.vetor[vetRun.vetor[indice].prox].anterior = vetRun.vetor[indice].anterior;
			vetRun.vetor[indice].prox = vetRun.disponivel;
		   	vetRun.disponivel = indice;
		}
		else if(situacao == 3){		//Excluir do fim
			vetRun.vetor[vetRun.vetor[indice].anterior].prox = vetRun.vetor[indice].prox;
			vetRun.vetor[indice].prox = vetRun.disponivel;
		   	vetRun.disponivel = indice;
		}
	}
}

/*	Objetivo: Tem como objetivo excluir de forma ordenada os apps em um determinado vetor. Função responsável por chamar de forma correta
	a função excluir()
	
	Parâmetros: Um int vetor para determinar em qual vetor será inserido o app e o índice do app a ser excluído
	
	Retorno: void
*/
void kill(int vetor, int indice){
	int indiceFim;
	
	if(vetor == 1){		//LLSE
		indiceFim = getIndice(vetApps.vetor[indice], 2, 2);	//Pegando o último elemento do vetor, situação 2
		
		if(vetApps.vetor[indice].app.tamanho <= (vetApps.vetor[vetApps.IL].app.tamanho)){
			excluir(1, indice, 1); //Excluir o app no início do vetor
		}
		else if(vetApps.vetor[indice].app.tamanho >= (vetApps.vetor[indiceFim].app.tamanho)){
			excluir(1, indice, 3); //Excluir o app no fim do vetor
		}
		else{
			excluir(1, indice, 2); //Excluir o app no meio do vetor
		}
	}
	else if(vetor == 2){		//LLDE
		indiceFim = getIndice(vetRun.vetor[indice], 2, 3);	//Pegando o último elemento do vetor
		
		if(vetRun.vetor[indice].app.tamanho <= (vetRun.vetor[vetRun.IL].app.tamanho)){
			excluir(2, indice, 1); //Excluir o app no início do vetor
		}
		else if(vetRun.vetor[indice].app.tamanho >= (vetRun.vetor[indiceFim].app.tamanho)){
			excluir(2, indice, 3); //Excluir o app no fim do vetor
		}
		else{
			excluir(2, indice, 2); //Excluir o app no meio do vetor
		}
	}
}

/*	Objetivo: Buscar as informações de um determinado app, e exibi-las para o usuário 
	
	Parâmetros: Um int situação para determinar em qual vetor o app será buscado, e um tamanho para a validação do ID
	
	Retorno: void
*/
void buscarApp(int situacao, int tamanho){
	int numApp, indice;
	
	gotoxy(0, 30);
   	printf("|                                         ");
   	gotoxy(0, 30);
   	printf("| ID para buscar:                              ");
	gotoxy(1,32);
	scanf("%d", &numApp);
	   	
	while(numApp < 1 || numApp > tamanho){	//Fixa a escolha do usuário aos números de apps exibidos
		gotoxy(0, 30);
	   	printf("| Digite um ID válido: ");
	   	gotoxy(1,32);
	   	scanf("%d", &numApp);
	}
	
	if(situacao == 1){	//Pesquisar dentre os apps da loja
		indice = getIndice(vetStore.vetor[numApp-1], 2, 1);	//Pegando o índice do vetor da loja, vetApps, na situação 2
		
		telaBuscar();	//Função responsável pela interface
		
		gotoxy(11, 9);
		printf(" ----> App buscado <----");
		gotoxy(9, 12);
		printf("Nome: %s", vetStore.vetor[indice].app.nome);
		gotoxy(9, 13);
		printf("Tamanho: %d", vetStore.vetor[indice].app.tamanho);
		gotoxy(9, 14);
		printf("Status: %s", vetStore.vetor[indice].app.status);
		
		getchar();
		getchar();
		system("cls");
		myStore();
	}
   	else if(situacao ==  2){	//Pesquisar dentre os Apps instalados
   		int i, j;
   		
   		for(i=vetApps.IL, j=0; j<numApp-1; i=vetApps.vetor[i].prox, j++){
			continue;
		}
		numApp = i;
   		
		indice = getIndice(vetApps.vetor[numApp], 1, 2);	//Pegando o índice do vetor dos instalados, vetStore, na situação 1
		
		telaBuscar();
		
		gotoxy(11, 9);
		printf(" ----> App buscado <----");
		gotoxy(9, 12);
		printf("Nome: %s", vetApps.vetor[indice].app.nome);
		gotoxy(9, 13);
		printf("Tamanho: %d", vetApps.vetor[indice].app.tamanho);
		gotoxy(9, 14);
		printf("Status: %s", vetApps.vetor[indice].app.status);
		
		getchar();
		getchar();
		system("cls");
	}
   	else if(situacao ==  3){	//Pesquisar dentre os Apps em execução
   		int i, j;
   		
   		for(i=vetRun.IL, j=0; j<numApp-1; i=vetRun.vetor[i].prox, j++){
			continue;
		}
		numApp = i;
   		
		indice = getIndice(vetRun.vetor[numApp], 1, 3); //Pegando o índice do vetor apps em execução, vetRun, na situação 1
		
		telaBuscar();
		
		gotoxy(11, 9);
		printf(" ----> App buscado <----");
		gotoxy(9, 12);
		printf("Nome: %s", vetRun.vetor[indice].app.nome);
		gotoxy(9, 13);
		printf("Tamanho: %d", vetRun.vetor[indice].app.tamanho);
		gotoxy(9, 14);
		printf("Status: %s", vetRun.vetor[indice].app.status);
		
		getchar();
		getchar();
		system("cls");
	}
}

/*	Objetivo: Mostrar todos os apps puxados dos arquivos, mostrando suas informações e status
	
	Parâmetros: void
	
	Retorno: void
*/
void myStore(){
	int i, j, resp;

	do{
		system("cls");
		tela();
		gotoxy(0, 4);
		printf("|            ------> Store <------              |\n");
		printf("|\t\t\t\t\t\t|\n");
		printf("| ID       Nome         Tamanho       Status    |\n");
		printf("|\t\t\t\t\t\t|\n");
		
		for(i=vetStore.IL, j=1; i<= vetStore.FL; i++, j++){	//Exibindo para o usuário os apps da loja
			printf("| %-2d     %-18s%-2d     %-13s\n", j, vetStore.vetor[i].app.nome, vetStore.vetor[i].app.tamanho, vetStore.vetor[i].app.status);
		}
		
		gotoxy(0, 30);
		printf("|     1 - Instalar         2 - Buscar");
		gotoxy(1,32);
		scanf("%d", &resp);
		
		switch(resp){
			case 1:{	// Instalar app
				int numApp, indice, indiceApp, indiceInstall;
				static int aux=0;
				
				gotoxy(0, 30);
			   	printf("| ID para instalar:                  ");
			   	gotoxy(1,32);
			   	scanf("%d", &numApp);
			   	while(numApp < 1 || numApp > TAM){	//Validação da entrada do usuário
					gotoxy(0, 30);
				   	printf("| Digite um ID válido: ");
				   	gotoxy(1,32);
				   	scanf("%d", &numApp);
				}
				indice = getIndice(vetStore.vetor[numApp-1], 2, 1);		//Pegando o índice do de um app dp vetor vetStore
				
				if(indice < 0){	//Verifica se o app não está instalado
					gotoxy(0, 30);
				   	printf("| App nao instalado.                            |");
				   	getchar();
				   	getchar();
				   	break;
				}

				if((getIndice(vetStore.vetor[indice], 1, 4) >= 0) || (getIndice(vetStore.vetor[indice], 1, 2) >= 0)){
					gotoxy(0, 30);			//Se o app já está na fila de instalação, ou se está instalado, ele não poderá prosseguir
				   	printf("|        App ja na fila ou ja instalado     ");
				   	getchar();
				   	getchar();
				   	break;
				}
				if(vetFila.disponivel == -1){ // Se a Fila estiver cheia... Então o primeiro app inserido na fila irá para o vetor
					int indiceFim;	//dos apps instalados e entrará outro app no final da fila, ficando nesse processo de loop
				
					indiceFim = getIndice(vetFila.vetor[0], 2, 4);	//Pegando o índice do último app da fila
					indiceApp = getIndice(vetFila.vetor[indiceFim], 2, 1);	//Pegando o índice respectivo do app para alterar seu campo status
					strcpy(vetStore.vetor[indiceApp].app.status, "Instalado");
					vetFila.vetor[vetFila.vetor[indiceFim].anterior].prox = -1;	//Retirando da fila o primeiro app inserido nela
					vetFila.disponivel = indiceFim;
					ordena(vetFila.vetor[indiceFim], 2);	//Enviando o app para ser instalado no vetor vetApps, LLSE
					aux=1;
				}
				
				indiceInstall = vetFila.disponivel;	//Alocação
				vetFila.disponivel = vetFila.vetor[vetFila.disponivel].prox;
				
				if(vetFila.IL == -1){ // Primeira inserção
					vetFila.IL=0;
					vetFila.vetor[indiceInstall] = vetStore.vetor[indice];
					vetFila.vetor[indiceInstall].prox = -1;
					vetFila.vetor[indiceInstall].anterior = -1;
					
					strcpy(vetStore.vetor[indice].app.status, "Na fila");
					gotoxy(0, 30);
					printf("|  App \"%s\" na fila para instalação...  ", vetFila.vetor[indiceInstall].app.nome);
				}
				else{ //Inserindo apps no início da FILA
					vetFila.vetor[indiceInstall] = vetStore.vetor[indice];
					vetFila.vetor[indiceInstall].prox = vetFila.IL;
					vetFila.vetor[indiceInstall].anterior = -1;
					vetFila.vetor[vetFila.IL].anterior = indiceInstall;
					vetFila.IL = indiceInstall;
	
					gotoxy(0, 30);
					printf("|  App \"%s\" na fila para instalação...  ", vetFila.vetor[indiceInstall].app.nome);
					strcpy(vetStore.vetor[indice].app.status, "Na fila");
					
					if(aux == 1){	//Exibindo uma mensagem ao usuário que o app que estava no início da fila foi instalado
						telaCarregamento(vetStore.vetor[indiceApp].app.nome, "Instalando", "instalado");
					}
				}
				getchar();
				getchar();
			}
			break;
			
			case 2:{	// Buscar app
				buscarApp(1, TAM);
			}
			break;
		
			case 0: return;
			
			default:{
				gotoxy(0,30);
				printf("|               Opção inválida!          ");
				getchar();
				getchar();
			}
		}
	}while (resp != 0);
}

/*	Objetivo: Mostrar todos os apps instalados pelo usuário, mostrando suas informações e status 
além de umas opções a se fazer com esses apps
	
	Parâmetros: void
	
	Retorno: void
*/
void myApps(){
	int i, j, resp;

	do{
		if(vetApps.IL == -1){	//Se não existem apps instalados
			system("cls");
			tela();
			gotoxy(0,30);
			printf("|         Não existem apps instalados!          ");
			getchar();
			getchar();
			return;
		}
		
		system("cls");
		tela();
		gotoxy(0, 4);
		printf("|           ------> My Apps <------             |\n");
		printf("|\t\t\t\t\t\t|\n");
		printf("| ID       Nome         Tamanho       Status    |\n");
		printf("|\t\t\t\t\t\t|\n");
		
		for(i=vetApps.IL, j=1; i != -1; i = vetApps.vetor[i].prox, j++){	//Imprimindo os apps instalados
			printf("| %-2d     %-18s%-2d     %-13s\n", j, vetApps.vetor[i].app.nome, vetApps.vetor[i].app.tamanho, vetApps.vetor[i].app.status);
		}
		
		gotoxy(0, 30);
		printf("|  1 - Executar   2 - Desinstalar   3 - Buscar  |");	//Menu de opções para o usuário
		gotoxy(1,32);
		scanf("%d", &resp);
		
		switch(resp){
			case 1:{	//Executar app
				int indice, numApp;
				
				gotoxy(0, 30);
			   	printf("| ID para executar:                           ");
			   	gotoxy(1,32);
			   	scanf("%d", &numApp);
			   	while(numApp < 1 || numApp > j-1){
					gotoxy(0, 30);
				   	printf("| Digite um ID válido:                          |");
				   	gotoxy(1,32);
				   	scanf("%d", &numApp);
				}
				
				for(i=vetApps.IL, j=0; j<numApp-1; i=vetApps.vetor[i].prox, j++){	//Validação do índice
					continue;
				}
				numApp = i;
				
				indice = getIndice(vetApps.vetor[numApp], 1, 2);	//Pegando o índice do app no vetor dos instaldos, vetApps
				
				if(indice < 0){	//Verifica se o app não está instalado
					gotoxy(0, 30);
				   	printf("| App nao instalado.                            |");
				   	getchar();
				   	getchar();
				   	break;
				}
				else if(getIndice(vetApps.vetor[indice], 1, 3) >= 0){	//Verifica se o app já está sendo executado
					gotoxy(0, 30);
				   	printf("|        App já está em execução         ");
				   	getchar();
				   	getchar();
				   	break;
				}
				
				strcpy(vetApps.vetor[indice].app.status, "Em execução");
				ordena(vetApps.vetor[indice], 3);	//enviando o app para ser colocado ordenadamento no vetor dos apps em execução
				
				gotoxy(0, 30);
				printf("|          App \"%s\" em execucao          ", vetApps.vetor[indice].app.nome);
				
				getchar();
				getchar();
			}
			break;
		
			case 2:{	//Desinstalar app
				int indice, indiceAux, indiceRun, numApp;
				
				gotoxy(0, 30);
			   	printf("| ID para desinstalar:                         ");
			   	gotoxy(1,32);
			   	scanf("%d", &numApp);
			   	while(numApp < 1 || numApp > j-1){	//Validação para o usuário
					gotoxy(0, 30);
				   	printf("| Digite um ID válido:                          |");
				   	gotoxy(1,32);
				   	scanf("%d", &numApp);
				}
				
				for(i=vetApps.IL, j=0; j<numApp-1; i=vetApps.vetor[i].prox, j++){
					continue;
				}
				numApp = i;
				
				indice = getIndice(vetApps.vetor[numApp], 1, 2);		//Pegando o índice do vetor vetApps
				indiceAux = getIndice(vetApps.vetor[numApp], 2, 1);		//Pegando o índice do vetor vetStore
				
				if(indice < 0){	//Verifica se o app não está instalado
					gotoxy(0, 30);
				   	printf("| App não está instalado.                       |");
				   	getchar();
				   	getchar();
				   	break;
				}
				
				
				if((indiceRun = getIndice(vetApps.vetor[indice], 1, 3)) >= 0){	//Se o app a ser desintalado também está em execução
					if(strcmp(vetApps.vetor[indice].app.nome, "Chrome") == 0){	//Se o Browser for parado, então a pilha deve ser esvaziada
						vetPilha.IL = -1; vetPilha.disponivel = 0; vetPilha.cont=0; vetPilha.vetor[0].prox=1; vetPilha.vetor[1].prox=2; vetPilha.vetor[2].prox=3; vetPilha.vetor[3].prox=4; vetPilha.vetor[4].prox=-1; vetPilha.abas=0;
					}
					strcpy(vetApps.vetor[indice].app.status, "A executar");
					kill(2, indiceRun);		//Enviando o app para ser excluído de maneira ordenada
				}
				
				gotoxy(0, 30);
				printf("|           App \"%s\" desinstalado          ", vetApps.vetor[indice].app.nome);
				strcpy(vetStore.vetor[indiceAux].app.status, "Não instalado");
				telaCarregamento(vetApps.vetor[indice].app.nome, "Desinstalando", "desinstalado");
				kill(1, indice); //Enviando o app para ser excluído de maneira ordenada
				
				getchar();
				getchar();
			}
			break;
			
			case 3:{	//Buscar app
				buscarApp(2, j-1);
			}
			break;

			case 0: return;
			
			default:{
				gotoxy(0,30);
				printf("|               Opção inválida!          ");
				getchar();
				getchar();
			}
		}
	}while(resp != 0);
}

/*	Objetivo: Mostrar todos os apps colocados em execução pelo usuário, mostrando suas informações e status 
além de umas opções a se fazer com esses apps
	
	Parâmetros: void
	
	Retorno: void
*/
void myRun(){
	int i, j, resp;
	
	do{
		if(vetRun.IL == -1){	//Se não existem apps em execução
			system("cls");
			tela();
			gotoxy(0,30);
			printf("|      Não existem apps em execução!         ");
			getchar();
			getchar();
			return;
		}
		
		system("cls");
		tela();
		gotoxy(0, 4);
		printf("|           ------> My Run <------              |\n");
		printf("|\t\t\t\t\t\t|\n");
		printf("|     ID          Nome           Tamanho        |\n");
		printf("|\t\t\t\t\t\t|\n");
		
		for(i=vetRun.IL, j=1; i != -1; i = vetRun.vetor[i].prox, j++){	//Imprimindo os apps em execução
			printf("|     %-2d        %-18s   %-2d\n", j, vetRun.vetor[i].app.nome, vetRun.vetor[i].app.tamanho);
		}
		
		gotoxy(0, 30);
		printf("|  1 - Parar app    2 - Buscar    3 - Browser   |");
		gotoxy(1,32);
		scanf("%d", &resp);
		
		switch(resp){
			case 1:{	//Parar app
				int indice, indiceAux, numApp;
				
				gotoxy(0, 30);
			   	printf("| ID para parar:                              ");
			   	gotoxy(1,32);
			   	scanf("%d", &numApp);
			   	while(numApp < 1 || numApp > j-1){	//Validação da entrada prara o usuário
					gotoxy(0, 30);
				   	printf("| Digite um ID válido:                          |");
				   	gotoxy(1,32);
				   	scanf("%d", &numApp);
				}
				
				for(i=vetRun.IL, j=0; j<numApp-1; i=vetRun.vetor[i].prox, j++){
					continue;
				}
				numApp = i;
				
				indice = getIndice(vetRun.vetor[numApp], 1, 3);	//Pegando o índice do vetor dos em execução, vetRun
				indiceAux = getIndice(vetRun.vetor[numApp], 1, 2);	//Pegando o índice do vetor dos instalados, vetApps
				
				if(indice < 0){		//Verifica se o app não está instalado
					gotoxy(0, 30);
				   	printf("| App nao esta em execucao.                     |");
				   	getchar();
				   	getchar();
				   	break;
				}
				
				if(strcmp(vetRun.vetor[indice].app.nome, "Chrome") == 0){	//Se o Browser for parado, então a pilha deve ser esvaziada e reinicializada
					vetPilha.IL = -1; vetPilha.disponivel = 0; vetPilha.cont=0; vetPilha.vetor[0].prox=1; vetPilha.vetor[1].prox=2; vetPilha.vetor[2].prox=3; vetPilha.vetor[3].prox=4; vetPilha.vetor[4].prox=-1; vetPilha.abas=0;
				}
				
				gotoxy(0, 30);
				printf("| App \"%s\" parado                   ", vetRun.vetor[indice].app.nome);
				strcpy(vetApps.vetor[indiceAux].app.status, "A executar");
				telaCarregamento(vetRun.vetor[indice].app.nome, "Parando", "parado");
				kill(2, indice);	//Enviando o app para ser ecluído de maneira ordenada
				
				getchar();
				getchar();
			}
			break;
			
			case 2:{	//Buscar app
				buscarApp(3, j-1);
			}
			break;
		
			case 3:{	//Navegador padrão (Chrome)
				browser();
			}
			break;

			case 0: return;
			
			default:{
				gotoxy(0,30);
				printf("|               Opção inválida!          ");
				getchar();
				getchar();
			}
		}
	}while(resp != 0);
}

/*	Objetivo: Mostrar um funcionamento semelhante ao de um navegador que utilizamos no dia-a-dia, onde é possível abrir
	novas abas e fechar as abas, tudo implementado seguindo o conceito de PILHA (FILO).
	
	Parâmetros: void
	
	Retorno: void
*/
void browser(){
	int i, k, indice, resp, indiceApp, aux;
				
	for(i=vetRun.IL; i!= -1; i=vetRun.vetor[i].prox){	//Verificação se o Browser está instalado
		if(strcmp(vetRun.vetor[i].app.nome, "Chrome") == 0){
			indiceApp = i;
			aux=0;
			break;
		}
		else aux=1;
	}
	if(aux == 1){
		gotoxy(0,30);
		printf("|        Browser padrão nao instalado!        ");
		getchar();
		getchar();
		return;
	}
	do{
		system("cls");
		tela();
	  	gotoxy(0, 4);
	   	printf("|           ------> Browser <------         \n");
		printf("|\t\t\t\t\t\t|\n\n");
		
		if(vetPilha.abas <= 0){		//Se nãoo existirem abas abertas
			gotoxy(4, 27);
			printf("      Não existem abas abertas!        ");
		}
		else{
			for(i=vetPilha.IL, k=1; i != -1; i = vetPilha.vetor[i].prox, k++){
			   	printf("|   %d.  Aba aberta!   \n\n", k);
			}
			gotoxy(5,27);
			printf("     Total de abas abertas: %d       ", vetPilha.abas);
		}
		
		gotoxy(0, 30);
		printf("|     1 - Abrir aba       2 - Fechar aba      ");
		gotoxy(1,32);
		scanf("%d", &resp);
			
		switch(resp){
			case 1:{		//Abrir uma nova aba
				if(vetPilha.cont >= 0 && vetPilha.cont <= 5){	// Condição usada para validar a alocação
					indice = vetPilha.disponivel;		//Alocação
				  	vetPilha.disponivel = vetPilha.vetor[vetPilha.disponivel].prox;
				  	++vetPilha.cont;
				}
		   		else{
					gotoxy(0,30);
					printf("|     Não é mais possível abrir abas        ");
					getchar();
					getchar();
					break;  
				}
				
				if(indice == -1){ // Vetor cheio
					gotoxy(0,30);
					printf("|     Não é mais possível abrir abas        ");
					getchar();
					getchar();
					break;
				}
				
				if(vetPilha.IL == -1){ //Se não existe nenhum elemento no vetor, primeira inserção
					vetPilha.IL=0;
					vetPilha.vetor[indice] = vetRun.vetor[indiceApp];
					vetPilha.vetor[indice].prox = -1;
					++vetPilha.abas;
					
					gotoxy(0,30);
					printf("|          Aba aberta com sucesso!         ");
					getchar();
				   	getchar();
					break;
				}
				else{
			  		vetPilha.vetor[indice] = vetRun.vetor[indiceApp];
				   	vetPilha.vetor[indice].prox = vetPilha.IL;
				   	vetPilha.IL = indice;
			   		++vetPilha.abas;
			   		
					gotoxy(0,30);
					printf("|          Aba aberta com sucesso!         ");
					getchar();
					getchar();
					break;
				}
			}
			break;
		
			case 2:{	// Fechar uma aba
				int aux;
				if(vetPilha.abas <= 0){
					gotoxy(0,30);
					printf("|         Não existem abas abertas!          ");
					getchar();
					getchar();
					break;
				}
				
				aux = vetPilha.IL;
				vetPilha.IL = vetPilha.vetor[vetPilha.IL].prox;
			   	vetPilha.vetor[aux].prox = vetPilha.disponivel;
			   	vetPilha.disponivel = aux;
			   	--vetPilha.abas;
			   	--vetPilha.cont;
			   	
			  	if(vetPilha.disponivel == 0){
				   vetPilha.IL = -1; vetPilha.disponivel = 0; vetPilha.vetor[0].prox=1; vetPilha.vetor[1].prox=2; vetPilha.vetor[2].prox=3; vetPilha.vetor[3].prox=4; vetPilha.vetor[4].prox=-1; vetPilha.abas=0;
				}
			   	
				gotoxy(0,30);
				printf("|         Aba fechada com sucesso!         ");
				getchar();
				getchar();
			}
			break;
		
			case 0: return;
				
			default:{
				gotoxy(0,30);
				printf("|               Opção inválida!          ");
				getchar();
				getchar();
			}
		}
	}while(resp != 0);
}

/*	Objetivo: Imprimir o vetor de apps instalados na área de trabalho, para que o usuário veja os apps q instalou na tela inicial
	
	Parâmetros: void
	
	Retorno: void
*/
void desktop(){
	int i, x, y, aux=0, cont=0;
	
	x=5, y=4;	//X e Y são utilizados para manter a ordem dos elementos, simulando uma matriz
	for(i=vetApps.IL; i != -1; i=vetApps.vetor[i].prox){
		gotoxy(y, x);
		printf("%-s", vetApps.vetor[i].app.nome);
		++aux;
		++cont;
		y+=14;
		if(cont == 3 || cont == 6 || cont == 9 || cont == 12 || cont == 15){
			x += 4;
			y=4;
		}
	}
}