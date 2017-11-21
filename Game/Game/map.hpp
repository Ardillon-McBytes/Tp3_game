/* En-tête du programme
====================
Programme:			tableau.hpp
Auteur:				Jean-Alain Sainton et Francis Forest
Date création:		06-11-2017
Description :		Objet composé d'une matrice dynamique à 2 dimensions. La matrice comprend
					un nombre de colonnes, un nombre de lignes, un nom et son contenu dépend
					du type de donnée utilisée lors de la construction de la tableau. Celle-ci peut
					être construite avec un flux d'information en entrée, être affichée ou modifiée.
*/

#pragma once
#include "modifierConsole.h"
#include <string>
#include <cassert>
#include <iostream>
#include "point.h"
using namespace std;

template <class TYPE>
class tableau
{
protected:
	char *_name;								//pointeur sur le nom de la tableau
	TYPE **_tableau;								//La tableau dynamique en ligne et colonne
	int  _nbLine, 								//nombre de ligne pour la matrice
		_nbCol;									//nombre de colonne pour la matrice
	point _start,								//position du départ du labyrinthe
		_end;									//position de la fin du labyrinthe

public:
	tableau();										//constructeur par défault
	tableau(const char* name, int line, int col);   //constructeur avec 3 parametres
	tableau(const tableau<TYPE>& tableau);					//constructeur de copie
	~tableau();										//destructeur

	void clear();								//vide toute les valeurs
	
	int nbLine()const; 							//retourne le nb de ligne
	int nbCol()const; 							//retourne le nb de colonne
	const char* getName()const; 	  			//retourne le nom de la tableau

												// setteurs
	void resize(int nbLine, int nbCol);			//resize la matrice avec nouv dims
	char* & tableau<TYPE>::setName(const char * name);  			//modifie le nom de la tableau
	TYPE& at(int posI, int posJ)const;			//retourne une référence à l’élément 
												//à la position i,j pour accéder ou modifier
												//autres
	void print(ostream& sortie)const;  			//print la matrice (sans le nom)
	void read(istream& entree);					//lit la matrice de la tableau ds le flux
	
												//opérateur
	const tableau<TYPE>& operator=(const tableau<TYPE>& tableau); //opérateur =
	TYPE* operator[] (int line);				//opérateur []

};

//opérateur << pour afficher le contenue de tableau
template <class TYPE>
ostream& operator<<(ostream& sortie, const tableau<TYPE>& x)
{
	x.print(sortie);
	return sortie;
}

//opérateur >> pour lire les valeurs a mettre dans la tableau
template <class TYPE>
istream& operator >> (istream& entree, tableau<TYPE>& x)
{
	x.read(entree);
	return entree;
}

//Constructeur par défaut
template<class TYPE>
tableau<TYPE>::tableau()
{
	_nbLine = 0;
	_nbCol = 0;
	_name = nullptr;
	_tableau = nullptr;
}

//constructeur avec un nom et une taille
template<class TYPE>
tableau<TYPE>::tableau(const char * name, int line, int col)
{
	assert(line >= 0 && col >= 0);

	_tableau = nullptr;								//vide la tableau
	_name = nullptr;

	_nbLine = line;								//set la taillde de la tableau
	_nbCol = col;

	if (_nbLine > 0 && _nbCol > 0)
	{
		_tableau = new TYPE*[_nbLine];				//créé des lignes selon le nombre de lignes

		for (size_t i = 0; i < _nbLine; i++)	//pr ch ligne
		{
			*(_tableau + i) = new TYPE[_nbCol];		//ajoute des colonnes à la ligne

			for (size_t j = 0; j < _nbCol; j++)
			{
				*(*(_tableau + i) + j) = NULL;		//chaque valeur de la tableau est NULL
			}
		}
	}
	
	int nbrChar = strlen(name);

	if (nbrChar > 0)							//nom non null
	{
		_name = new char[nbrChar + 1];
		strcpy_s(_name, nbrChar + 1, name);
	}
}

