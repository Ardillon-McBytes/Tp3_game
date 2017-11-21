///*******************************************************************
//* Fichier : point.h						  
//* Auteur  : Roméo Tsarafodiana-Bugeaud et Jean-Alain Sainton                                     
//* Date    : 13/09/2017                                    
//* But     : Ce fichier d'entête contient la classe
//            point qui a comme utilité de manipuler 1 coordonée x
//			  et 1 coordonée y.  
//*******************************************************************/

/* Directives au pré-processeur
============================ */
#pragma once
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
using namespace std;

class point							//manipules 1 coordonée x et une coordonée y.
{

private:
	double _abs;					//abscisse du point  (x)
	double _ord;					//ordonnée du point  (y)
public:

	point();						//constructeur sans paramètres
	point(double abs, double ord);  //constructeur à 2 paramètre
	point(const point &p);			//constructeur de copie

	~point();						//destructeur 

									//getters
	double getAbs()const;			//retourne l'abscisse
	double getOrd()const;			//retourne l'ordonée

	//setters
	void setAbs(double nombre);							//modifie la valeur de l'abscisse
	void setOrd(double nombre);							//modifie la valeur de l'ordonée
	void setPoint(double nombreX, double nombreY);		//modifie la valeur du point

														//autres méthodes
	void lirePoint(istream& input);			    		//lire un point à partir d'un fichier texte
	void afficher(ostream &sortie)const;				//afficher une coordonée

														//opérateurs
	bool operator==(const point &p)const;				//permet d'utiliser l'opérateur ==
	bool operator!=(const point &p)const;				//permet d'utiliser l'opérateur !=
	point operator=(const point &p);					//permet d'utiliser l'affectateur
};
ostream& operator<<(ostream& sortie, const point &p);			//Permet d'utiliser l'opérateur <<
istream& operator >> (istream& entree, const point &p);			//Permet d'utiliser l'opérateur >>
point::point()
{
	_abs = _ord = 0;
}

//constructeur à 2 paramètres
point::point(double x, double y)
{
	_abs = x;
	_ord = y;
}

//destructeur
point::~point()
{
	_abs = _ord = 0;
}

//constructeur de copie
point::point(const point &p)
{
	_abs = p._abs;
	_ord = p._ord;
}

//retourne l'abscisse
double point::getAbs()const
{
	return _abs;
}

//retourne l'ordonée
double point::getOrd()const
{
	return _ord;
}

//modifie l'abscisse
void point::setAbs(double x)
{
	_abs = x;
}

//modifie l'ordonée
void point::setOrd(double y)
{
	_ord = y;
}

//modifie l'abcisse et l'ordonée
void point::setPoint(double x, double y)
{
	_abs = x;
	_ord = y;
}

//lire un point dans un fichier texte
void point::lirePoint(istream& input)
{
	string coordonner = "\n";
	string::size_type sz; //pour la fonction stod

						  //met dans une string la premiere ligne jusqu'a la virgule
	getline(input, coordonner, ',');

	// Enleve  le premier caractère de la string si c'est une parentèse ouverte
	if ('(' == coordonner[0])
	{
		coordonner[0] = ' ';
	}

	//Transforme la string en double et la met dans la variable _abs
	_abs = stod(coordonner, &sz);

	//Test pour vérifier que les points sont bon
	cout << _abs << ", ";

	//met le reste de la ligne dans une string jusqu'a parentèse fermer
	getline(input, coordonner, ')');

	//Transforme la string en double et la met dans la variable _ord
	_ord = stod(coordonner, &sz);

	//Test pour vérifier que les points sont bon
	cout << _ord << endl;

	//Prend le reste de la ligne (normalement vide mais necessaire)
	getline(input, coordonner);

}


//afficher une coordonée
void point::afficher(ostream& sortie)const
{
	char parenthese1 = '(', parenthese2 = ')', virgule = ',';
	sortie << parenthese1 << _abs << virgule << _ord << parenthese2;
}

//utiliser l'opérateur << pour afficher un point via la console
ostream& operator<<(ostream& sortie, const point &p)
{
	p.afficher(cout);
	return sortie;
}

//utiliser l'opérateur >> pour lire (modifier) un point via un fichier texte
istream& operator >> (istream& sortie, point &p)
{
	p.lirePoint(sortie);
	return sortie;
}


//utiliser l'opérateur == pour comparer 2 points. 
//Retourne vrai si égaux, faux sinon
bool point::operator==(const point &p)const
{
	return _abs == p._abs && _ord == p._ord;
}

//utiliser l'opérateur != pour comparer 2 points. 
//Retourne l'inverse de l'opérateur ==
bool point::operator!=(const point &p)const
{
	return !operator==(p);
}

//affecter un point a un autre 
point point::operator=(const point &p)
{
	_abs = p._abs;
	_ord = p._ord;

	return p;
}
