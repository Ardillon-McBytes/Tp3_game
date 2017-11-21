///*******************************************************************
//* Fichier : point.h						  
//* Auteur  : Rom�o Tsarafodiana-Bugeaud et Jean-Alain Sainton                                     
//* Date    : 13/09/2017                                    
//* But     : Ce fichier d'ent�te contient la classe
//            point qui a comme utilit� de manipuler 1 coordon�e x
//			  et 1 coordon�e y.  
//*******************************************************************/

/* Directives au pr�-processeur
============================ */
#pragma once
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
using namespace std;

class point							//manipules 1 coordon�e x et une coordon�e y.
{

private:
	double _abs;					//abscisse du point  (x)
	double _ord;					//ordonn�e du point  (y)
public:

	point();						//constructeur sans param�tres
	point(double abs, double ord);  //constructeur � 2 param�tre
	point(const point &p);			//constructeur de copie

	~point();						//destructeur 

									//getters
	double getAbs()const;			//retourne l'abscisse
	double getOrd()const;			//retourne l'ordon�e

	//setters
	void setAbs(double nombre);							//modifie la valeur de l'abscisse
	void setOrd(double nombre);							//modifie la valeur de l'ordon�e
	void setPoint(double nombreX, double nombreY);		//modifie la valeur du point

														//autres m�thodes
	void lirePoint(istream& input);			    		//lire un point � partir d'un fichier texte
	void afficher(ostream &sortie)const;				//afficher une coordon�e

														//op�rateurs
	bool operator==(const point &p)const;				//permet d'utiliser l'op�rateur ==
	bool operator!=(const point &p)const;				//permet d'utiliser l'op�rateur !=
	point operator=(const point &p);					//permet d'utiliser l'affectateur
};
ostream& operator<<(ostream& sortie, const point &p);			//Permet d'utiliser l'op�rateur <<
istream& operator >> (istream& entree, const point &p);			//Permet d'utiliser l'op�rateur >>
point::point()
{
	_abs = _ord = 0;
}

//constructeur � 2 param�tres
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

//retourne l'ordon�e
double point::getOrd()const
{
	return _ord;
}

//modifie l'abscisse
void point::setAbs(double x)
{
	_abs = x;
}

//modifie l'ordon�e
void point::setOrd(double y)
{
	_ord = y;
}

//modifie l'abcisse et l'ordon�e
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

	// Enleve  le premier caract�re de la string si c'est une parent�se ouverte
	if ('(' == coordonner[0])
	{
		coordonner[0] = ' ';
	}

	//Transforme la string en double et la met dans la variable _abs
	_abs = stod(coordonner, &sz);

	//Test pour v�rifier que les points sont bon
	cout << _abs << ", ";

	//met le reste de la ligne dans une string jusqu'a parent�se fermer
	getline(input, coordonner, ')');

	//Transforme la string en double et la met dans la variable _ord
	_ord = stod(coordonner, &sz);

	//Test pour v�rifier que les points sont bon
	cout << _ord << endl;

	//Prend le reste de la ligne (normalement vide mais necessaire)
	getline(input, coordonner);

}


//afficher une coordon�e
void point::afficher(ostream& sortie)const
{
	char parenthese1 = '(', parenthese2 = ')', virgule = ',';
	sortie << parenthese1 << _abs << virgule << _ord << parenthese2;
}

//utiliser l'op�rateur << pour afficher un point via la console
ostream& operator<<(ostream& sortie, const point &p)
{
	p.afficher(cout);
	return sortie;
}

//utiliser l'op�rateur >> pour lire (modifier) un point via un fichier texte
istream& operator >> (istream& sortie, point &p)
{
	p.lirePoint(sortie);
	return sortie;
}


//utiliser l'op�rateur == pour comparer 2 points. 
//Retourne vrai si �gaux, faux sinon
bool point::operator==(const point &p)const
{
	return _abs == p._abs && _ord == p._ord;
}

//utiliser l'op�rateur != pour comparer 2 points. 
//Retourne l'inverse de l'op�rateur ==
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
