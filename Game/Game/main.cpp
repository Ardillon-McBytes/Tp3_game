/* En-tête du programme */
///==================== */
/* Programme:		tutoGitsSFML.cpp
// Auteur:			Jean-Alain Sainton & Olivier Lemay Dostie
// Date création:	16/04/2017
// Description:		Programme des essais SFML et de connection Git pour le projet final en C++
// .				Espace de développement pour l'application.*/

/* Directives au pré-processeur */
///============================ */
#pragma once
#include "saisieSecurisee.h"	//Pour la fonction ouvrirFichier
#include <iomanip>	
#include <fstream>
#include <locale>				
#include <string>				
#include <vector>				
#include <math.h>	
#include <cstdlib>				
using namespace std;
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>		
#include <SFML\Window.hpp>
#include<SFML\System.hpp>
using namespace sf;
//#include<client.h> // pour la communication avec le serveur
#include "map.hpp"

/* Prototypes des fonctions */

const int NBRNIVEAU = 5;
const int NBRPOUVOIR = 10;
const int NBRPERSONNAGE = 15;

struct skill
{
	string cheminImg[3];
	Text description[3];
};

struct personnage
{
	string nom;
	string classe;
	string description;
	int lvl = 1;
	int exp = 0;
	RectangleShape img;
	Texture fondImage;
	Text text;
	Font font;
	skill skill;
};
struct joueur
{
	string nomJoueur,
		mdpJoueur;

	int argent = 0,
		score[NBRNIVEAU] = { 0 },      // Score atteint
		pouvoir[NBRPOUVOIR][2],		//Numero pouvoir | acheter(1) ou non (2)
		rang = 0,
		personnage = 0;
	bool nouveauJoueur = true;
};

struct boutonClick
{
	int posXRect = 350,
		posYRect = 50,
		hauteur = 50,
		largeur = 300,
		posXTexte = 375,
		posYTexte = 75;
	RectangleShape rectangle;
	Texture fondImage;
	Text text;
	Font font;
};

int menuLogIn(RenderWindow & window, joueur &joueur, Font font, int &HAUTEURWINDOW, int &LARGEURWINDOW);
string saisieTexte(RenderWindow & window, RectangleShape rectangle, Text &text, Font &font);
string saisieTexteCacher(RenderWindow & window, RectangleShape rectangle, Text &text, Font &font, string &MDP);
int listeEnregistrement(RenderWindow & window2, joueur & joueur);
int enregistrement(joueur joueur);
void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille);
int fenetreAvertissement(Font font, string message, int LARGEURWINDOW, int HAUTEURWINDOW);
void selectionPersonnage(RenderWindow & window, Font font, joueur &joueur, int HAUTEURWINDOW, int LARGEURWINDOW);
void fenetreErreur(Font font, string message, int LARGEURWINDOW, int HAUTEURWINDOW);
void afficherListePerso(RenderWindow &window, RectangleShape &rectangle, Font font, personnage personnage[NBRPERSONNAGE], string cheminImg[NBRPERSONNAGE]);
void initPersonnage(personnage personnage[NBRPERSONNAGE], string cheminImg[NBRPERSONNAGE], Texture texture[NBRPERSONNAGE]);
void bougerPerso(RenderWindow &window, Sprite &sprite2, RectangleShape &rectangle, int direction, Clock &clock);
void resize(RenderWindow &window, int &LARGEURWINDOW, int &HAUTEURWINDOW);
void createRoad(tableau<int> &laby, point &start, point &end);
void createSubRoad(tableau<int> &laby, point &startPos, point &endPos);
void main()
{

	joueur joueur;
	RenderWindow window;
	tableau<int> labyrinthe;
	window.create(VideoMode::getDesktopMode(), "Log In");

	srand(time(NULL));
	labyrinthe.resize(20, 20);
	int randomNbr;
	int randomNbr2;
	point startPos(10, 1);
	point endPos(18, 18);

	//init la map
	for (size_t i = 0; i < labyrinthe.nbLine(); i++)
	{

		for (size_t j = 0; j < labyrinthe.nbCol(); j++)
		{
			labyrinthe.at(i, j) = 1;
		}
	}
	for (size_t i = 1; i < labyrinthe.nbLine() - 1; i++)
	{

		for (size_t j = 1; j < labyrinthe.nbCol() - 1; j++)
		{
			labyrinthe.at(i, j) = 0;
		}
	}

	createRoad(labyrinthe, startPos, endPos);

	//trouve un start et une fin pour une subroad
	for (size_t i = 0; i < 5; i++)
	{

		do
		{
			randomNbr = rand() % 18 + 1;
			randomNbr2 = rand() % 18 + 1;

		} while (labyrinthe.at(randomNbr, randomNbr2) != 0);
		startPos.setPoint(randomNbr2, randomNbr);
		do
		{
			randomNbr = rand() % 18 + 1;
			randomNbr2 = rand() % 18 + 1;

		} while (labyrinthe.at(randomNbr, randomNbr2) != 2);
		endPos.setPoint(randomNbr2, randomNbr);

		createSubRoad(labyrinthe, startPos, endPos);
	}

	//set les mur autour des chemins
	for (size_t i = 0; i < labyrinthe.nbLine(); i++)
	{

		for (size_t j = 0; j < labyrinthe.nbCol(); j++)
		{
			if (labyrinthe.at(i,j )== 0)
			{
				labyrinthe.at(i, j) = 1;
			}
			
		}
	}

	labyrinthe.print(cout);


	int HAUTEURWINDOW = window.getSize().y;
	int LARGEURWINDOW = window.getSize().x;
	Font font;
	if (!font.loadFromFile("Sketch_College.ttf"))
		cout << "error";		//Si la police n'est pas trouvable, une erreur est afficher dans la console
	int debutant = 1;
	int choixMenu = 0,
		choixNiveau = 0,
		mouvement = 0, // (1=haut, 2=bas, 3=gauche 4=droite)
		action = 0, // ()
		typeMonstre = 0,
		niveauChoisie = 0;


	debutant = menuLogIn(window, joueur, font, HAUTEURWINDOW, LARGEURWINDOW);
	if (debutant == 1 || joueur.personnage == 0)
	{
		selectionPersonnage(window, font, joueur, HAUTEURWINDOW, LARGEURWINDOW);
	}
}
void createSubRoad(tableau<int> &laby, point &startPos, point &endPos)
{
	point nextPos;
	int randomNbr;
	int randomNbr2;

	laby.at(startPos.getOrd(), startPos.getAbs()) = 1;
	laby.at(endPos.getOrd(), endPos.getAbs()) = 0;
	for (size_t i = 0; i < laby.nbCol(); i++)
	{
		system("cls");
		laby.print(cout);
		if (nextPos.getOrd() == endPos.getOrd())
		{
			if (endPos.getAbs() < startPos.getAbs())
				nextPos.setPoint(startPos.getAbs() - 1, startPos.getOrd());

			else if (endPos.getAbs() > startPos.getAbs())
				nextPos.setPoint(startPos.getAbs() + 1, startPos.getOrd());

			else if (endPos.getOrd() < startPos.getOrd())
				nextPos.setPoint(startPos.getAbs(), startPos.getOrd() - 1);

			else
				nextPos.setPoint(startPos.getAbs(), startPos.getOrd() + 1);

		}
		else
		{


			randomNbr = rand() % 3 + 1;
			switch (randomNbr)
			{
			case 1:
				nextPos.setPoint(startPos.getAbs() - 1, startPos.getOrd());
				break;
			case 2:
				nextPos.setPoint(startPos.getAbs() + 1, startPos.getOrd());
				break;
			case 3:
				nextPos.setPoint(startPos.getAbs(), startPos.getOrd() + 1);
				break;
			case 4:

				break;
			default:
				break;
			}
		}
		if (laby.at(nextPos.getOrd(), nextPos.getAbs()) == 0)
		{
			laby.at(startPos.getOrd(), startPos.getAbs()) = 2;
			startPos = nextPos;

		}
	}
}

