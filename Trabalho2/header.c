//
//  header.c
//  Trabalho2
//
//  Created by Léo Silva on 22/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

//atualiza indice
void atualizaIndice(struct index *index, char buffer[], int offset){
    int i = 0;
    
    while(index[i].cod_segurado[0] != '\0'){
        i++;
    }
    
    //atualiza o indice
    index[i].cod_segurado[0] = buffer[0];
    index[i].cod_segurado[1] = buffer[1];
    index[i].cod_segurado[2] = buffer[2];
    index[i].offset = offset;
    
}

//retorna o offset do registro no arquivo saida.bin
int pegaOffset(){
    FILE *saida;
    abreArquivo(&saida, "r+", "saida.bin");
    
    rewind(saida);
    
    int offset = 0;
    
    while(fgetc(saida) != EOF){
        offset++;
    }
    
    return offset;
}

//escreve no indice
void escreveIndice(char buffer[], int offset){
    FILE *indice;
    abreArquivo(&indice, "r+", "indice.bin");
    
    while(fgetc(indice) != EOF);
    
    fprintf(indice, "%c%c%c%i#", buffer[0], buffer[1], buffer[2], offset);
    
    fclose(indice);
}

//cria indice
void criaListaIndice(struct index *index){
    
    FILE *indice;
    abreArquivo(&indice, "r+", "indice.bin");
    
    rewind(indice);
    
    //armazena dados lidos de indice.bin
    char buffer[3];
    
    int offset = 0;
    
    char caractere;
    
    //i para as posiçõe do buffer e j para as posições do index
    int i = 0;
    int j = 0;
    
    while(!((caractere = fgetc(indice)) == EOF)){
        
        //limpa o buffer preenchendo com '\0'
        memset(buffer,(char)'\0',sizeof(buffer));
        
        //preenche o buffer com cod_segurado
        for (i = 0; i < 3; i++){
            buffer[i] = caractere;
            caractere = fgetc(indice);
        }
        
        ungetc(caractere, indice);
        
        fscanf(indice, "%d", &offset);
        
        //atualiza o indice
        index[j].cod_segurado[0] = buffer[0];
        index[j].cod_segurado[1] = buffer[1];
        index[j].cod_segurado[2] = buffer[2];
        index[j].offset = offset;
        
        j++;
        
        caractere = fgetc(indice);
    }
}

//verifica se já carregou os arquivos para a struct
bool arquivoCarregado(struct cadastro *cadastro){
    if ((void*) strlen(cadastro[0].cod_segurado) == NULL){
        printf("Carregue os arquivos de entrada primeiro (opcao 5).\n");
        return false;
    }else{
        return true;
    }
}

//conta somente os caracteres no buffer
int contaCharBuffer(char * buffer){
    
    int i = 0;
    int total_caracteres = 0;
    
    while(i < TAM_REG_ENTRADA){
        if(buffer[i] != '\0'){
            total_caracteres++;
        }
        i++;
    }
    return total_caracteres;
}

//conta quantos registros tem no arquivo de entrada para serem inseridos
int contaRegistrosEntrada(){
    
    FILE *entrada;
    
    abreArquivo(&entrada, "r", "insere.bin");
    
    char registro[TAM_REG_ENTRADA];
    int quant_registros = 0;
    
    //descobre quantos registros têm no arquivo para salvar na struct
    while (fread(&registro,sizeof(char),TAM_REG_ENTRADA,entrada))
    {
        quant_registros++;
    }
    
    fclose(entrada);
    
    return quant_registros;
}

//abre o arquivo com a flag desejada
void abreArquivo (FILE** arquivo, char *flag, char *nome_arquivo){
    
    unsigned long tam = strlen(nome_arquivo) + strlen("/Users/leo/Desktop/Faculdade/ED2 - 2019/EDII-T2/Trabalho2/");
    
    char caminho_arquivo[tam];
    
    sprintf(caminho_arquivo, "%s%s", "/Users/leo/Desktop/Faculdade/ED2 - 2019/EDII-T2/Trabalho2/", nome_arquivo);
    
    if ((*arquivo = fopen(caminho_arquivo, flag)) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
        exit(0);
    }
    
}

//faz o dump do arquivo
void dump (){
    
    FILE *saida;
    
    abreArquivo(&saida, "r+", "saida.bin");
    
    char value;
    
    while (!feof(saida)){
        value = fgetc(saida);
        
        if (isprint(value)){
            //%x imprime em hexadecimal
            printf("%x ", value);
        }
    }
    
    fclose(saida);
    
}

