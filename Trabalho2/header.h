//
//  header.h
//  Trabalho2
//
//  Created by Léo Silva on 22/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#ifndef header_h
#define header_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#define TAM_REG_ENTRADA 134
#define TAM_REG_REMOVE 4
#define TAM_COD_SEGURADO 4
#define TAM_NOME_SEGURADO 50
#define TAM_SEGURADORA 50
#define TAM_TIPO_SEGURO 30
#define INC 5 //2 inteiros + 4 #
#define CHAR_DIVISOR '#'

//struct para armazenar as inforções lidas do arquivo de inserção
struct cadastro {
    char cod_segurado[4];
    char nome_segurado[51];
    char seguradora[51];
    char tipo_seguro[31];
};

struct index {
    char cod_segurado[4];
    int offset;
};

//faz o dump do arquivo
void dump (void);

//insere registros no arquivo de saida e no index
void insereRegistro(struct cadastro *cadastro, struct index *index);

//função para ler o arquivo de entrada e passar os registros para memória (struct)
void carregaArquivos(struct cadastro *cadastro, int quant_registros_insere);

//abre o arquivo com a flag desejada
void abreArquivo (FILE** arquivo, char *flag, char *nome_arquivo);

//conta quantos registros tem no arquivo para serem inseridos
int contaRegistrosEntrada(void);

//conta somente os caracteres no buffer
int contaCharBuffer(char * buffer);

//compacta arquivo
void compacta(FILE * arquivo);

//verifica se já carregou os arquivos para a struct
bool arquivoCarregado(struct cadastro *cadastro);

//cria indice
void criaIndice(struct index *index);

//escreve no indice
void escreveIndice(char buffer[], int offset);

//atualiza indice
void atualizaIndice(struct index *index, char buffer[], int offset);

//retorna o offset do registro no arquivo saida.bin
int pegaOffset(void);

//conta quantos registros tem no arquivo de indice
int contaRegistrosIndice(void);

#endif /* header_h */
