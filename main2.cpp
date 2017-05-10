#include <bits/stdc++.h>
#include "Material.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;
#define pi 3.141592

vector<Material> materiais;
double dist[3][9],deformacao[3][9],sigma_fi[3],sigmas[3][2]; //sigmas: cada linha corresponde a um fi, e cada coluna é sigma11 ou sigma22
double fi[3]={0,pi/4,pi/2};
double psi[9]={-65,-51.71,-39.86,-26.95,0,26.95,39.86,51.71,65};
double sen2psi[9]={0.821,0.616,0.411,0.205,0,0.205,0.411,0.616,0.821};
double m[3]; //coeficiente angular da reta para cada fi
int k;


bool buscaMaterial(string nome){
	bool resp = false;
	for (int i=0;i<materiais.size();i++) if (nome == materiais[i].getNome()) resp = true;
	return resp;
}

void criarMaterial(string n){
	double y,p,d;
	string virgula;
	cout << "Vamos adicionar esse material a nosso banco de dados.\nInforme o módulo de Young para esse material: ";
	cin >> virgula;
	replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
	y = atof(virgula.c_str());  //passa de string para double
	virgula.erase();
	cout << "Informe o coeficiente de poisson do material: ";
	cin >> virgula;
	replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
	p = atof(virgula.c_str());  //passa de string para double
	virgula.erase();
	cout << "Informe a distância padrão d0 para esse material: ";
	cin >> virgula;
	replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
	d = atof(virgula.c_str());  //passa de string para double
	virgula.erase();
	Material mat = Material(n,y,p,d);
	materiais.push_back(mat);
}

void alteraMaterial(string n){
	double d;
	string virgula;
	cout << "O material já consta no banco de dados.\nInforme o valor da distancia padrão d0: ";
	cin >> virgula;
	replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
	d = atof(virgula.c_str());  //passa de string para double
	virgula.erase();
	for(int i=0; i<materiais.size();i++) if (materiais[i].getNome() == n and materiais[i].getD0() == 0) materiais[i].setD0(d);
}

bool verifica_d0(string nome){
	bool resp = false;
	for(int i=0;i<materiais.size();i++) if (nome == materiais[i].getNome() and materiais[i].getD0() == 0) resp = true;
	return resp; 
}

double lms(double *x, double deformacao[][9], int l) //REGRESSÃO LINEAR
{
    double y[9]; 
    for(int i=0;i<9;i++) y[i]=deformacao[l][i]; //l= 0,1 ou 2, depende do fi
    int i;
    double m;
    double sumYX = 0.;
    double sumX = 0.;
    double sumY = 0.;
    double sumX2 = 0.;
    double sum2X = 0.;
    for(i = 0; i < 9; i++){
        sumYX += x[i] * y[i];
        sumX += x[i];
        sumY += y[i];
        sumX2 += x[i] * x[i];
    }
    sum2X = sumX * sumX;
    m = (sumYX - (sumX * sumY) / 9.0) / (sumX2 - sum2X / 9.0);
    //b = sumY / (double)n - *m * sumX / (double)n;
    return m;
} //Para chamar a funçao, use lms(sen2psi,deformacao,l))


void realizarCalculos(string nome){
	string virgula;
	memset(dist,0,sizeof(dist));
	memset(deformacao,0,sizeof(deformacao));
	cout << "Por favor, primeiramente insira os 9 valores para as distância entre os planos difratores inclinados referentes ao ângulo fi=0°\n";
	cout << "Para isso, digite os valores correspondentes a cada psi em ordem crescente desses ângulos, apertando o botão ENTER entre cada um\n";
	for(int i=0;i<9;i++){
		cin >> virgula;
		replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
		dist[0][i] = atof(virgula.c_str());  //passa de string para double
		virgula.erase();
	} 
	cout << "Por favor, agora insira os 9 valores para as distância entre os planos difratores inclinados referentes ao ângulo fi=45°\n";
	cout << "Para isso, digite os valores correspondentes a cada psi em ordem crescente desses ângulos, apertando o botão ENTER entre cada um\n";
	for(int i=0;i<9;i++){
		cin >> virgula;
		replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
		dist[1][i] = atof(virgula.c_str());  //passa de string para double
		virgula.erase();
	}
	cout << "Por favor, agora insira os 9 valores para as distância entre os planos difratores inclinados referentes ao ângulo fi=90°\n";
	cout << "Para isso, digite os valores correspondentes a cada psi em ordem crescente desses ângulos, apertando o botão ENTER entre cada um\n";
	for(int i=0;i<9;i++){
		cin >> virgula;
		replace(virgula.begin(),virgula.end(),',','.'); //converte o numero com virgula para ponto
		dist[2][i] = atof(virgula.c_str());  //passa de string para double
		virgula.erase();
	} 
	//usuário já inseriu todas as informações requeridas 
	//vamos encontrar o índice(k) do material na lista do banco de dados
	for (int i=0;i<materiais.size();i++) if (nome == materiais[i].getNome()) k=i;
	//preenchimento da matriz das deformações
	for(int i=0;i<3;i++){
		for(int j=0;i<9;i++){
			deformacao[i][j]=(dist[i][j]-materiais[k].getD0())/materiais[k].getD0();
		}
	}
	for(int i=0;i<3;i++) m[i]=lms(sen2psi,deformacao,i);
	printf("m1: %lf\nm2: %lf\nm3: %lf\n",m[0],m[1],m[2]);
	double y_mat = materiais[k].getYoung(); //unidade: GPa
	double p_mat = materiais[k].getPoisson();
	for(int i=0;i<3;i++) sigma_fi[i]=m[i]*(y_mat*1000)/(1+p_mat); //unidade: MPa
	for(int i=0;i<3;i++) printf("sigma_fi: %f MPa\n",sigma_fi[i]);	
	for(int i=0;i<3;i++){ //calculo de sigma11 e sigma22 : unidade MPa
		if(i==1) continue; //para fi=45º o denominador se anula, por isso nao calcularemos sigma11 e sigma22 para este fi
		sigmas[i][0]=sigma_fi[i] + (1000*y_mat*deformacao[i][4]*sin(fi[i])*sin(fi[i]))/p_mat;
		sigmas[i][0] /= (cos(fi[i])*cos(fi[i]) - sin(fi[i])*sin(fi[i]));
		sigmas[i][1]=-(sigma_fi[i] + (1000*y_mat*deformacao[i][4]*cos(fi[i])*cos(fi[i]))/p_mat);
		sigmas[i][1] /= (cos(fi[i])*cos(fi[i]) - sin(fi[i])*sin(fi[i]));
	}	
	printf("Sigmas 11: %lf\n\t%lf\n\t%lf\nsigmas22: %lf\n\t%lf\n\t%lf\n",sigmas[0][0],sigmas[1][0],sigmas[2][0],sigmas[0][1],sigmas[1][1],sigmas[2][1]);
	double media_11, media_22;
	media_11=(sigmas[0][0] + sigmas[2][0])/2; //consertar as médias, talvez não faça sentido pra teoria
	media_22=(sigmas[0][1] + sigmas[2][1])/2;
	cout << "Fazendo as médias, obtemos:\nsigma11= " << media_11 << "\nsigma22= " << media_22 << endl;
}

void menu(){
	string nome;
	int op;
	cout << "O que vc deseja fazer?\n";
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
	double n1,n2,n3;
	cout << "Seja bem vindo ao nosso Software de Cálculo de Tensão Residual :-)\n";
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