//insere registros no arquivo de saida
void insereRegistro(struct cadastro *cadastro, struct index *index){
    
    FILE *inserido;
    FILE *saida;
    FILE *indice;
    
    abreArquivo(&indice, "r+", "indice.bin");
    abreArquivo(&inserido, "r+", "inserido.bin");
    abreArquivo(&saida, "r+", "saida.bin");
    
    //contador das posições da struct cadastro
    int i = 0;
    
    //contador
    int k = 0;
    
    //offset do registro em saida.bin
    int offset = 0;
    
    //quantidade de seek
    int quant_seek = 0;
    
    //char para guardar a informação lida de inserido.bin
    char cod_segurado_inserido[TAM_COD_SEGURADO];
    
    //escreve o tamanho em saida.bin
    char buffer[TAM_REG_ENTRADA];
    
    //limpa a struct preenchendo tudo '\0'
    memset(buffer,(char)'\0',sizeof(buffer));
    
    //cabeçalho inicial
    char cabecalho[] = "0-1";
    
    //verifica se o arquivo está vazio para inserir o cabeçalho
    if(fgetc(saida) == EOF){
        fputc(cabecalho[0], saida);
        fputc(cabecalho[1], saida);
        fputc(cabecalho[2], saida);
        fputc('\n', saida);
    }
    
    rewind(saida);
    
    //pula o header e '\n'
    fseek(saida, 4, 1);
    
    //enquanto puder ler em inserido.bin significa que já foi feita inserção e encontra código válido
    while(fread(cod_segurado_inserido, sizeof(cod_segurado_inserido), 1, inserido)){
        if (cadastro[i].cod_segurado[2] == cod_segurado_inserido[2]){
            //printf("Codigo %.3s ja existe, procurando novo codigo.\n", cadastro[i].cod_segurado);
            i++;
        }
    }
    
    if(cadastro[i].cod_segurado[2] == '\0'){
        printf("Nao ha mais registro para inserir.\n");
    }else{
        //soma todos os offsets desde o primeiro registro até cadastro[i]
        while(k < i){
            
            quant_seek += strlen(cadastro[k].cod_segurado);
            quant_seek += strlen(cadastro[k].nome_segurado);
            quant_seek += strlen(cadastro[k].seguradora);
            quant_seek += strlen(cadastro[k].tipo_seguro);
            k++;
            
            //incrementa 5: 3 '#' e 2 digitos do tamanho
            if(k >= 1){
                quant_seek += 5;
            }
        }
        
        //escreve em inserido.bin o cod_segurado ja inserido
        fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
        
        offset = pegaOffset();
        
        //escreve em indice.bin o cod_segurado e o offset
        escreveIndice(cadastro[i].cod_segurado, offset);
    
        //atualiza o indice em memoria principal
        atualizaIndice(index, cadastro[i].cod_segurado, offset);
        
        //garante que saida.bin esteja na posição correta e não sobrescreva nada
        fseek(saida, quant_seek, 1);
        
        memcpy(buffer, cadastro[k].cod_segurado, sizeof(cadastro[k].cod_segurado));
        memcpy(buffer + sizeof(cadastro[k].cod_segurado), cadastro[k].nome_segurado, sizeof(cadastro[k].nome_segurado));
        memcpy(buffer + sizeof(cadastro[k].cod_segurado) + sizeof(cadastro[k].nome_segurado), cadastro[k].seguradora, sizeof(cadastro[k].seguradora));
        memcpy(buffer + sizeof(cadastro[k].cod_segurado) + sizeof(cadastro[k].nome_segurado) + sizeof(cadastro[k].tipo_seguro), cadastro[k].tipo_seguro, sizeof(cadastro[k].tipo_seguro));
        
        fprintf(saida, "%d", contaCharBuffer(buffer) + INC);
        
        //posiciona os '#'
        bool flag = true;
        for(int l = 0; l < TAM_REG_ENTRADA; l++){
            
            if(buffer[l] != '\0' && buffer[l-1] == '\0'){
                fputc('#', saida);
            }
            
            if(buffer[l] != '\0'){
                fputc(buffer[l], saida);
            }
            
            if(flag){
                fseek(saida, -2, 1);
                fputc(buffer[l], saida);
                flag = false;
            }
        }
    }
    
    fclose(indice);
    fclose(inserido);
    fclose(saida);
}

//função para ler o arquivo de entrada e passar os registros para memória (struct)
void carregaArquivos(struct cadastro *cadastro, int quant_registros_insere){
    
    FILE *insere;
    
    abreArquivo(&insere, "r+", "insere.bin");
    
    //salva o registro do arquivo para memória principal
    char buffer_insere[TAM_REG_ENTRADA];
    
    //percorre as posições da struct
    int j = 0;
    
    //percorre as posições do buffer_insere
    int i = 0;
    
    //garante que arquivo esteja no começo
    rewind(insere);
    
    //limpa a struct preenchendo tudo '\0'
    memset(cadastro,(char)'\0',sizeof(struct cadastro));
    
    //enquanto houver registros
    while (j < quant_registros_insere){
        
        //le o registro do arquivo e passa para memória principal para ser manipulado
        fgets(buffer_insere,TAM_REG_ENTRADA+1,insere);
        
        strcpy(cadastro[j].cod_segurado, &buffer_insere[0]);
        strcpy(cadastro[j].nome_segurado, &buffer_insere[4]);
        strcpy(cadastro[j].seguradora, &buffer_insere[54]);
        strcpy(cadastro[j].tipo_seguro, &buffer_insere[104]);
        
        i = 0;
        j++;
    }
    
    fclose(insere);
}