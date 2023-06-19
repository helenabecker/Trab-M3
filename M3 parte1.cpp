#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

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

using namespace std;
#define tam_maximo 10

struct Lista {
    string palavras[tam_maximo];
    int tam = 0;
    bool deletar = false;
};

int opcao_invalida(string frase, int min, int max) {
    int op;
    do {
        cout << frase << " (" << min << " - " << max << "): ";
        cin >> op;
        if (op < min || op > max) {
            cout << "\nOpcao invalida. Tente novamente." << endl;
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
    T* aux = new T[tam + pos]; //aumenta o vetor com a quantidade de posições desejadas
    for (int i = 0; i < tam; i++) {
        aux[i] = listas[i];
    }
    delete[] listas;
    tam += pos;
    return aux;
}

int contar_linhas_arquivo() {
    ifstream arquivo;
    arquivo.open("listaPalavras.csv");
    int contador = 0;
    string linha;

    if (arquivo.is_open()) {
        while (getline(arquivo, linha)) {
            contador++;
        }
        arquivo.close();
        return contador;
    }
    else return 0;
}

void escrever_nova_lista(Lista listas[], int tam) {
    int cont = 0;
    listas[tam - 1].tam = opcao_invalida("Informe quantas palavras deseja escrever", 1, tam_maximo);

    do {
        cout << "\n\t" << cont + 1 << ") ";
        cin >> listas[tam - 1].palavras[cont];
        cont++;
    } while (cont < listas[tam - 1].tam);

    cout << "\nPalavras adicionadas a lista com sucesso!" << endl;
}

void mostrar_listas(Lista listas[], int tam) {
    for (int i = 0; i < tam; i++) {
        if (!listas[i].deletar) {
            cout << "\n\tLista " << i + 1 << ": ";

            for (int j = 0; j < listas[i].tam; j++) {
                if (j == listas[i].tam - 1) // se for a ultima palavra da lista não coloca a virgula no final
                    cout << listas[i].palavras[j];
                else
                    cout << listas[i].palavras[j] << ", ";
            }
            cout << endl;
        }
	}
}

void escrever_no_arquivo(Lista listas[], int tam) {
    ofstream arquivo;
    arquivo.open("listaPalavras.csv");
    if (arquivo.is_open()) {
        for (int i = 0; i < tam; i++) {
            if (!listas[i].deletar) { //só permite salvar as listas que onde deletar é false
                for (int j = 0; j < listas[i].tam; j++) {
                    arquivo << listas[i].palavras[j] << ","; //salva as palavras no arquivo com a virgula como separador
                }
                arquivo << endl;
            }
        }
        arquivo.close();
        cout << "Arquivo salvo com sucesso!" << endl;
    }
    else cout << "\nErro ao abrir o arquivo\n";
}

void pegar_listas_do_arquivo(Lista listas[], int tam) {
    ifstream arquivo;
    arquivo.open("listaPalavras.csv");
    string palavra, linha;

    if (arquivo.is_open()) {
        for (int i = 0; i < tam; i++) {
            int pos = 0; //coloca a posição inicial de cada vetor "palavras" como 0
            getline(arquivo, linha); //pega uma linha do arquivo

            for (char c : linha) { //percorre cada caracter da linha
                if (c != ',' ) //pega todos os caracteres ate encontrar uma virgula
                    palavra += c; //salva cada caracter na string
                else {
                    listas[i].palavras[pos] = palavra;
                    palavra.clear();
                    pos++;
                }
            }
            listas[i].tam = pos; //posições ocupadas do vetor palavras
        }
        arquivo.close();
    }
    else //se nao abrir o arquivo corretamente
        cout << "\nErro ao abrir o arquivo\n";
}

void deletar_lista(Lista listas[], int tam) {
    cout << "Listas atuais: \n" << endl;
    mostrar_listas(listas, tam);

    int escolha = opcao_invalida("\n\nQual lista deseja deletar", 1, tam);
    listas[escolha - 1].deletar = true;
}

void exibirMenu() {
    cout << "\n - - - - - MENU - - - - -\n" << endl
        << " [1] - Visualizar Lista Completa\n"
        << " [2] - Escrever nova lista\n"
        << " [3] - Deletar uma lista\n"
        << " [4] - Jogar\n"
        << " [0] - Salvar arquivo\n";
}

int main()
{
    int escolha, tam = 1;
    Lista* listas = new Lista[tam];

    int cont_linhas = contar_linhas_arquivo();

    if (cont_linhas > 1)
        listas = aumentar_vetor(listas, tam, cont_linhas - 1);

    if (cont_linhas > 0)
        pegar_listas_do_arquivo(listas, tam);

    while (true) {
        exibirMenu();
        escolha = opcao_invalida("\nDigite sua escolha", 0, 4);

        switch (escolha)
        {
        case 0: // SALVAR ARQUIVO
            escrever_no_arquivo(listas, tam);
            delete[] listas;
            return 0;
            break;

        case 1: // VISUALIZAR LISTA
            system("cls");
            mostrar_listas(listas, tam);
            cout << "\n";
            system("pause");
            system("cls");
            break;

        case 2: // ESCREVER LISTA
            system("cls");
            if (cont_linhas == tam) {
                listas = aumentar_vetor(listas, tam, 1);
            }
            escrever_nova_lista(listas, tam);
            cont_linhas++;
            system("pause");
            system("cls");
            break;

        case 3: // DELETAR UMA LISTA
            system("cls");
            cont_linhas = cont_linhas - 1;
            deletar_lista(listas, tam);
            system("pause");
            system("cls");
            break;

        case 4: // JOGAR (EM BREVE)
            system("cls");
            cout << "Logo teremos esta opcao disponivel" << endl;
            system("pause");
            system("cls");
            break;
        }
    }
}