void createRoad(tableau<int> &laby, point &startPos, point &endPos)
{

	point nextPos;
	int randomNbr;
	int randomNbr2;

	laby.at(startPos.getOrd(), startPos.getAbs()) = 1;
	laby.at(endPos.getOrd(), endPos.getAbs()) = 0;
	while (startPos != endPos)
	{
		system("cls");
		laby.print(cout);
		if (nextPos.getOrd() == endPos.getOrd())
		{
			if (endPos.getAbs() < startPos.getAbs())
				nextPos.setPoint(startPos.getAbs() - 1, startPos.getOrd());

			else if (endPos.getAbs() > startPos.getAbs())
				nextPos.setPoint(startPos.getAbs() + 1, startPos.getOrd());

			else if (endPos.getOrd() < startPos.getOrd())
				nextPos.setPoint(startPos.getAbs(), startPos.getOrd() - 1);

			else
				nextPos.setPoint(startPos.getAbs(), startPos.getOrd() + 1);

		}
		else
		{


			randomNbr = rand() % 3 + 1;
			switch (randomNbr)
			{
			case 1:
				nextPos.setPoint(startPos.getAbs() - 1, startPos.getOrd());
				break;
			case 2:
				nextPos.setPoint(startPos.getAbs() + 1, startPos.getOrd());
				break;
			case 3:
				nextPos.setPoint(startPos.getAbs(), startPos.getOrd() + 1);
				break;
			case 4:

				break;
			default:
				break;
			}
		}
		if (laby.at(nextPos.getOrd(), nextPos.getAbs()) == 0)
		{
			if (nextPos == endPos)
			{
				laby.at(startPos.getOrd(), startPos.getAbs()) = 3;
			}
			else
			{
				laby.at(startPos.getOrd(), startPos.getAbs()) = 2;
				
			}
			startPos = nextPos;

		}
		if (laby.at(startPos.getOrd(), startPos.getAbs() + 1) != 0 &&
			laby.at(startPos.getOrd(), startPos.getAbs() - 1) != 0 &&
			laby.at(startPos.getOrd() + 1, startPos.getAbs()) != 0 &&
			laby.at(startPos.getOrd() - 1, startPos.getAbs()) != 0)
			return;
	}


}
int menuLogIn(RenderWindow & window, joueur &joueur, Font font, int &HAUTEURWINDOW, int &LARGEURWINDOW)
{
	int argc = 0;
	char *argv[15];


	/*conectionWinsock(argc, argv);*/


	Texture texture;

	if (!texture.loadFromFile("background.png"));		//Si la texture n'est pas trouvable, une erreur est afficher dans la console
	Texture textureBtnIntro;
	if (!textureBtnIntro.loadFromFile("telechargement.jpg"));	//Si la texture n'est pas trouvable, une erreur est afficher dans la console


	Sprite background(texture);

	Text infoUtilisateur[2];	//Nom + mdp
	const int NBRTEXT = 2;		//Nom + mdp
	const int NBRBOUTON = 4;	//Nom + mdp + continuer + quitter
	boutonClick bouton[NBRBOUTON];
	while (window.isOpen())
	{
		window.draw(background);
		for (size_t i = 0; i < NBRBOUTON; i++)
		{
			bouton[i].rectangle.setFillColor(Color::White);
			bouton[i].rectangle.setOutlineThickness(5);
			bouton[i].rectangle.setOutlineColor(Color::Red);
			bouton[i].text.setCharacterSize(30);
			bouton[i].text.setStyle(Text::Bold);
			bouton[i].text.setFillColor(Color::Green);
			bouton[i].fondImage.setSmooth(true);
			bouton[i].text.setFont(font);
			bouton[i].rectangle.setSize(Vector2f(350, 150));
			bouton[i].rectangle.setTexture(&textureBtnIntro);
		}

		bouton[0].rectangle.setPosition((LARGEURWINDOW / 2) - (bouton[0].rectangle.getSize().x / 2),		//Position du bouton Nom
			HAUTEURWINDOW / 3 - (bouton[0].rectangle.getSize().y / 2));

		bouton[1].rectangle.setPosition((LARGEURWINDOW / 2) - (bouton[1].rectangle.getSize().x / 2),		//Position du bouton mdpNom
			HAUTEURWINDOW / 3 - (bouton[1].rectangle.getSize().y / 2) + (bouton[0].rectangle.getSize().y * 2));

		bouton[2].rectangle.setPosition((LARGEURWINDOW / 4), HAUTEURWINDOW - (HAUTEURWINDOW / 4));			//Position du bouton Continuer
		bouton[3].rectangle.setPosition((LARGEURWINDOW / 1.75), HAUTEURWINDOW - (HAUTEURWINDOW / 4));		//Position du bouton Quitter

		int btnNomX = bouton[0].rectangle.getPosition().x - ((bouton[0].rectangle.getSize().x));		//Position du Texte pour le nom en X
		int btnNomy = bouton[0].rectangle.getPosition().y + (bouton[0].rectangle.getSize().y / 2.5);	//Position du Texte pour le nom en Y
		int btnMdpX = bouton[1].rectangle.getPosition().x - ((bouton[1].rectangle.getSize().x));		//Position du Texte pour le mdp en X
		int btnMdpy = bouton[1].rectangle.getPosition().y + (bouton[1].rectangle.getSize().y / 2.5);	//Position du Texte pour le mdp en Y

		int btnConectX = bouton[2].rectangle.getPosition().x + ((bouton[2].rectangle.getSize().x / 3)); //Position du Texte pour le bouton Continuer en X
		int btnConectY = bouton[2].rectangle.getPosition().y + (bouton[2].rectangle.getSize().y / 2.5);	//Position du Texte pour le bouton Continuer en Y
		int btnQuitX = bouton[3].rectangle.getPosition().x + ((bouton[3].rectangle.getSize().x / 3));	//Position du Texte pour le bouton Quitter en X
		int btnQuitY = bouton[3].rectangle.getPosition().y + (bouton[3].rectangle.getSize().y / 2.5);	//Position du Texte pour le bouton Quitter en Y


		bouton[0].text.setPosition(btnNomX, btnNomy);
		bouton[1].text.setPosition(btnMdpX, btnMdpy);
		bouton[2].text.setPosition(btnConectX, btnConectY);
		bouton[3].text.setPosition(btnQuitX, btnQuitY);


		bouton[0].text.setString("Nom du joueur" + '\0');
		bouton[1].text.setString("Mot de passe" + '\0');
		bouton[2].text.setString("Connecter" + '\0');
		bouton[3].text.setString("Quitter" + '\0');

		Event event;
		int reponse = 0, reponse2 = 0;
		while (window.waitEvent(event))
		{
			window.draw(background);
			for (size_t i = 0; i < NBRBOUTON; i++)
			{
				window.draw(bouton[i].rectangle);
				window.draw(bouton[i].text);
			}

			for (size_t i = 0; i < NBRTEXT; i++)
			{
				window.draw(infoUtilisateur[i]);
			}


			window.display();
			for (size_t i = 0; i < NBRBOUTON; i++)
			{
				if (
					(event.mouseMove.x > bouton[i].rectangle.getPosition().x		//Si le curseur est sur un des bouton
						&& event.mouseMove.x < (bouton[i].rectangle.getPosition().x + bouton[i].rectangle.getSize().x))
					&& (event.mouseMove.y > bouton[i].rectangle.getPosition().y
						&& event.mouseMove.y < (bouton[i].rectangle.getPosition().y + bouton[i].rectangle.getSize().y)))
				{

					bouton[i].rectangle.setOutlineColor(Color::Blue);
				}
				else
				{
					bouton[i].rectangle.setOutlineColor(Color::Red);
				}
			}

			/*	if (event.type == Event::Resized)
			{
			resize(window, HAUTEURWINDOW, LARGEURWINDOW);

			}*/
			switch (event.type)
			{
			case Event::Closed:
				exit(0);


			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{// S l'utilisateur appuis que la touche echap, ferme le menu d'option et retourne au menu ou au jeu
					window.setVisible(true);
					exit(0);
				}

			case Event::MouseButtonPressed:
				Mouse::getPosition(window);
				//Clique sur le bouton Nom
				if ((event.mouseButton.x > bouton[0].rectangle.getPosition().x && event.mouseButton.x < (bouton[0].rectangle.getPosition().x + bouton[0].rectangle.getSize().x))
					&& (event.mouseButton.y > bouton[0].rectangle.getPosition().y && event.mouseButton.y < (bouton[0].rectangle.getPosition().y + bouton[0].rectangle.getSize().y)))
				{
					bouton[0].rectangle.setFillColor(Color::Magenta);
					window.draw(infoUtilisateur[0]);
					window.draw(bouton[0].rectangle);
					window.display();
					infoUtilisateur[0].setFillColor(Color::White);
					infoUtilisateur[0].setString(saisieTexte(window, bouton[0].rectangle, infoUtilisateur[0], font));

					joueur.nomJoueur = infoUtilisateur[0].getString();
					bouton[0].rectangle.setFillColor(Color::White);
					infoUtilisateur[0].setFillColor(Color::Black);
				}
				//Clique sur le bouton Mdp
				else if ((event.mouseButton.x > bouton[1].rectangle.getPosition().x && event.mouseButton.x < (bouton[1].rectangle.getPosition().x + bouton[1].rectangle.getSize().x))
					&& (event.mouseButton.y > bouton[1].rectangle.getPosition().y && event.mouseButton.y < (bouton[1].rectangle.getPosition().y + bouton[1].rectangle.getSize().y)))
				{
					bouton[1].rectangle.setFillColor(Color::Magenta);
					infoUtilisateur[1].setFillColor(Color::White);
					window.draw(infoUtilisateur[1]);
					window.draw(bouton[1].rectangle);
					window.display();
					infoUtilisateur[1].setString(saisieTexteCacher(window, bouton[1].rectangle, infoUtilisateur[1], font, joueur.mdpJoueur));
					bouton[1].rectangle.setFillColor(Color::White);
					infoUtilisateur[1].setFillColor(Color::Black);

				}
				//Clique sur le bouton Continuer
				else if ((event.mouseButton.x > bouton[2].rectangle.getPosition().x && event.mouseButton.x < (bouton[2].rectangle.getPosition().x + bouton[2].rectangle.getSize().x))
					&& (event.mouseButton.y > bouton[2].rectangle.getPosition().y && event.mouseButton.y < (bouton[2].rectangle.getPosition().y + bouton[2].rectangle.getSize().y)))
				{

					if (joueur.nomJoueur.size() > 3 && joueur.mdpJoueur.size() > 3)
					{


						int reponse = 0;
						reponse = listeEnregistrement(window, joueur);
						if (reponse == 2)
						{
							reponse2 = fenetreAvertissement(font, "Enregistrement introuvable \nCréer un nouveau profil?", LARGEURWINDOW, HAUTEURWINDOW);
							if (reponse2 == 1)
							{
								enregistrement(joueur);
								return 1;
							}

						}
						else if (reponse == 3)
						{

							fenetreErreur(font, "Utilisateur ou \nMot de passe incorrect", LARGEURWINDOW, HAUTEURWINDOW);

						}
						else if (reponse == 1)
						{
							return 2;
						}
					}

				}
				//Clique sur le bouton Quitter
				else if ((event.mouseButton.x > bouton[3].rectangle.getPosition().x && event.mouseButton.x < (bouton[3].rectangle.getPosition().x + bouton[3].rectangle.getSize().x))
					&& (event.mouseButton.y > bouton[3].rectangle.getPosition().y && event.mouseButton.y < (bouton[3].rectangle.getPosition().y + bouton[3].rectangle.getSize().y)))
				{
					exit(0);

				}

			}

		}
	}
}

