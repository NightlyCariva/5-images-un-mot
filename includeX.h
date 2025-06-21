#include <SDL2/SDL.h> // main biblio -lSLD2main -lSDL2
#include <SDL2/SDL_image.h> //pour plus de liberter sur les images -lSDL2_image
#include <SDL2/SDL_ttf.h> //Pour gestion de text -lSDL2_ttf
#include <stdio.h> //necessaire pour SDL_GetError
#include <stdlib.h>
#include <string.h> // pour les str
#include <time.h> //Random et calcule du temps d'execution
#include <math.h> //fonction mathematique
#include <SDL2/SDL_mixer.h> //necessaire pour la music -lSDL2_mixer
#define HEIGHT 800
#define WIDTH 1200
#define BACKGROUND_PATH "images/background.jpg"
#define MENU_PATH "images/menu_states/menuComplet.png"
#define MENU_PATH_OP1 "images/menu_states/menu1H.png"
#define MENU_PATH_OP2 "images/menu_states/menu2H.png"
#define MENU_PATH_OP3 "images/menu_states/menu3H.png"
#define MENU_POLICE "polices/Roboto.ttf"
#define MAIN_MUSIC_PATH "sounds/mainsound.mp3"
#define SELECTION_SOUND "sounds/menuselection.mp3"
#define pathCLose "images/icones/close.png"
#define pathCLoseH "images/icones/closeH.png"
#define pathSound  "images/icones/sound.png"
#define pathSoundH "images/icones/soundH.png"
#define pathFr "images/icones/flags/france.png"
#define pathEsp "images/icones/flags/espagnole.png"
#define pathUk "images/icones/flags/uk.png"
#define pathApropos "images/apropos/Apropos.png"
#define pathOk "images/icones/ok.png"
#define pathOkH "images/icones/okH.png"
#define pathBackmenu "images/icones/backmenu.png"
#define pathImageboard "images/ingame/bigboard.png"
#define pathTextArea "images/ingame/textarea.png"
#define pathScoreBoard "images/ingame/scoreboard.png"
#define pathRetry "images/icones/retry.png"
#define pathRetryH "images/icones/retryH.png" 
#define pathNext "images/icones/next.png"
#define pathPrevious "images/icones/previous.png"
#define PATHIMAGES "images/save/"


#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES // evité les inclusions infinies (appel des includes en boucle)
#endif
/*==================SDL ERREURS======================*/
int GestionError(void* pointeursdl, const char* messageErreur);
/*==================TTF ERREURS======================*/
int GestionErrorTTF(void* pointeursdl, const char* messageErreur);
/*==================Mixer ERREURS======================*/
int GestionErrorMix(void* pointeursdl, const char* messageErreur);
/*====================Diminution du volume=================*/
void volumeDown(int* volume);
/*===================Augmentation du volume================*/
void volumeUp(int* volume);
/*==================TTF Ecriture text======================*/
SDL_Texture* insertText(const char* PoliceName, int PoliceSize, SDL_Color Color, const char* message, SDL_Renderer* renderer);
/*======================Si la souris est sur une zone type rect==================*/
int isMouseOnRect(Sint32 x, Sint32 y, SDL_Rect* rect);
/*========================Boutton switcher=================*/
SDL_bool switcherBool(SDL_bool bool);
/*==================INSERTION D'IMAGE======================*/
SDL_Texture* loadImage(char* path, SDL_Renderer *renderer);
/*Delet last caracter*/
void supp(char* chaine);

/*=======================================STRUCTURES======================================================*/

/*================ENREGISTREMENT VALEUR=====================*/
typedef struct Enregistrement
{
    char* MotFr;
    char* Mot2Langue;
    char* AdresseImage;
}Enregistrement;

/*==================C'EST QUOI UN ARBRE ?===================*/
typedef struct Arbre
{
    Enregistrement* valeur;
    struct Arbre *gauche;
    struct Arbre *droit;
}Arbre;

/*==================ACCES BRANCHES==================*/
Arbre* _G(Arbre* A);
Arbre* _D(Arbre* A);
/*==================ARBRE VIDE ?====================*/
int ArbreVide(Arbre* A);
/*==================CALCULE MAX=====================*/
int  Imax(int a, int b);
/*==================Nouveau Arbre===================*/
Arbre* newArbre(Enregistrement* valeur);
/*===============Nouveau Enregistrement=============*/
Enregistrement* newValeur(char* MotFr, char* Mot2Langue, char* AdresseImage);
/*==================HAUTEUR AVL====================*/
int hauteur(Arbre* A);
/*==================ROTATIONS SIMPLE & DOUBLES===================*/
Arbre* _rg(Arbre* A);
Arbre* _rd(Arbre* A);
Arbre* _rgd(Arbre* A);
Arbre* _rdg(Arbre* A);
/*==================DESEQUILIBRE===================*/
int desequilibre(Arbre* A);
/*=================REEQUILIBRE=====================*/
Arbre* reequilibre(Arbre* A);
/*==================INSERTION AVL==================*/
Arbre* AjoutAVL(Arbre* A, Enregistrement* v);
/*================PARCOURS AVL=====================*/
void ParcoursAVL(Arbre* A);
void FreeAVL(Arbre* A);
/*===============RECHERCHE AVL====================*/
int RechercheAVL(Arbre* A, char* MotFr, char* MotLangueChoisie, char* AdresseImage);
/*============FILES FUNCTIONS SPLIT===============*/
char** split(char* chaine,const char* delim,int vide);
/*===================CHARGER UN FICHIER EN TABLEAU======================*/
char** FileToTable(const char* pathFile);
/*=====================Nombre Aleatoire entre A et B Inclus=====================*/
double random_function(void);
int randomAB(int borne_minimale , int borne_maximale);
/*===============================Free Table 2D==============================*/
void FreeTab2D(char** tab, int taille);
/*===============================Score===================================*/
int Score(int tentative, int ScoreBase);
char* itoa(int val, int base);