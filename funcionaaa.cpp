#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

/*
Instruções:
PARTE 1:
	- Implementar a possibilidade de o usuário criar e armazenar usando arquivos em disco várias listas de palavras.
	- Permitir ao usuário a modificação ou inclusão de uma nova lista.
	- Você pode usar um menu para a escolha das opções do usuário.
	- As palavras não poderão ter acentos ou caracteres especiais (como ç, por exemplo).
	- As listas de palavras deverão ser armazenadas em vetores de estruturas (structs).
	- O arquivo deverá ser salvo em formato CSV, separado por virgulas. Cada linha conterá uma lista de palavras.
	- Quando o seu programa iniciar a execução, o arquivo deve ser lido e as listas armazenadas em memória.
	- Quando o usuário escolher a opção fim do seu programa, a sua lista em memória deverá ser salva novamente em
	  disco no mesmo formato de antes, sobrescrevendo o arquivo original
*/

#define tam_maximo 5

struct Lista {
	string palavras[tam_maximo]; //cada vetor representa uma linha do arquivo csv
	int tam = 0; //quantas posições do vetor realmente estão sendo usadas
	bool deletar = false;
};

int opcao_invalida(string frase, int min, int max) {
	int op;
	do {
		cout << frase << " (" << min << " - " << max << "): ";
		cin >> op;
		if (op < min || op > max) {
			cout << "\nOpcao invalida. Tente novamente." << endl;
			//Sleep(500);
		}
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			op = min - 1;
		}
	} while (op < min || op > max);
	return op;
}

template<typename T>
T* aumentar_vetor(T* listas, int& tam, int pos) {
	T* aux = new T[tam + pos];
	for (int i = 0; i < tam; i++) {
		aux[i] = listas[i];
	}
	delete[]listas;
	tam += pos; //sempre aumenta o tamanho de um em um
	return aux;
}

int contar_linhas_arquivo() {
	ifstream arquivo;
	arquivo.open("listaPalavras.csv"); // Abre o arquivo para leitura
	int contador = 0;
	string linha;

	if (arquivo.is_open()) {
		while (getline(arquivo, linha)) { // Lê cada linha do arquivo
			contador++;
		}
		arquivo.close(); // Fecha o arquivo
		return contador;
	}
	else return 0;
}

void escrever_nova_lista(Lista listas[], int tam) {
	int cont = 0;
	listas[tam - 1].tam = opcao_invalida("Informe quantas palavras deseja escrever", 1, 10);

	do {
		cout << "\n\t" << cont + 1 << ") ";
		cin >> listas[tam - 1].palavras[cont];
		cont++;
	} while (cont < listas[tam - 1].tam);

	cout << "\nPalavras adcionadas a lista com suceso!" << endl;
}

void mostrar_listas(Lista listas[], int tam) {
	for (int i = 0; i < tam; i++) {
		cout << "\n\tLista " << i + 1 << ": ";
		for (int j = 0; j < listas[i].tam; j++) {
			cout << listas[i].palavras[j] << " + ";
		}
	}
}

void escrever_no_arquivo(Lista listas[], int tam) {
	ofstream arquivo;
	arquivo.open("listaPalavras.csv");
	if (arquivo.is_open()) {
		for (int i = 0; i < tam; i++) {
			if (!listas[i].deletar) { //só salva no arquivo as listas onde o deletar for false
				for (int j = 0; j < listas[i].tam; j++) {
					arquivo << listas[i].palavras[j] << ",";
				}
			}
			cout << "\n";
		}
		arquivo.close();
	}
	else cout << "\nErro ao abrir o arquivo\n";
}

void pegar_listas_do_arquivo(Lista listas[], int tam) {
	ifstream arquivo;
	arquivo.open("listaPalavras.csv");
	string aux;

	if (arquivo.is_open()) {
		for (int i = 0; i < tam; i++) {
			for (int j = 0; j < tam_maximo; j++) {
				arquivo >> listas[i].palavras[j];
				listas[i].tam = j + 1;
			}
		}
		arquivo.close();
	}
	else cout << "\nErro ao abrir o arquivo\n";
}

void deletar_lista(Lista listas[], int tam) {
	cout << "Estas sao as listas atuais: \n" << endl;
	mostrar_listas(listas, tam); 

	int escolha = opcao_invalida("\n\nQual lista deseja deletar", 1, tam);
	listas[escolha - 1].deletar = true;
}

int main()
{
	int tam = 1;
	int escolha;
	Lista* listas = new Lista[tam];

	int cont_linhas = contar_linhas_arquivo(); //verifica quantidade de linhas do arquivo
	if (cont_linhas > 1) {
		listas = aumentar_vetor(listas, tam, cont_linhas - 1); //se tiver mais do que uma linha atualiza o tamanho do vetor
	}
	if (cont_linhas > 0) {
		pegar_listas_do_arquivo(listas, tam); //atualiza a lista inicial com os dados do arquivo csv
	}

	while (true) {
		cout << "\n - - - - - MENU - - - - -\n" << endl;
		cout << " [1] - Visualizar Lista Completa\n"
			<< " [2] - Escrever nova lista\n"
			<< " [3] - Deletar uma lista\n"
			<< " [4] - Jogar\n"
			<< " [0] - Salvar arquivo\n";
		escolha = opcao_invalida("\nDigite sua escolha", 0, 4);


		switch (escolha)
		{
		case 0: //SALVAR ARQUIVO
			escrever_no_arquivo(listas, tam);
			delete[] listas;
			return 0;
			break;

		case 1: //VISUALIZAR LISTA
			mostrar_listas(listas, tam);
			break;

		case 2: //ESCREVER LISTA
			if (cont_linhas == tam) {
				listas = aumentar_vetor(listas, tam, 1);
			}
			escrever_nova_lista(listas, tam);
			cont_linhas++;
			break;

		case 3: //DELETAR UMA LISTA
			cont_linhas = cont_linhas - 1;
			deletar_lista(listas, tam);
			break;

		case 4: //JOGAR
			cout << "Logo teremos esta opcao disponivel" << endl;
			break;
		}
	}
}