string saisieTexte(RenderWindow & window, RectangleShape rectangle, Text &text, Font &font)
{
	Event event;
	string s = text.getString();
	text.setPosition(rectangle.getPosition().x + 15, rectangle.getPosition().y + (rectangle.getSize().y / 2.5));
	text.setFont(font);

	text.setString(s); 	// choix de la chaîne de caractères à afficher

	window.draw(rectangle);	//Affiche un rectangle vide
	window.draw(text);
	window.display();
	int test1 = 0;
	int test2 = 0;
	int test3 = 0;
	int test4 = 0;
	int test5 = 0;
	int test6 = 0;
	while (window.waitEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
			exit(0);
		case Event::MouseButtonPressed:
			Mouse::getPosition(window);
			test1 = event.mouseButton.x;
			test2 = event.mouseButton.y;
			test3 = rectangle.getPosition().x;
			test4 = rectangle.getPosition().y;
			test5 = rectangle.getPosition().x + rectangle.getSize().x;
			test6 = rectangle.getPosition().y + rectangle.getSize().y;
			if ((event.mouseButton.x < rectangle.getPosition().x || event.mouseButton.x >(rectangle.getPosition().x + rectangle.getSize().x))
				|| (event.mouseButton.y < rectangle.getPosition().y || event.mouseButton.y >(rectangle.getPosition().y + rectangle.getSize().y)))
			{
				text.setString(s);
				return s;
			}
		case Event::KeyPressed:
			if (event.key.code == Keyboard::Escape)
			{
				window.setVisible(true);
				exit(0);
			}



			while (window.pollEvent(event)) {

				if (event.type == Event::Closed)
					window.close();

				if (event.type == Event::TextEntered) {
					if (event.key.code == Keyboard::Escape)
						window.close();
					if (event.key.code == 8 && s.size() != 0)		//touche backspace
					{
						s.pop_back();  //Enleve le dernier char dans la string
					}
					else if (event.key.code == 13) //touche enter
					{
						if (s.size() != 0)
						{
							text.setString(s);
							return s;
						}
					}
					else if (
						((event.key.code >= 97 && event.key.code <= 122)
							|| event.key.code == 32 || (event.key.code == Keyboard::RShift || event.key.code == Keyboard::LShift)
							|| (event.key.code >= 48 && event.key.code <= 57))
						&& (s.size() <= 10))	// caractere ASCII (A - Z, 0 - 9 )
					{
						s.push_back((char)event.text.unicode); // ajoute un char a la string

					}
					text.setString(s); 	// choix de la chaîne de caractères à afficher

					window.draw(rectangle);	//Affiche un rectangle vide
					window.draw(text);
					window.display();
				}
			}

		}
	}

}

