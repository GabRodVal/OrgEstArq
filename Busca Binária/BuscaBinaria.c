#include <stdio.h>
#include <stdio.h>
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

void buscaBinaria (FILE *f, int left, int right, char* x, int count){
    Endereco en;
    if(right>=left){
        int middle = (right+left)/2;
        fseek(f, middle*(sizeof(Endereco)),SEEK_SET);
        int rt = fread(&en, sizeof(Endereco),1,f);
        if(strncmp(en.cep, x, 8)==0){
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",en.logradouro,en.bairro,en.cidade,en.uf,en.sigla,en.cep);
            printf("Esta busca foi feita %d vezes", count);
        }
        else if(strncmp(en.cep, x, 8)>0){
            return buscaBinaria(f, left, middle-1, x, count+1);
        }
        else{
            return buscaBinaria(f, middle+1, right, x, count+1);
        }
    }
}

int main(int argc, char**argv)
{
    FILE *arq;
	Endereco e;
    int end, amount;
	
    arq = fopen("cep_ordenado.dat","r");
    fseek(arq, 0, SEEK_END);
    end = ftell(arq);
    amount = end/sizeof(Endereco);

    buscaBinaria(arq, 0, amount, argv[1], 0);
    
	fclose(arq);

    return 0;

}