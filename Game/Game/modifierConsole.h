/* En-tête du programme
====================
Programme:			modifierConsole.h
Auteur:				Jean-Alain Sainton et Francis Forest
Date création:		06-11-2017
Description :		contient les getteur et setteur pour la taille et la position de la console
*/
#pragma once
#include <windows.h>

using namespace std;

/*Redimentionne la taille de la console
=====================================*/

//set la position a partir de la position en haut a gauche de l'écran en pixel
void SetConsolePos(int x, int y)
{	
	// Console handle
	HWND hConsole = GetConsoleWindow(); //

	//taille en pixel
	int tailleX = GetSystemMetrics(SM_CXSCREEN);
	int tailleY = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(hConsole, 0, x, y, tailleX - 10, tailleY - 10, SWP_NOSIZE | SWP_NOZORDER);
}
/*Redimentionne la taille de la console au maximum
=====================================*/
void SetTailleMaxEcran()
{
	HANDLE MaConsoleEcran = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = GetLargestConsoleWindowSize(MaConsoleEcran);	  //taille maximum possible
	COORD MaTailleEcran = { size.X - 1 ,100 };					  //nombre de ligne et collonne de la console
	SMALL_RECT MonRectangle = { 0,0,size.X - 1 ,size.Y - 1 };	  // taille du rectangle en pixel

	SetConsoleScreenBufferSize(MaConsoleEcran, MaTailleEcran);
	SetConsoleWindowInfo(MaConsoleEcran, TRUE, &MonRectangle);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

//Redimentionne la taille de la console
void tailleEcran(int x, int y)
{
	// Console handle
	HANDLE MaConsoleEcran = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = GetLargestConsoleWindowSize(MaConsoleEcran); //taille ax en pixel
	if (x > size.X)
		x = size.X;
	
	if (y > size.Y)
		y = size.Y;
	
	COORD MaTailleEcran = { size.X ,size.Y };	//nombre de ligne et collonne de la console

	SMALL_RECT MonRectangle = { 0,0, x,y };		// taille du rectangle en pixel

	SetConsoleScreenBufferSize(MaConsoleEcran, MaTailleEcran);
	SetConsoleWindowInfo(MaConsoleEcran, TRUE, &MonRectangle);


}

//permet d'aller à une position exact dans la console (X, Y)
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos;
	scrn.Y = ypos;

	SetConsoleCursorPosition(hOuput, scrn);
}

//retourne la largeur de la console en nombre de colonne
int getTailleConsoleX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HWND hConsole = GetConsoleWindow();
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return 0;
	
	else
		return 	csbi.dwSize.X;

}

//retourne la largeur de la console en Pixel
int getPixelConsoleX()
{
	int tailleX = GetSystemMetrics(SM_CXVIRTUALSCREEN); //taile en pixel
	return	tailleX;

}

//retourne la hauteur de la console en Pixel
int getPixelConsoleY()
{
	int tailleY = GetSystemMetrics(SM_XVIRTUALSCREEN);
	return tailleY;
}

//retourne la hauteur de la console nombre de ligne
int getTailleConsoleY()
{

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return 0;

	else
		return csbi.srWindow.Bottom;
	

}
//retourne la colonne du millieu
int getPosXMid()
{
	return getTailleConsoleX() / 2;
}

//modifie la taille de la police
void setFontSize(int tailleX, int tailleY)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = tailleX;                   // Width of each character in the font
	cfi.dwFontSize.Y = tailleY;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

//retourne la largeur de la police
int getFontSizeX()
{
	// Obtain the Console handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Obtain the Console handle

	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();

	// set the size of the CONSOLE_FONT_INFOEX
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);

	// get the current value
	GetCurrentConsoleFontEx(hConsole, 0, lpConsoleCurrentFontEx);

	// set size to be 8x18, the default size is 8x16
	return	lpConsoleCurrentFontEx->dwFontSize.X;

}

//retourne la hauteur de la police
int getFontSizeY()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Obtain the Console handle

	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();

	// set the size of the CONSOLE_FONT_INFOEX
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);

	// get the current value
	GetCurrentConsoleFontEx(hConsole, 0, lpConsoleCurrentFontEx);

	// set size to be 8x18, the default size is 8x16
	return	lpConsoleCurrentFontEx->dwFontSize.Y;
}