string saisieTexteCacher(RenderWindow & window, RectangleShape rectangle, Text &text, Font &font, string &MDP)
{
	Event event;
	string s;
	string mdpCacher;
	text.setPosition(rectangle.getPosition().x + 15, rectangle.getPosition().y + (rectangle.getSize().y / 2.5));
	text.setFont(font);
	MDP = "\0";
	while (window.waitEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
			exit(0);
		case Event::MouseButtonPressed:
			Mouse::getPosition(window);
			if ((event.mouseButton.x < rectangle.getPosition().x || event.mouseButton.x >(rectangle.getPosition().x + rectangle.getSize().x))
				&& (event.mouseButton.y < rectangle.getPosition().y || event.mouseButton.y >(rectangle.getPosition().y + rectangle.getSize().y)))
			{
				text.setString(s);
				MDP = s;
				return mdpCacher;
			}
		case Event::KeyPressed:
			if (event.key.code == Keyboard::Escape)
			{
				window.setVisible(true);
				exit(0);
			}
			while (window.pollEvent(event)) {

				if (event.type == Event::Closed)
					window.close();

				if (event.type == Event::TextEntered) {
					if (event.key.code == Keyboard::Escape)
						window.close();
					if (event.key.code == 8 && s.size() != 0)		//touche backspace
					{
						s.pop_back();  //Enleve le dernier char dans la string
						mdpCacher.pop_back();
					}
					else if (event.key.code == 13) //touche enter
					{
						if (s.size() != 0)
						{
							text.setString(s);
							MDP = s;
							return mdpCacher;
						}
					}
					else if (
						((event.key.code >= 97 && event.key.code <= 122)
							|| event.key.code == 32 || (event.key.code == Keyboard::RShift || event.key.code == Keyboard::LShift)
							|| (event.key.code >= 48 && event.key.code <= 57))
						&& (s.size() <= 10))	// caractere ASCII (A - Z, 0 - 9 )
					{
						s.push_back((char)event.text.unicode); // ajoute un char a la string
						mdpCacher += '*';
					}
					text.setString(mdpCacher); 	// choix de la chaîne de caractères à afficher

					window.draw(rectangle);	//Affiche un rectangle vide
					window.draw(text);
					window.display();
				}
			}

		}
	}
}

int listeEnregistrement(RenderWindow & window2, joueur & joueur)
{
	ifstream listeJoueur;		//Fichier contenant les informations des joueurs
	char sauvegardeNomJoueur[15],
		MDP[15];
	int lvlEnregistrer[3],
		scoreEnregistrer[3],
		rang = 0;			//permet de lister le joueurs pour le trie
							//char continuerPartie;
	bool sauvegardeExiste = false,
		charIdentique = false;
	int longueur = 10;
	RectangleShape identiteEnregistrer(Vector2f(300, 125));
	Font font;
	if (!font.loadFromFile("styles/font_arial.ttf")); // choix de la police à utiliser
	Text text;


	if (!ouvrirFichier(listeJoueur, "score.txt"))
		exit(0);

	//lis le fichier de score au complet pour trouver les informations du joueurs
	while (listeJoueur >> sauvegardeNomJoueur && joueur.nouveauJoueur == true) {

		listeJoueur >> MDP >> scoreEnregistrer[0] >> scoreEnregistrer[1] >> scoreEnregistrer[2];

		strcat_s(sauvegardeNomJoueur, "\0");
		int longeurNom = strlen(sauvegardeNomJoueur);
		rang++;

		//si le nom entrer  existe dans le fichier score
		if (joueur.nomJoueur == sauvegardeNomJoueur)
		{ //Si le mdp ne correspond pas
			if (MDP != joueur.mdpJoueur)
			{
				return 3;
			}
			joueur.nouveauJoueur = false;
			break;
		}

	}

	if (joueur.nouveauJoueur == false) {
		joueur.score[0] = scoreEnregistrer[0];
		joueur.score[1] = scoreEnregistrer[1];
		joueur.score[2] = scoreEnregistrer[2];
		joueur.rang = rang;
		return 1;

	}
	else
		return 2;


}

int enregistrement(joueur joueur)
{
	char sauvegardeNomJoueur[30],
		MDP[30];
	int lvlEnregistrer[3],
		scoreEnregistrer[3],
		rang = 0;

	int reponse = 1;
	if (reponse == 1) {						//Enregistre tout les score dans un tableau

		ifstream vieuxscore;
		ofstream nouveauscore;

		vieuxscore.open("score.txt", ifstream::in | ifstream::out | ifstream::app);
		nouveauscore.open("score.txt", ofstream::in | ofstream::out | ofstream::app);

		char listeNomJoueur[150][20] = { '\0' };				//maximum de 150 identite enregistrer
		int levelMax[20];
		int scoreMax[20];

		int cptJoueur = 0;

		if (joueur.nouveauJoueur == false) {

			while (vieuxscore >> sauvegardeNomJoueur)
			{
				vieuxscore >> MDP[cptJoueur];
				for (size_t j = 0; j < 3; j++)
				{
					vieuxscore >> scoreEnregistrer[j];
				}
				cptJoueur++;
			}

			vieuxscore.close();
			vieuxscore.open("score.txt", ifstream::out | ifstream::trunc);
			vieuxscore.close();

			/*	trierInsertion(listeNomJoueur, scoreMax, cptJoueur);
			*/
			for (int i = 0; i < joueur.rang - 1; i++)		// Écrit tout les autre identite avant
			{
				nouveauscore << left << setw(20) << sauvegardeNomJoueur[i] << setw(20) << MDP[i] << setw(20) << scoreEnregistrer[i] << setw(20) << scoreEnregistrer[i] << setw(20) << scoreEnregistrer[i] << endl;
			}
			nouveauscore << left << setw(20) << joueur.nomJoueur << setw(20) << joueur.mdpJoueur << joueur.score[1];

			for (int i = 0 + joueur.rang; i < cptJoueur; i++) // Écrit tout les autre identite apres
			{
				nouveauscore << left << setw(20) << sauvegardeNomJoueur[i] << setw(20) << MDP[i] << setw(20) << scoreEnregistrer[i] << setw(20) << scoreEnregistrer[i] << setw(20) << scoreEnregistrer[i] << endl;
			}
		}

		else										//inscrit le nouveau identite a la fin de la liste de score
		{


			nouveauscore << left << setw(20) << joueur.nomJoueur << setw(20) << joueur.mdpJoueur << setw(20) << joueur.score[0] << setw(20) << joueur.score[1] << setw(20) << joueur.score[2] << endl;
			joueur.nouveauJoueur = false;
		}
		nouveauscore.close();
	}
	return 1;
}

