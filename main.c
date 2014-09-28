#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-------------Estrutura
typedef struct No{
	bool exists;//Sinaliza se existe a palavra até esse ponto
	struct No* sheet[26];
	char* line;
} No;
//----------------------

//-----------Constante
int c_alphabet_length = 26;
//---------------------
//No trabalho de estrutura fazer com que o texto busque na gramatica e não a gramatica buscar no texto

//---------------Variaveis Globais
No* gno_root_dictionary;//n�� raiz  ; gno =global No

//--------------------------------

char* append(char* a_str, char a_c) {
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* a_str: Recebe a string como base;
	//			char c: A letra que deseja-se concatenar
	//
	//Descrição da função:
	//			Adiciona uma letra ao final da string, Ex.: cas + a = casa
	//-------------------------------------------------------------

	char * ls_new_str;
	if(a_str==NULL){// Se for nulo, aloca um espaço de memoria pra armazenar a letra e o '\0'
		a_str = (char*)malloc(sizeof(char)*2);
		a_str[0] = a_c;
		a_str[1] = '\0';
	}
	else{
		int i;
		ls_new_str = a_str;// salva em um ponteiro temporario
		a_str = (char*)malloc(sizeof(char)*(strlen(ls_new_str)+2));//Aloca um novo espaço do tamanho da nova palavra
		for(i=0; i< strlen(ls_new_str); i++)
			a_str[i] = ls_new_str[i];
		a_str[i] = a_c;
		i++;
		a_str[i] = '\0';
		ls_new_str = NULL;
		free(ls_new_str);
	}
	return a_str;
}

char* append_string(char* a_str1, char* a_str2) {
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* ls_str1: Recebe a string como base;
	//			char* ls_str2: Recebe a segunda string
	//
	//Descrição da função:
	//			Concatena Strings
	//-------------------------------------------------------------

	char * ls_new_str = NULL;
	int tamanho=0;
	if(a_str1 != NULL){
		tamanho +=strlen(a_str1);
	}
	if(a_str2 != NULL){
		tamanho += strlen(a_str2);
	}
	ls_new_str = malloc(sizeof(char)*tamanho);//Erro aqui ao dar strlen em null
	if(a_str1 != NULL){
		strcpy(ls_new_str, a_str1);
	}
	if(a_str2 != NULL){
		strcat(ls_new_str,a_str2);
	}
	return ls_new_str;
}

void insert_word(No** a_root, char* a_word){
	//-------------------------------------------------------------
	//Retorno:
	//			void;
	//
	//Argumentos:
	//			No** a_root: Nó da arvore que será inserido a letra da palavra(Passagem por parametro);
	//			char* a_word: String que se deseja inserir;
	//
	//Descrição da função:
	//			Inserção na arvore trie de maneira recursiva;
	//-------------------------------------------------------------

	int i;

	if (*a_root == NULL){//Caso o nó é nulo (caso base)

		*a_root = malloc(sizeof(No));

		for (i = 0;i<c_alphabet_length;i++){
			(*a_root)->sheet[i]=NULL;//Retirada dos possiveis lixos de memoria
			(*a_root)->line=NULL;
		}

		if(a_word[0]!='\0'){//Se n for final de palavra
			insert_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1);//obs: a = 97, logo a-a = 0, posição 0(zero)
			(*a_root)->exists=false;
		}else{
			(*a_root)->exists=true;//Final de palavra
		}

	}else{

		if(a_word[0] !='\0'){//Se a palavra não chegou ao fim
			insert_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1);
		}else{
			(*a_root)->exists=true;
		}

	}
}

void exist_word(No* a_root, char* a_word){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = palavra achada
	//					false = palavra não achada
	//
	//Argumentos:
	//			No* a_root: nó da arvore
	//			char* a_word: auxiliar de string
	//
	//Descrição da função:
	//			Varre toda a arvore Trie, imprimindo as palavras encontradas
	//-------------------------------------------------------------


	if(a_root == NULL)//Caso a arvore esteja nula, só ocorre na inicialização
		return;
	else {

		int i;
		if(a_root->exists)
			if(a_root->line != NULL)
				printf("%s %s\n",a_word,a_root->line);//Se a palavra existe e ocorreu no texto, mostre!


		for(i = 0 ; i<c_alphabet_length; i++)//Continue procurando em cada filho
			if(a_root->sheet[i]!= NULL)
				exist_word(a_root->sheet[i],append(a_word,'a'+i));
	}

	return;

}
/*
char* exist_word(No* a_root, char* a_word){

	if(a_root == NULL)
		return NULL;
	else if(a_word[0]=='\0')
		return a_root->line;
	else
		return exist_word(a_root->sheet[a_word[0]-'a'],a_word+1);

}*/

bool verify_word(No** a_root, char* a_word, int a_line){//exist_word 2.0
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = palavra achada
	//					false = palavra não achada
	//
	//Argumentos:
	//			No** a_root: nó da arvore
	//			char* a_word: palavra que se deseja achar
	//			int a_line: A linha a palavra foi encontrada
	//
	//Descrição da função:
	//			Busca a palavra na arvore,marca a linha do texto caso exista
	//			e retorna a se existe ou não
	//-------------------------------------------------------------


	if((*a_root) == NULL)
		return false;
	else if(a_word[0]=='\0')

		if ((*a_root)->exists){//Se existe a palavra, marca a linha que foi encontrada
			char aux[5];
			if((*a_root)->line == NULL){
				sprintf(aux,"%d",a_line);
				(*a_root)->line = append_string(NULL,aux);
			}else{
				sprintf(aux,"%d",a_line);
				(*a_root)->line = append((*a_root)->line,',');
				(*a_root)->line = append_string((*a_root)->line,aux);
			}
			return true;

		}else{
			return false;
		}

	else
		return verify_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1,a_line);

}

