/* En-tête du programme
====================
Programme:        test tableau 1
Auteur:           Jean-Alain Sainton
Date création:    31/01/2017
Description:     permet de stocker certain algorithme utile pour pouvoir les appeler plus facilement.*/

#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>		
#include <locale.h>
#include <string>


using namespace std;


void viderBuffer()																			//vide le flux
{
	cin.clear();
	cin.seekg(0, ios::end);

	if (!cin.fail())															
		cin.ignore();
	else																	
		cin.clear();
																	
}

void pause()														//Fait une pause et vide le buffer
{
	viderBuffer();
	cin.ignore();
}

void enter()															//Attend que l'utilisateur appuie sur une touche pour continue
{
	system("pause>0");
}

int saisirInt(string message)										//Valide la saisie d'un int
{
	int nombre;

	cout << message;
	viderBuffer();
	cin >> nombre;

	while (!cin)
	{																	//si le stream devient faux ou if(cin.fail())
		cout << endl << "Not a numeric value." << endl;
		viderBuffer();
		cout << message;
		cin >> nombre;
	}

	return nombre;
}

int saisirIntPositif(string message)											//Valide si l'int est positif
{
	int entier;

	entier = saisirInt(message);
	while (entier < 0)
	{
		cout << endl << "vous devez entrer un nombre positif :";
		entier = saisirInt(message);
	}

	return entier;
}

float saisirFloat(float nombre)													//Valide la saisie d'un float

{
	
	while (nombre <= 0)
	{
		viderBuffer();
		cout << " vous devez entrer une valeur positif et non nulle ";
		cin >> nombre;

	}
	return nombre;
}

float saisirFloatPositif(string message)													//Valide si le float est positif
{

	float entier;
	cout << message;
	cin >> entier;

	while (entier < 0)
	{
		viderBuffer();
		cout << endl << "vous devez entrer un nombre positif :";
		cin >> entier;

	}
	entier = saisirFloat(entier);

	return entier;
}

char saisirCharValide(string message, char valeur1, char valeur2)							//Valide la selection du char
{
	char sexe;
	viderBuffer();
	cout << message;																	
	sexe = cin.get();
	sexe = tolower(sexe);

	while (sexe != valeur1 && sexe != valeur2)
	{
		viderBuffer();
		cout << "vous devez entrez f ou g :";
		sexe = cin.get();
		sexe = tolower(sexe);
	}
	return sexe;
}

string saisirStringNonVide(string message)												//vérifie si la string est valide
{
	string nom;
	do
	{
		viderBuffer();
		cout << message;															
		getline(cin, nom);
	} while (nom.empty());

	return nom;

}

char questionOuiNon(string question)															//Valide la réponse une question

{
	char confirmation;
	cout << question;	
	confirmation = cin.get();
	confirmation = tolower(confirmation);
	while (confirmation != 'o' && confirmation != 'n')
	{
		viderBuffer();
		cout << "Entrez o pour recommencer et n pour fermer le programme" << endl;
		confirmation = cin.get();
	}




	return confirmation;
}

bool ouvrirFichier(ifstream &input, string nomFichier)				//Vérifie si le fichier existe
{
	 input.open(nomFichier);

	if (!input.good())												 // si le fichier est absent
	{

		cout << "Le fichier n’a pas été trouvé" << endl;
		enter();
		return 0;
	}
	else
		return 1;
}




bool siFichierVide(ifstream& input, string nomFichier)			//Vérifie si le fichier contient de l'information
{
	input.seekg(0, ios::end);
	int taille = input.tellg();
	input.seekg(0, ios::beg);
	if (taille == 0)											// si le fichier est vide
	{
		cout << "Le fichier est vide" << endl;	
		system("pause>0");
		input.close();
		return 1;
	}
	else
		return 0;
}