void trierInsertion(char nomJoueur[][20], int scoreMax[], int taille)
{
	char nomJoueurTemp[20];
	int scoreJoueur[20];
	int i, j;
	for (i = 1; i < taille; ++i) {
		int elem = scoreMax[i];
		strcpy_s(nomJoueurTemp, 20, nomJoueur[i]);
		for (j = i; j > 0 && scoreMax[j - 1] < elem; j--) {
			scoreMax[j] = scoreMax[j - 1];
			strcpy_s(nomJoueur[j], 20, nomJoueur[j - 1]);
		}
		scoreMax[j] = elem;
		strcpy_s(nomJoueur[j], 20, nomJoueurTemp);
	}
}

int fenetreAvertissement(Font font, string message, int LARGEURWINDOW, int HAUTEURWINDOW)
{
	//Cree une fenetre avec le tier de la dimention de L'ecran
	float WindowsLargeur = VideoMode::getDesktopMode().width / 3;
	float WindowsHauteur = VideoMode::getDesktopMode().height / 3;
	RenderWindow windowsMessage(VideoMode(WindowsLargeur, WindowsHauteur), "Message");

	Texture texture;
	if (!texture.loadFromFile("background2.png"));		//Si la texture n'est pas trouvable, une erreur est afficher dans la console
	Sprite background(texture);
	windowsMessage.draw(background);
	boutonClick bouton[2];
	Text text;
	text.setFont(font);
	text.setFillColor(Color::Green);
	text.setCharacterSize(30);
	text.setString(message);
	text.setPosition((WindowsLargeur / 5) - (bouton[0].rectangle.getSize().x / 2), WindowsHauteur / 3 - (bouton[0].rectangle.getSize().y / 2));

	for (size_t i = 0; i < 2; i++)
	{
		bouton[i].rectangle.setFillColor(Color::White);
		bouton[i].rectangle.setOutlineThickness(5);
		bouton[i].rectangle.setOutlineColor(Color::Red);
		bouton[i].text.setCharacterSize(30);
		bouton[i].text.setStyle(Text::Bold);
		bouton[i].text.setFillColor(Color::Green);
		bouton[i].fondImage.setSmooth(true);
		bouton[i].text.setFont(font);
		bouton[i].rectangle.setSize(Vector2f(200, 75));
		bouton[i].rectangle.setTexture(&texture);
	}

	while (windowsMessage.isOpen())
	{
		windowsMessage.draw(text);

		bouton[0].text.setString("Annuler" + '\0');
		bouton[1].text.setString("Continuer" + '\0');

		bouton[0].rectangle.setPosition((WindowsLargeur / 10), WindowsHauteur - (WindowsHauteur / 4));
		bouton[1].rectangle.setPosition((WindowsLargeur / 1.75), WindowsHauteur - (WindowsHauteur / 4));

		int btnAnnulerX = bouton[0].rectangle.getPosition().x + ((bouton[0].rectangle.getSize().x / 4));
		int btnAnnulerY = bouton[0].rectangle.getPosition().y + (bouton[0].rectangle.getSize().y / 4);
		int btnContinuerX = bouton[1].rectangle.getPosition().x + ((bouton[1].rectangle.getSize().x / 6));
		int btnContinuerY = bouton[1].rectangle.getPosition().y + (bouton[1].rectangle.getSize().y / 4);


		bouton[0].text.setPosition(btnAnnulerX, btnAnnulerY);
		bouton[1].text.setPosition(btnContinuerX, btnContinuerY);

		for (size_t i = 0; i < 2; i++)
		{
			windowsMessage.draw(bouton[i].rectangle);
			windowsMessage.draw(bouton[i].text);
		}

		Event event;
		windowsMessage.display();

		while (windowsMessage.waitEvent(event))
		{
			windowsMessage.draw(background);
			for (size_t i = 0; i < 2; i++)
			{
				windowsMessage.draw(bouton[i].rectangle);
				windowsMessage.draw(bouton[i].text);
			}


			windowsMessage.draw(text);


			windowsMessage.display();

			//Bouton annuler
			if ((event.mouseMove.x > bouton[0].rectangle.getPosition().x && event.mouseMove.x < (bouton[0].rectangle.getPosition().x + bouton[0].rectangle.getSize().x))
				&& (event.mouseMove.y > bouton[0].rectangle.getPosition().y && event.mouseMove.y < (bouton[0].rectangle.getPosition().y + bouton[0].rectangle.getSize().y)))
			{

				bouton[0].rectangle.setOutlineColor(Color::Blue);


			}
			else
			{
				bouton[0].rectangle.setOutlineColor(Color::Red);

			}
			//Bouton Continuer
			if ((event.mouseMove.x > bouton[1].rectangle.getPosition().x && event.mouseMove.x < (bouton[1].rectangle.getPosition().x + bouton[1].rectangle.getSize().x))
				&& (event.mouseMove.y > bouton[1].rectangle.getPosition().y && event.mouseMove.y < (bouton[1].rectangle.getPosition().y + bouton[1].rectangle.getSize().y)))
			{

				bouton[1].rectangle.setOutlineColor(Color::Blue);


			}
			else
			{
				bouton[1].rectangle.setOutlineColor(Color::Red);

			}
			switch (event.type)
			{
			case Event::Closed:
				return 2;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{// S l'utilisateur appuis que la touche echap, ferme le menu d'option et retourne au menu ou au jeu
					windowsMessage.setVisible(true);
					return 2;
				}

			case Event::MouseButtonPressed:
				Mouse::getPosition(windowsMessage);
				//Bouton Annuler
				if ((event.mouseButton.x > bouton[0].rectangle.getPosition().x && event.mouseButton.x < (bouton[0].rectangle.getPosition().x + bouton[0].rectangle.getSize().x))
					&& (event.mouseButton.y > bouton[0].rectangle.getPosition().y && event.mouseButton.y < (bouton[0].rectangle.getPosition().y + bouton[0].rectangle.getSize().y)))
				{
					return 2;
				}
				//Bouton Continuer
				else if ((event.mouseButton.x > bouton[1].rectangle.getPosition().x && event.mouseButton.x < (bouton[1].rectangle.getPosition().x + bouton[1].rectangle.getSize().x))
					&& (event.mouseButton.y > bouton[1].rectangle.getPosition().y && event.mouseButton.y < (bouton[1].rectangle.getPosition().y + bouton[1].rectangle.getSize().y)))
				{
					return 1;

				}
			}
		}
	}
}