bool is_letter(char a_c){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = é um char entre 'a' e 'z'(minusculo)
	//					false = é algo que não é entre 'a' e 'z' (minusculo)
	//
	//Argumentos:
	//			char a_c: letra que se deseja comparar
	//
	//Descrição da função:
	//			verificar se o 'char c' é uma letra minuscula(a-z)
	//-------------------------------------------------------------

	if(a_c>='a' && a_c<='z')
		return true;
	else
		return false;
}

char lower(char a_c){
	//-------------------------------------------------------------
	//Retorno:
	//			char : a letra minuscula
	//
	//Argumentos:
	//			char a_c: letra que se deseja torna minuscula
	//
	//Descrição da função:
	//			Tornar uma letra maiuscula em minuscula
	//-------------------------------------------------------------

	if(a_c>='A' && a_c<='Z')
		return a_c+32;//Tabela ASCII
	else
		return a_c;
}

void initialize_dictionary(char* a_name_file){
	//-------------------------------------------------------------
	//Retorno:
	//			void
	//
	//Argumentos:
	//			char* a_name_file: diretório/nome do arquivo que será aberto
	//
	//Descrição da função:
	//			Inicializa a arvore com as palavras do dicionário
	//-------------------------------------------------------------

	FILE *lf_file;
			lf_file = fopen("Debug/gramatica.txt","r");
			//lf_file = fopen(a_name_file,"r"); // Gramatica
			if(lf_file!=NULL){
				char lc_c;
				char* ls_str;
				long int li_file_size;


				gno_root_dictionary = NULL;

				fseek(lf_file, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
				li_file_size = ftell(lf_file);//pegar o tamanho do arquivo
				fseek(lf_file,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


				lc_c=' ';//preenche com qualquer coisa para entao entrar no while


				while(lc_c!=EOF && ftell(lf_file) < li_file_size){//Loop para pegar o texto no arquivo
					ls_str=NULL;
					fscanf(lf_file,"%c",&lc_c);
					lc_c = lower(lc_c);

					while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
						ls_str = append(ls_str,lc_c);
						fscanf(lf_file,"%c",&lc_c);
						lc_c = lower(lc_c);
					}
					if(ls_str!=NULL){
						if(!is_letter(lc_c)){
							insert_word(&gno_root_dictionary,ls_str);
						}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
							ls_str = append(ls_str,lc_c);
							insert_word(&gno_root_dictionary,ls_str);
						}
					}
				}
				ls_str=NULL;
				free(ls_str);
				fclose(lf_file);
			}else{
				printf("File (%s) not found!",a_name_file);
				exit(-1);
			}

}

void initialize_text(char* a_name_file){
	//-------------------------------------------------------------
	//Retorno:
	//			void
	//
	//Argumentos:
	//			char* a_name_file: diretório/nome do arquivo que será aberto
	//
	//Descrição da função:
	//			Lê o texto, buscando as palavras que existem e marcando a linha(verify_word)
	//-------------------------------------------------------------

	FILE *lf_file;
		lf_file = fopen("Debug/texto.txt","r");
		//f = fopen(a_name_file,"r"); // Gramatica
		if(lf_file!=NULL){
			char lc_c;
			char* ls_str;
			long int li_file_size;
			int li_line = 1;

			fseek(lf_file, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
			li_file_size = ftell(lf_file);//pegar o tamanho do arquivo
			fseek(lf_file,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


			lc_c=' ';//preenche com qualquer coisa para entao entrar no while


			while(lc_c!=EOF && ftell(lf_file) < li_file_size){//Loop para pegar o texto no arquivo
				ls_str=NULL;
				fscanf(lf_file,"%c",&lc_c);
				lc_c = lower(lc_c);

				while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
					ls_str = append(ls_str,lc_c);
					fscanf(lf_file,"%c",&lc_c);
					lc_c = lower(lc_c);
				}
				if(ls_str!=NULL){
					if(!is_letter(lc_c)){
						verify_word(&gno_root_dictionary,ls_str,li_line);
					}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
						ls_str = append(ls_str,lc_c);
						verify_word(&gno_root_dictionary,ls_str,li_line);
					}
				}
				if(lc_c == '\n'){
					li_line++;
				}
			}
			ls_str=NULL;
			free(ls_str);
			fclose(lf_file);

		}else{
			printf("File (%s) not found!",a_name_file);
			exit(-1);
		}

}


int main(int argc, char **argv) {


	if(argc >= 1){//Mudar dps para 3<<<<<<<<<<<<<<<<<<<<<<<<<

		initialize_dictionary(argv[1]);
		initialize_text(argv[2]);

//TODO
		//1-Tratar palavras q n existem na funcao verify_word
		//Logica para fazer; ex casa
		// primeiro faz a insercao casa[a-z]
		// segundo a troca cas[a-z]
		//delete ca[a-z]a


		exist_word(gno_root_dictionary,NULL);//Mostra a saída

	}else{
		printf("Bad arguments");
	}
	return 0;
}
