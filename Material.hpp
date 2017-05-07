#include <bits/stdc++.h>
using namespace std;

class Material{

	string nome;
	float Young;
	float poisson;
	float d0;

public:

	// Construtores
	Material(string n, int y, int p, int d):nome(n),Young(y),poisson(p),d0(d){}
	Material(){};

	// Getters e setters
	void setNome(string n);
	void setD0(float d);
	string getNome();
	float getYoung();
	float getPoisson();
	float getD0();
};

void Material::setD0(float d){
	d0 = d;
}

float Material::getD0(){
	return d0;
}

float Material::getYoung(){
	return Young;
}

void Material::setNome(string n){
	nome = n;
}

string Material::getNome(){
	return nome;
}

float Material::getPoisson(){
	return poisson;
}