void fenetreErreur(Font font, string message, int LARGEURWINDOW, int HAUTEURWINDOW)
{

	float WindowsLargeur = VideoMode::getDesktopMode().width / 3;
	float WindowsHauteur = VideoMode::getDesktopMode().height / 3;
	RenderWindow windowsMessage(VideoMode(WindowsLargeur, WindowsHauteur), "Message");

	Texture texture;
	if (!texture.loadFromFile("background2.png"));		//Si la texture n'est pas trouvable, une erreur est afficher dans la console
	Sprite background(texture);
	windowsMessage.draw(background);
	boutonClick bouton;
	Text text;
	text.setFont(font);
	text.setFillColor(Color::Green);
	text.setCharacterSize(30);
	text.setString(message);
	text.setPosition((WindowsLargeur / 5) - (bouton.rectangle.getSize().x / 2), WindowsHauteur / 3 - (bouton.rectangle.getSize().y / 2));


	bouton.rectangle.setFillColor(Color::White);
	bouton.rectangle.setOutlineThickness(5);  //défini la grosseur de la bordure
	bouton.rectangle.setOutlineColor(Color::Red); //défini la couleur de la bordure
	bouton.text.setCharacterSize(30);
	bouton.text.setStyle(Text::Bold);
	bouton.text.setFillColor(Color::Green);
	bouton.fondImage.setSmooth(true);
	bouton.text.setFont(font);
	bouton.rectangle.setSize(Vector2f(200, 75));
	bouton.rectangle.setTexture(&texture);


	while (windowsMessage.isOpen())
	{
		windowsMessage.draw(text);



		bouton.rectangle.setPosition((WindowsLargeur / 3), WindowsHauteur - (WindowsHauteur / 2));


		int btnOkX = bouton.rectangle.getPosition().x + ((bouton.rectangle.getSize().x / 3));
		int btnOkY = bouton.rectangle.getPosition().y + (bouton.rectangle.getSize().y / 3);



		bouton.text.setPosition(btnOkX, btnOkY);
		bouton.text.setString("Ok" + '\0');





		windowsMessage.draw(bouton.rectangle);
		windowsMessage.draw(bouton.text);


		Event event;
		windowsMessage.display();

		while (windowsMessage.waitEvent(event))
		{
			windowsMessage.draw(background);

			windowsMessage.draw(bouton.rectangle);
			windowsMessage.draw(bouton.text);



			windowsMessage.draw(text);


			windowsMessage.display();

			if ((event.mouseMove.x > bouton.rectangle.getPosition().x && event.mouseMove.x < (bouton.rectangle.getPosition().x + bouton.rectangle.getSize().x))
				&& (event.mouseMove.y > bouton.rectangle.getPosition().y && event.mouseMove.y < (bouton.rectangle.getPosition().y + bouton.rectangle.getSize().y)))
			{

				bouton.rectangle.setOutlineColor(Color::Blue);


			}
			else
			{
				bouton.rectangle.setOutlineColor(Color::Red);

			}

			switch (event.type)
			{
			case Event::Closed:
				return;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{// S l'utilisateur appuis que la touche echap, ferme le menu d'option et retourne au menu ou au jeu
					windowsMessage.setVisible(true);
					return;
				}

			case Event::MouseButtonPressed:
				Mouse::getPosition(windowsMessage);
				if ((event.mouseButton.x > bouton.rectangle.getPosition().x && event.mouseButton.x < (bouton.rectangle.getPosition().x + bouton.rectangle.getSize().x))
					&& (event.mouseButton.y > bouton.rectangle.getPosition().y && event.mouseButton.y < (bouton.rectangle.getPosition().y + bouton.rectangle.getSize().y)))
				{
					return;
				}


			}
		}
	}
}

