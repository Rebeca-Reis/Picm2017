/*
-Input:
Qual o material?
Theta e D(7 valores)
?b(largura do pico)?
-Output:
Tensão Residual e sua característica compressiva ou trativa
Gráfico Deformação relativa x Sen^2(fi)

Banco de Dados.:.
-Colunas:
Nome(Código do material)
Distância Padrão entre planos
Módulo de Young
Razão de Poisson

Procedimento:
Plotaremos um Gráfico Deformação relativa x Sen^2(fi) e pegaremos seu coeficiente angular para calcularmos a Tensão Residual



*/
#include <bits/stdc++.h>
#include <cstdlib>
#include <cmath>
#include <cstring>
//#include <koolplot.h>
//#include <graphics.h>

using namespace std;

float fi[9]/*Entrado 9 vees para gerar 9 medições para o plot do gráfico principal de saída*/,theta,d[9]/*Distânica medida*/,d0[9]/*Distância padrão presente no banco de dados*/;
float y/*Módulo de Young*/,v/*Razão de Poisson*/,tensao/*Calculada pela fórmula advinda do gráfico já regredido linearmente de Deformação relativa por sin^2(fi)*/;
float deformacao_relativa/*(d-d0)/d*/;
string material;
 
 int main(){
 	int a=3;
 	FILE *p;//Ponteiro Bolado para mexer no Banco de Dados
	p=fopen("registromateriais.txt","a");
	if(!p) printf("Erro ao abrir o arquivo!!!");
 	printf("Seja bem vindo ao nosso Software de Cálculo de Tensão Residual :-) \n");
 	while(a!=2){
 		printf("Você deseja realizar uma consulta:\n");//Opção incial do programa
 		printf("Digite 1 Para SIM;\nDigite 2 para FECHAR o programa;\n\n");
 		scanf("%d",&a);
 		printf("Opção %d escolhida\n", a);
 		if(a==2){
 			printf("Obrigado por Utilizar o programa\nVolte Sempre que Precisar\n")
 			exit();
 		}//Sai do programa
 		printf("Insira o código do material a ser analisado:\nCódigo:\t");
 		gets(material);
	 		p=fopen("registromateriais.txt","a");
				if(!p) printf("Erro ao abrir o arquivo!!!");
//Estrutura para procurar pelo nome do material no banco de dados:
				while(!feof(p)){
					fscanf(p,"%s\t%f\t%f\t%f\n",materialteste,&d0,&y,&v);		
					i = strcmp(material,materialteste);//Se retornar 0 as strings são iguais
					if (i==0) break;//Qnd forem iguais teremos nossos valores armazenados
	 					}									
	 		fclose(p);
//Dados acessados do banco de dados, vamos agora aos cálculos
 		   }
  	return 0;
  }


