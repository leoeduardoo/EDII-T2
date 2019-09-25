//
//  main.c
//  Trabalho2
//
//  Created by Léo Silva on 22/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

int main(void)
{
    struct index index[10];
    
    //seta todas as posições como '\0'
    memset(&index,(char)'\0',sizeof(index));
    
    criaListaIndice(index);
    
    //guarda a quantidade de registros a serem inseridos
    int quant_registros_insere = contaRegistrosEntrada();
    
    //declara a struct cadastro
    struct cadastro cadastro[quant_registros_insere];
    
    //seta todas as posições como '\0'
    memset(&cadastro,(char)'\0',sizeof(cadastro));
    
    int opcao = 0;
    
    do {
        printf("\nCadastro de Seguradora:\n");
        printf("\n\t1. Insercao");
        printf("\n\t2. Procurar por Codigo de Segurado");
        printf("\n\t3. Procurar por Nome de Segurado");
        printf("\n\t4. Dump Arquivo");
        printf("\n\t5. Carrega Arquivos");
        printf("\n\t6. Sair\n");
        printf("\nDigite uma opcao: ");
        fflush(stdin);
        scanf(" %d", &opcao);
        
        switch (opcao) {
            case 1:
                if (arquivoCarregado(cadastro)){
                    insereRegistro(cadastro, index);
                }
                break;
                
            case 2:
                printf("Procurar por Codigo de Segurado nao implementado.\n");
                break;
                
            case 3:
                printf("Procurar por Nome de Segurado nao implementado.\n");
                break;
                
            case 4:
                dump();
                break;
                
            case 5:
                carregaArquivos(cadastro, quant_registros_insere);
                break;
                
            case 6:
                break;
                
            default:
                printf("\nOpcao invalida.\n");
        }
        
    } while (opcao != 6);
    
    exit(0);
    return 0;
}