void selectionPersonnage(RenderWindow & window, Font font, joueur &joueur, int HAUTEURWINDOW, int LARGEURWINDOW)
{
	personnage personnage[NBRPERSONNAGE];
	Texture textureFond;
	if (!textureFond.loadFromFile("background3.png"));
	Texture textureHero;
	Texture textureSkill[3];
	Sprite background(textureFond);
	string cheminImg[NBRPERSONNAGE];
	Texture texture[NBRPERSONNAGE];
	Image image[3];
	Text infoPersonnage;
	const int NBRTEXT = 2;
	const int NBRBOUTON = 7;
	boutonClick bouton[NBRBOUTON];
	RectangleShape heroAfficher;
	Sprite sprite;
	Clock timer;
	initPersonnage(personnage, cheminImg, texture);

	for (size_t i = 0; i < NBRBOUTON; i++)
	{
		bouton[i].rectangle.setFillColor(Color::White);
		bouton[i].rectangle.setOutlineThickness(5);  //défini la grosseur de la bordure
		bouton[i].rectangle.setOutlineColor(Color::Red); //défini la couleur de la bordure
		bouton[i].text.setCharacterSize(30);
		bouton[i].text.setStyle(Text::Bold);
		bouton[i].text.setFillColor(Color::Green);
		bouton[i].fondImage.setSmooth(true);
		bouton[i].text.setFont(font);
	}

	infoPersonnage.setCharacterSize(30);
	infoPersonnage.setFillColor(Color::Green);
	infoPersonnage.setFont(font);
	infoPersonnage.setString('\0');


	bouton[0].rectangle.setSize(Vector2f(LARGEURWINDOW / 4, HAUTEURWINDOW - (HAUTEURWINDOW / 2.75)));
	bouton[1].rectangle.setSize(Vector2f(LARGEURWINDOW - (LARGEURWINDOW / 4) - bouton[0].rectangle.getOutlineThickness(), HAUTEURWINDOW - (HAUTEURWINDOW / 2.75)));

	bouton[2].rectangle.setSize(Vector2f(HAUTEURWINDOW / 10, HAUTEURWINDOW / 10));
	bouton[3].rectangle.setSize(Vector2f(HAUTEURWINDOW / 10, HAUTEURWINDOW / 10));
	bouton[4].rectangle.setSize(Vector2f(HAUTEURWINDOW / 10, HAUTEURWINDOW / 10));

	bouton[5].rectangle.setSize(Vector2f(350, 150));
	bouton[6].rectangle.setSize(Vector2f(350, 150));

	bouton[0].rectangle.setPosition(0 + bouton[0].rectangle.getOutlineThickness(), 0 + bouton[0].rectangle.getOutlineThickness());
	bouton[1].rectangle.setPosition(bouton[0].rectangle.getSize().x, 0 + bouton[0].rectangle.getOutlineThickness());

	bouton[2].rectangle.setPosition(LARGEURWINDOW - (LARGEURWINDOW / 4), HAUTEURWINDOW / 8);
	bouton[3].rectangle.setPosition(LARGEURWINDOW - (LARGEURWINDOW / 4), bouton[2].rectangle.getPosition().y + (bouton[2].rectangle.getSize().y + (bouton[2].rectangle.getSize().y / 2)));
	bouton[4].rectangle.setPosition(LARGEURWINDOW - (LARGEURWINDOW / 4), bouton[3].rectangle.getPosition().y + (bouton[3].rectangle.getSize().y + (bouton[2].rectangle.getSize().y / 2)));


	bouton[5].rectangle.setPosition((LARGEURWINDOW / 4), HAUTEURWINDOW - (HAUTEURWINDOW / 4));
	bouton[6].rectangle.setPosition((LARGEURWINDOW / 1.75), HAUTEURWINDOW - (HAUTEURWINDOW / 4));


	bouton[0].text.setString("Personnage");
	bouton[1].text.setString(joueur.nomJoueur);

	bouton[2].text.setString("Skill 1");
	bouton[3].text.setString("Skill 2");
	bouton[4].text.setString("Skill 3");

	bouton[5].text.setString("Connecter");
	bouton[6].text.setString("Quitter");


	string textPerso = bouton[0].text.getString();
	int textPersoLongeur = textPerso.size();

	string textnom = bouton[1].text.getString();
	int textNomLongeur = textnom.size();

	int descriptionPersoX = bouton[1].rectangle.getPosition().x + (bouton[0].rectangle.getOutlineThickness() * 2);
	int descriptionPersoY = bouton[1].rectangle.getSize().y - (bouton[1].rectangle.getSize().y / 6);

	int textPersoX = bouton[0].rectangle.getPosition().x + ((bouton[0].rectangle.getSize().x / 3));
	int textPersoY = bouton[0].rectangle.getPosition().y + (bouton[0].rectangle.getOutlineThickness() * 3);

	int textnomX = bouton[1].rectangle.getPosition().x + ((bouton[1].rectangle.getSize().x / 3));
	int textnomY = bouton[1].rectangle.getPosition().y + (bouton[1].rectangle.getOutlineThickness() * 3);

	int textSkill1X = bouton[2].rectangle.getPosition().x + bouton[2].rectangle.getSize().x + (bouton[2].rectangle.getSize().x / 2);
	int textSkill1Y = bouton[2].rectangle.getPosition().y + (bouton[2].rectangle.getSize().y / 2) - (bouton[2].text.getCharacterSize() / 2);

	int textSkill2X = bouton[3].rectangle.getPosition().x + bouton[3].rectangle.getSize().x + (bouton[3].rectangle.getSize().x / 2);
	int textSkill2Y = bouton[3].rectangle.getPosition().y + (bouton[3].rectangle.getSize().y / 2) - (bouton[3].text.getCharacterSize() / 2);

	int textSkill3X = bouton[4].rectangle.getPosition().x + bouton[4].rectangle.getSize().x + (bouton[4].rectangle.getSize().x / 2);
	int textSkill3Y = bouton[4].rectangle.getPosition().y + (bouton[4].rectangle.getSize().y / 2) - (bouton[4].text.getCharacterSize() / 2);

	int btnConectX = bouton[5].rectangle.getPosition().x + ((bouton[2].rectangle.getSize().x / 3));
	int btnConectY = bouton[5].rectangle.getPosition().y + (bouton[2].rectangle.getSize().y / 2.5);

	int btnQuitX = bouton[6].rectangle.getPosition().x + ((bouton[3].rectangle.getSize().x / 3));
	int btnQuitY = bouton[6].rectangle.getPosition().y + (bouton[3].rectangle.getSize().y / 2.5);

	infoPersonnage.setPosition(descriptionPersoX, descriptionPersoY);
	bouton[0].text.setPosition(textPersoX, textPersoY);
	bouton[1].text.setPosition(textnomX, textnomY);

	bouton[2].text.setPosition(textSkill1X, textSkill1Y);
	bouton[3].text.setPosition(textSkill2X, textSkill2Y);
	bouton[4].text.setPosition(textSkill3X, textSkill3Y);

	bouton[5].text.setPosition(btnConectX, btnConectY);
	bouton[6].text.setPosition(btnQuitX, btnQuitY);

	bouton[0].rectangle.setFillColor(Color::Transparent);
	bouton[1].rectangle.setFillColor(Color::Transparent);
	bouton[2].rectangle.setFillColor(Color::Magenta);
	bouton[3].rectangle.setFillColor(Color::Black);
	bouton[4].rectangle.setFillColor(Color::Green);
	bouton[5].rectangle.setFillColor(Color::Yellow);
	bouton[6].rectangle.setFillColor(Color::Black);

	while (window.isOpen())
	{

		Event event;
		while (window.waitEvent(event))
		{

			window.draw(background);
			for (size_t i = 0; i < NBRBOUTON; i++)
			{
				window.draw(bouton[i].rectangle);
				window.draw(bouton[i].text);
			}
			window.draw(infoPersonnage);
			afficherListePerso(window, bouton[0].rectangle, font, personnage, cheminImg);
			/*	window.draw(heroAfficher);*/
			window.draw(sprite);
			window.display();

			for (size_t i = 0; i < NBRPERSONNAGE; i++)
			{

				//Si le curseur passe sur le bouton
				if ((event.mouseMove.x > personnage[i].img.getPosition().x && event.mouseMove.x < (personnage[i].img.getPosition().x + personnage[i].img.getSize().x))
					&& (event.mouseMove.y > personnage[i].img.getPosition().y && event.mouseMove.y < (personnage[i].img.getPosition().y + personnage[i].img.getSize().y)))
				{

					personnage[i].img.setOutlineColor(Color::Blue);
					window.draw(personnage[i].img);

				}

				else
				{
					personnage[i].img.setOutlineColor(Color::Red);
					window.draw(personnage[i].img);

				}
			}

			switch (event.type)
			{
			case Event::Closed:
				exit(0);
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{// S l'utilisateur appuis que la touche echap, ferme le menu d'option et retourne au menu ou au jeu
					window.setVisible(true);
					exit(0);
				}
				else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
				{
					bougerPerso(window, sprite, heroAfficher, 0, timer);
				}
				else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D)
				{
					bougerPerso(window, sprite, heroAfficher, 1, timer);
				}
				else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
				{
					bougerPerso(window, sprite, heroAfficher, 2, timer);
				}
				else if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A)
				{
					bougerPerso(window, sprite, heroAfficher, 3, timer);
				}

			case Event::MouseButtonPressed:
				Mouse::getPosition(window);
				for (size_t i = 0; i < NBRPERSONNAGE; i++)
				{
					//Si on appuie sur un des boutons
					if ((event.mouseButton.x > personnage[i].img.getPosition().x && event.mouseButton.x < (personnage[i].img.getPosition().x + personnage[i].img.getSize().x))
						&& (event.mouseButton.y > personnage[i].img.getPosition().y && event.mouseButton.y < (personnage[i].img.getPosition().y + personnage[i].img.getSize().y)))
					{

						infoPersonnage.setString(personnage[i].description);

						heroAfficher.setSize(Vector2f(bouton[1].rectangle.getSize().x / 3, bouton[1].rectangle.getSize().x / 3));
						heroAfficher.setPosition(bouton[1].rectangle.getPosition().x + (bouton[1].rectangle.getSize().x / 3) - (heroAfficher.getSize().x / 3),
							bouton[1].rectangle.getPosition().y + (bouton[1].rectangle.getSize().y / 3) - (heroAfficher.getSize().y / 3));
						heroAfficher.setOutlineColor(Color::Red);
						heroAfficher.setOutlineThickness(5);


						/*	if (!textureHero.loadFromFile(personnage[i].fondImage));
						*/

						sprite.setTexture(personnage[i].fondImage);
						sprite.setTextureRect(sf::IntRect(48, 64, 24, 32));
						/*sprite.setFillColor(Color::Green);*/
						sprite.setPosition(heroAfficher.getPosition().x, heroAfficher.getPosition().y);
						sprite.setScale(8, 8);



						for (size_t j = 0; j < 3; j++)
						{
							textureSkill[j].loadFromFile(personnage[i].skill.cheminImg[j]);

						}
						for (size_t s = 2; s < 5; s++)
						{
							bouton[s].rectangle.setTexture(&textureSkill[s - 2]);
							bouton[s].text.setString(personnage[i].skill.description[s - 2].getString());
						}
					}
				}
			}
		}
	}
}

