#include <bits/stdc++.h>
using namespace std;

class Material{

	string nome;
	double Young;
	double poisson;
	double d0;

public:

	// Construtores
	Material(string n, double y, double p, double d):nome(n),Young(y),poisson(p),d0(d){}
	Material(){};

	// Getters e setters
	void setNome(string n);
	void setD0(double d);
	string getNome();
	double getYoung();
	double getPoisson();
	double getD0();
};

void Material::setD0(double d){
	d0 = d;
}

double Material::getD0(){
	return d0;
}

double Material::getYoung(){
	return Young;
}

void Material::setNome(string n){
	nome = n;
}

string Material::getNome(){
	return nome;
}

double Material::getPoisson(){
	return poisson;
}