//constructeur de copie
template<class TYPE>
tableau<TYPE>::tableau(const tableau<TYPE>& tableau)
{
	_nbCol = tableau._nbCol;
	_nbLine = tableau._nbLine;
	_start = tableau._start;
	_end = tableau._end;
	int nameSize;

	if (tableau._name != nullptr)					//nom non null
	{
		nameSize = strlen(tableau._name);

		if (nameSize > 0)						//nom non null
		{
			_name = new char[nameSize + 1];
			strcpy_s(_name, nameSize + 1, tableau._name);
		}
	}

	if (_nbCol != 0 && _nbLine != 0)			//au moins 1 ligne et 1 colonne existe
	{
		_tableau = new TYPE*[_nbLine];				//création de lignes selon le nombre de lignes

		for (size_t i = 0; i < _nbLine; i++)	//pr ch ligne
		{
			*(_tableau + i) = new TYPE[_nbCol];		//création d'une ligne selon le nombre de colonnes
			
			for (size_t j = 0; j < _nbCol; j++)	//pr ch colonne de ch ligne
			{
				*(*(_tableau + i) + j) = *(*(tableau._tableau + i) + j);	//copie des valeurs de oldtableau
			}
		}
	}
}

//destructeur
template<class TYPE>
inline tableau<TYPE>::~tableau()
{
	clear();
}

//met les valeurs a 0 et détruie les pointeurs
template<class TYPE>
inline void tableau<TYPE>::clear()
{
	delete[] _name;								//delete le nom
	_name = nullptr;

	for (size_t i = 0; i < _nbLine; i++)		//pr ch ligne
	{
		delete[] * (_tableau + i);					//détruit la ligne
		*(_tableau + i) = nullptr;
	}

	delete[] _tableau;								//delete la tableau		
	_tableau = nullptr;

	_nbLine = 0;
	_nbCol = 0;
}

//retourne le nombre de ligne
template<class TYPE>
int tableau<TYPE>::nbLine() const
{
	return _nbLine;
}

//retourne le nombre de colonne
template<class TYPE>
int tableau<TYPE>::nbCol() const
{
	return _nbCol;
}

//modifie le nombre de ligne et de collone
template<class TYPE>
inline void tableau<TYPE>::resize(int nbLine, int nbCol)
{
	assert(nbLine >= 0 && nbCol >= 0);
	if (nbLine == _nbLine && nbCol == _nbCol)	//aucun changement des dimensions
	{
		return;
	}

	if (nbLine == 0 || nbCol == 0)				//line ou y = 0 --> on clear la tableau
	{
		clear();
		return;
	}

	// sauvegarde les données de _tableau
	TYPE **oldtableau = new TYPE*[nbLine];

	for (size_t i = 0; i < nbLine; i++)			//pr ch ligne
	{
		*(oldtableau + i) = new TYPE[nbCol];		//création d'une ligne avec le nombre de colonnes de la tableau

	}

	for (size_t i = 0; i < _nbLine; i++)		//pr ch ligne
	{
		for (size_t j = 0; j < _nbCol; j++)		//pr ch colonne
		{

			*(*(oldtableau + i) + j) = *(*(_tableau + i) + j);	//copie des valeurs déjà présente dans la old tableau

		}
	}

	//vide la tableau
	for (size_t i = 0; i < _nbLine; i++)		//pr ch ligne
	{
		delete[] * (_tableau + i);					//delete ligne de la old tableau
		*(_tableau + i) = nullptr;
	}
	delete[]_tableau;
	_tableau = nullptr;

	_nbCol = nbCol;
	_nbLine = nbLine;

	_tableau = oldtableau;
	oldtableau = nullptr;

}

//retourne la valeur de la ligne I collone J
template<class TYPE>
inline TYPE & tableau<TYPE>::at(int i, int j) const
{
	assert(i >= 0 && i < _nbLine && j >= 0 && j < _nbCol);	//x et y doivent être dans la size de la tableau

	return *(*(_tableau + i) + j);
}