void afficherListePerso(RenderWindow &window, RectangleShape &rectangle, Font font, personnage personnage[NBRPERSONNAGE], string cheminImg[NBRPERSONNAGE])
{


	int hauteurIcon = 1;

	for (size_t i = 0; i < NBRPERSONNAGE; i++)
	{
		/*personnage[i].text.setFillColor(Color::Black);
		personnage[i].text.setFont(font);*/
		personnage[i].img.setSize(Vector2f(rectangle.getSize().x / 4, rectangle.getSize().x / 4));
		personnage[i].img.setOutlineThickness(5);

	}
	personnage[0].img.setPosition((rectangle.getPosition().x + (rectangle.getOutlineThickness() * 3)), (rectangle.getPosition().y + (rectangle.getOutlineThickness() * 12)));

	for (size_t i = 1; i < NBRPERSONNAGE; i++)
	{

		personnage[i].img.setPosition(((personnage[i - 1].img.getSize().x + personnage[i - 1].img.getPosition().x) + (rectangle.getOutlineThickness() * 3)), (personnage[i - 1].img.getPosition().y));

		if (personnage[i].img.getPosition().x + (personnage[i].img.getSize().x) >= (rectangle.getSize().x + rectangle.getPosition().x))
		{

			personnage[i].img.setPosition(Vector2f(personnage[0].img.getPosition().x,
				(personnage[i - 1].img.getPosition().y + personnage[i - 1].img.getSize().y + (personnage[i - 1].img.getOutlineThickness() * 3))));
			hauteurIcon++;
		}

	}
	for (size_t i = 0; i < NBRPERSONNAGE; i++)
	{

		window.draw(personnage[i].img);
	}



}

void initPersonnage(personnage personnage[NBRPERSONNAGE], string cheminImg[NBRPERSONNAGE], Texture texture[NBRPERSONNAGE])
{
	personnage[0].nom = "Albert";
	personnage[0].classe = "Chevalier";
	personnage[0].description = ("Albert est un chevalier qui peux \nFaire des choses de chevalier");
	personnage[0].skill.description[0].setString("Peux subir plus \nde domage");
	personnage[0].skill.description[1].setString("Peux Foncer et \ninfliger du domage");
	personnage[0].skill.description[2].setString("A un look BadAss");
	personnage[0].skill.cheminImg[0] = "icon/armorIcon.png";
	personnage[0].skill.cheminImg[1] = "icon/spearIcon.png";
	personnage[0].skill.cheminImg[2] = "icon/chevalierBadAssIcon.png";
	cheminImg[0] = "personnage/barbare.png";

	personnage[1].nom = "Oulio";
	personnage[1].classe = "Mage";
	personnage[1].description = ("Oulio est un mage qui peux lancer des sorts! (Meh)");
	personnage[1].skill.description[0].setString("FIREBALL!!");
	personnage[1].skill.description[1].setString("Peux faire des \ntruc de glace :)");
	personnage[1].skill.description[2].setString("Peux Flotter!!");
	personnage[1].skill.cheminImg[0] = "icon/fireballIcon.png";
	personnage[1].skill.cheminImg[1] = "icon/iceIcon.png";
	personnage[1].skill.cheminImg[2] = "icon/levitationIcon.png";
	cheminImg[1] = "personnage/mage3.png";

	personnage[2].nom = "Gertrude";
	personnage[2].classe = "Aventurier";
	personnage[2].description = ("Gertrurde est UN aventurier très spécial....");
	cheminImg[2] = "personnage/aventurier.png";


	personnage[3].nom = "Belzeor";
	personnage[3].classe = "Elfe";
	personnage[3].description = ("Belzeor est un elfe....des plaines du desert");
	cheminImg[3] = "personnage/elf.png";


	personnage[4].nom = "Gimlie";
	personnage[4].classe = "Guerrier";
	personnage[4].description = ("Gimlie est un guerrier qui se bat!!");
	cheminImg[4] = "personnage/warrior.png";

	personnage[5].nom = "TireLoin";
	personnage[5].classe = "Archer";
	personnage[5].description = ("TireLoin peux tirer des fleches \ntres loin");
	cheminImg[5] = "personnage/archer.png";

	personnage[6].nom = "Bertrand";
	personnage[6].classe = "Assassin";
	personnage[6].description = ("Un assassin qui tue!");
	cheminImg[6] = "personnage/assassin.png";

	personnage[7].nom = "BedingBedang";
	personnage[7].classe = "Barbare";
	personnage[7].description = ("BedingBedang peux mal....");
	cheminImg[7] = "personnage/barbare.png";

	personnage[8].nom = "Listen";
	personnage[8].classe = "Barde";
	personnage[8].description = ("Joue de la musique ");
	cheminImg[8] = "personnage/barde.png";

	personnage[9].nom = "CanTouchYou";
	personnage[9].classe = "Escrimeur";
	personnage[9].description = ("CanTouchYou est une brute en \n combat singulier");
	cheminImg[9] = "personnage/warrior.png";

	personnage[10].nom = "Flow";
	personnage[10].classe = "Faucheur";
	personnage[10].description = ("Peux Faucher les âme impures...");
	cheminImg[10] = "personnage/faux.png";

	personnage[11].nom = "Sir";
	personnage[11].classe = "Gentleman";
	personnage[11].description = ("Un simple Gentleman");
	cheminImg[11] = "personnage/gentleman.png";

	personnage[12].nom = "Roboto";
	personnage[12].classe = "Robot";
	personnage[12].description = ("Roboto....Tuer....Humain...");
	cheminImg[12] = "personnage/robot.png";

	personnage[13].nom = "King King";
	personnage[13].classe = "King";
	personnage[13].description = ("King King est un roi....:D");
	cheminImg[13] = "personnage/roi.png";

	personnage[14].nom = "AOOUUUU";
	personnage[14].classe = "Wolf";
	personnage[14].description = ("AOOUUUU peux manger n'imrpote quoi");
	cheminImg[14] = "personnage/wolf.png";




	for (size_t i = 0; i < NBRPERSONNAGE; i++)
	{
		if (!texture[i].loadFromFile(cheminImg[i]));
		personnage[i].img.setTexture(&texture[i]);
		personnage[i].img.setTextureRect(sf::IntRect(48, 64, 24, 32));
		personnage[i].fondImage = texture[i];
	}


}

void bougerPerso(RenderWindow &window, Sprite &sprite, RectangleShape &rectangle, int direction, Clock &clock)
{

	//Direction 0=haut 3=bas 4=gauche 1=droite


	int positionY = 33 * direction;
	sf::IntRect rectSourceSprite(sprite.getTextureRect());
	sprite.setTextureRect(rectSourceSprite);

	rectSourceSprite.top = direction * 32;
	if (rectSourceSprite.left >= 48)
		rectSourceSprite.left = 0;
	else
		rectSourceSprite.left += 24;



	sprite.setTextureRect(rectSourceSprite);
	sprite.setPosition(rectangle.getPosition());
	sprite.setScale(8, 8);

	window.draw(sprite);
	window.display();
	window.display(); // double display sinon l'ancien sprite ne s'éfface pas
}

void resize(RenderWindow &window, int &LARGEURWINDOW, int &HAUTEURWINDOW)
{
	HAUTEURWINDOW = window.getSize().y;
	LARGEURWINDOW = window.getSize().x;

	window.setSize(Vector2u(HAUTEURWINDOW, LARGEURWINDOW));
}

void menuPrincipal(RenderWindow &window, joueur &joueur)
{


}