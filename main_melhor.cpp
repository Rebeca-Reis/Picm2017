#include <bits/stdc++.h>
#include "Material.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;

vector<Material> materiais;
float dist[3][9],deformacao[3][9];
float fi[3]={0,45,90},psi[9]={-65,-51.71,-39.86,-26.95,0,26.95,39.86,51.71,65};
float sen2psi[9]={0.821,0.616,0.411,0.205,0,0.205,0.411,0.616,0.821};
float m[3]; //coeficiente angular da reta para cada fi
int k;


bool buscaMaterial(string nome){
	bool resp = false;
	for (int i=0;i<materiais.size();i++) if (nome == materiais[i].getNome()) resp = true;
	return resp;
}

void criarMaterial(string n){
	float y,p,d;
	cout << "Vamos adicionar esse material a nosso banco de dados.\nInforme o módulo de Young para esse material: ";
	cin >> y;
	cout << "Informe o coeficiente de poisson do material: ";
	cin >> p;
	cout << "Informe a distância padrão d0 para esse material: ";
	cin >> d;
	Material mat = Material(n,y,p,d);
	materiais.push_back(mat);
}

void alteraMaterial(string n){
	float d;
	cout << "O material já consta no banco de dados.\nInforme o valor da distancia padrão d0: ";
	cin >> d;
	for(int i=0; i<materiais.size();i++) if (materiais[i].getNome() == n and materiais[i].getD0() == 0) materiais[i].setD0(d);
}

bool verifica_d0(string nome){
	bool resp = false;
	for(int i=0;i<materiais.size();i++) if (nome == materiais[i].getNome() and materiais[i].getD0() == 0) resp = true;
	return resp; 
}

float lms(float *x, float deformacao[][9], int l) //REGRESSÃO LINEAR
{
    float y[9]; 
    for(int i=0;i<9;i++) y[i]=deformacao[l][i]; //l= 0,1 ou 2, depende do fi
    int i;
    float m;
    float sumYX = 0.;
    float sumX = 0.;
    float sumY = 0.;
    float sumX2 = 0.;
    float sum2X = 0.;
    for(i = 0; i < 9; i++){
        sumYX += x[i] * y[i];
        sumX += x[i];
        sumY += y[i];
        sumX2 += x[i] * x[i];
    }
    sum2X = sumX * sumX;
    m = (sumYX - (sumX * sumY) / 9.0) / (sumX2 - sum2X / 9.0);
    //b = sumY / (float)n - *m * sumX / (float)n;
    return m;
} //Para chamar a funçao, use lms(sen2psi,deformacao,l))


void realizarCalculos(string nome){
	cout << "Oi! Vc está no caminho certo!" << endl;
	memset(dist,0,sizeof(dist));
	memset(deformacao,0,sizeof(deformacao));
	cout << "Por favor, primeiramente insira os 9 valores para as distância entre os planos difratores inclinados referentes ao ângulo fi=0°\n";
	cout << "Para isso, digite os valores correspondentes a cada psi em ordem crescente desses ângulos, apertando o botão ENTER entre cada um\n";
	for(int i=0;i<9;i++) cin >> dist[0][i];
	cout << "Por favor, agora insira os 9 valores para as distância entre os planos difratores inclinados referentes ao ângulo fi=45°\n";
	cout << "Para isso, digite os valores correspondentes a cada psi em ordem crescente desses ângulos, apertando o botão ENTER entre cada um\n";
	for(int i=0;i<9;i++) cin >> dist[1][i];
	cout << "Por favor, agora insira os 9 valores para as distância entre os planos difratores inclinados referentes ao ângulo fi=90°\n";
	cout << "Para isso, digite os valores correspondentes a cada psi em ordem crescente desses ângulos, apertando o botão ENTER entre cada um\n";
	for(int i=0;i<9;i++) cin >> dist[2][i];
	//usuário já inseriu toas as informações requeridas 
	//vamos encontrar o índice(k) do material na lista do banco de dados
	for (int i=0;i<materiais.size();i++) if (nome == materiais[i].getNome()) k=i;
	//preenchimento da matriz das deformações
	for(int i=0;i<3;i++){
		for(int j=0;i<9;i++){
			deformacao[i][j]=(dist[i][j]-materiais[k].getD0())/materiais[k].getD0();
		}
	}
	for(int i=0;i<3;i++) m[i]=lms(sen2psi,deformacao,i);
	
}

void menu(){
	string nome;
	int op;
	cout << "\nDigite:\n1 - Consulta\n2 - Adicionar Material\n3 - Sair do Programa\n\n";
	cin >> op;
	switch(op){
		case 1:{
		 	cout << "Informe o nome do material: ";
		 	cin >> nome;
		 	if(buscaMaterial(nome) and verifica_d0(nome)) alteraMaterial(nome);
		 	if(!buscaMaterial(nome)){
		 		cout << "O material ainda não existe!!! Ou vc pode ter digitado errado!! Digite novamente o nome: \n";
		 		cin >> nome;
		 		if(buscaMaterial(nome) and verifica_d0(nome)) alteraMaterial(nome);
		 		if(!buscaMaterial(nome)){ 
		 			cout << "O material não consta no banco de dados." << endl;
		 			return ;
		 		}
		 	}
		 	realizarCalculos(nome);
		 	menu();
		 	break;
		}

		case 2:{
		 	cout << "Informe o nome do material: ";
			cin >> nome;
			criarMaterial(nome);
			menu();
			break;
		}
	}
}

int main(){

	Material mat;
	string s;
	float n1,n2,n3;
	cout << "Seja bem vindo ao nosso Software de Cálculo de Tensão Residual :-) \nO que vc deseja fazer?\n";

	// PRIMEIRA COISA DO MAIN
	fstream file("Registro.txt");
	if(file.is_open()){
		while(file >> s >> n1 >> n2 >> n3){
			mat = Material(s,n1,n2,n3);
			materiais.push_back(mat);	
		}
	}
	file.close();

	menu();

	// ULTIMA COISA DO MAIN
	fstream novo("Registro.txt");
	if(novo.is_open()){
		for(int i=0;i<materiais.size();i++) novo << materiais[i].getNome() << " " << materiais[i].getYoung() << " " << materiais[i].getPoisson() << " " << materiais[i].getD0() << endl;
	}
	novo.close();
}