//retourne la nom de la tableau
template<class TYPE>
inline const char * tableau<TYPE>::getName() const
{
	if (_name == nullptr || _name == "")			//tableau sans nom
	{
		return "\0";								//car d'échapement
	}
	return _name;
}

//modifie le nom de la tableau
template<class TYPE>
char* & tableau<TYPE>::setName(const char * name)
{

	delete[] _name;									//delete le nom
	_name = nullptr;

	if (name != nullptr)							//nom non null
	{
		int nbrChar = strlen(name);
		if (nbrChar > 0)							//nom non null									//A MODIFIER CHECK NULL ET NAMESIZE > 0 REDONDANT
		{
			_name = new char[nbrChar + 1];
			strcpy_s(_name, nbrChar + 1, name);
		}
	}

	return _name;
}

//Affiche le contenue de la tableau au milieu de la console
template<class TYPE>
inline void tableau<TYPE>::print(ostream & sortie) const
{

	while (nbCol() > getTailleConsoleX())
	{
		setFontSize(getFontSizeX() - 2, getFontSizeX() - 2);
		SetTailleMaxEcran();				//necessaire pour un bon affichage
	}
			

	int	sizeX = getPosXMid() - (_nbCol / 2);
	int sizeY = 0;


	for (size_t i = 0; i < _nbLine; i++)				//pr ch ligne
	{
		gotoxy(sizeX, sizeY);
		for (size_t j = 0; j < _nbCol; j++)				//pr ch col
		{
			if (*(*(_tableau + i) + j) == 48 || *(*(_tableau + i) + j) == 0)				//48 ASCII = 0
				sortie << '\0';
			else if (*(*(_tableau + i) + j) == 48 || *(*(_tableau + i) + j) == 2)
			{
				sortie << "*";
			}
			else if (*(*(_tableau + i) + j) == 48 || *(*(_tableau + i) + j) == 3)
			{
				sortie << "|";
			}
			else
				sortie << static_cast<char>(219);		//219 ASCII = BLOCK
		}
		sizeY++;
		sortie << endl;
	}
}

//lis un fichier texte contenant un nombre de ligne, un nombre de collone et une matrice
//et place les valeurs dans la tableau
template<class TYPE>
inline void tableau<TYPE>::read(istream & entree)
{
	clear();

	entree >> _nbLine;
	entree >> _nbCol;

	assert(_nbLine > 0 && _nbCol > 0);

	TYPE temp;

	_tableau = new TYPE*[_nbLine];

	for (size_t i = 0; i < _nbLine; i++)			//pr ch ligne
	{
		*(_tableau + i) = new TYPE[_nbCol];				//nouvelle ligne de longueur col

		for (size_t j = 0; j < _nbCol; j++)			//pr ch col
		{
				entree >> *(*(_tableau + i) + j);		//fill la tableau avec le bon caractere
		}
	}
}

//affectateur
template<class TYPE>
const tableau<TYPE>& tableau<TYPE>::operator=(const tableau<TYPE>& tableau)
{
	if (this == &tableau)							//même tableau, on ne fait rien
		return *this;

	clear();

	_nbCol = tableau._nbCol;
	_nbLine = tableau._nbLine;

	setName(tableau._name);

	if (_nbLine > 0 && _nbCol > 0)		
	{

		_tableau = new TYPE*[_nbLine];				//création de la tableau selon le nombre de lignes

		for (size_t i = 0; i < _nbLine; i++)	//pr ch ligne
		{

			*(_tableau + i) = new TYPE[_nbCol];		//création d'une ligne avec le nombre de colonnes selon _nbCol

			for (size_t j = 0; j < _nbCol; j++)	//pr ch colonne
			{
				*(*(_tableau + i) + j) = *(*(tableau._tableau + i) + j);	//copie les valeurs de la oldtableau
			}
		}
	}
	return *this;
}

//opérateur []
template<class TYPE>
TYPE* tableau<TYPE>::operator[](int pos)
{
	assert(pos >= 0);
	return *(_tableau + pos);
}
