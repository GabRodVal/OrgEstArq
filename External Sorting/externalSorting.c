#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

void merge(int first, int last){
	FILE *arqA, *arqB, *arqOut;
	Endereco eA, eB;
	
	char entradaA[20];
	char entradaB[20];
	char saida[20];



	sprintf(entradaA,"cep%d.dat",first);
	arqA = fopen(entradaA, "rb");
	fread(&eA, sizeof(Endereco), 1, arqA);

	sprintf(entradaB,"cep%d.dat",last);
	arqB = fopen(entradaB, "rb");
	fread(&eB, sizeof(Endereco), 1, arqB);

	sprintf(saida,"cep%d.dat",last+1);
	arqOut = fopen (saida, "wb");

	while(!feof(arqA) && !feof(arqB)){

		if(compara(&eA,&eB)<0){
			fwrite(&eA,(sizeof(Endereco)), 1, arqOut);
			fread(&eA, sizeof(Endereco), 1, arqA);
		}else{
			fwrite(&eB,(sizeof(Endereco)), 1, arqOut);
			fread(&eB, sizeof(Endereco), 1, arqB);
		}

	}

	while(!feof(arqA)){
		fwrite(&eA, sizeof(Endereco), 1, arqOut);
		fread(&eA, sizeof(Endereco), 1, arqA);
	}
	while(!feof(arqB)){
		fwrite(&eB, sizeof(Endereco), 1, arqOut);
		fread(&eB, sizeof(Endereco), 1, arqB);
	}

	fclose(arqA);
	fclose(arqB);
	fclose(arqOut);
	if(remove(entradaA)!=0){
		printf("ERRO");
		}
	if(remove(entradaB)!=0){
		printf("ERRO");
	}
}

int exSort(int num, int count, char* op){
    
	FILE *arq, *arqOut;
	Endereco *e;
	int total, amount, half, first, last;
    char entradaA[20];
    char entradaB[20];
    char saida[20];

    arq = fopen(op,"rb");

    fseek(arq,0,SEEK_END);
    total = ftell(arq);
    amount = total/(sizeof(Endereco));
    half = amount/2;

    if(count <5){
		e = (Endereco*) malloc(half*sizeof(Endereco));
		fseek(arq,0,SEEK_SET);
		fread(e,sizeof(Endereco),half,arq);
		num++;
		sprintf(entradaA,"cep%d.dat",num);
		arqOut = fopen(entradaA, "wb");
		fwrite(e, sizeof(Endereco), half, arqOut);
		fclose(arqOut);
		free (e);
		num = exSort(num, count+1, entradaA);
		first = num;

		e = (Endereco*) malloc((amount-half)*sizeof(Endereco));
		fread(e,sizeof(Endereco),amount-half,arq);
		num++;
		sprintf(entradaB,"cep%d.dat",num);
		arqOut = fopen(entradaB, "wb");
		fwrite(e, sizeof(Endereco), half, arqOut);
		fclose(arqOut);
		free (e);
		num = exSort(num, count+1, entradaB);
		last = num;

		merge(first, last);
		num++;
		fclose(arq);
		if(count>0){
			if(remove(op)!=0){
				printf("ERRO");
			}
		}
    }else{
		e = (Endereco*) malloc(total);
		fseek(arq,0,SEEK_SET);
		fread (e, sizeof(Endereco), amount, arq);
		qsort (e, amount, sizeof(Endereco), compara);
		num++;
		sprintf(entradaA,"cep%d.dat",num);
		arqOut = fopen(entradaA, "wb");
		fwrite(e, sizeof(Endereco), amount, arqOut);
		free(e);
		fclose(arq);
		fclose(arqOut);
		if(remove(op)!=0){
			printf("ERRO");
		}
	}
	

    return num;
}

int main(int argc, char**argv){
	int num;
	char arquivo[] = "cep.dat";
	char ordenado[] = "cep_ordenado.dat";
	char es[20];

	num = exSort(0, 0, arquivo);
	sprintf(es, "cep%d.dat", num);
	rename(es, ordenado);

	return 0;
}
