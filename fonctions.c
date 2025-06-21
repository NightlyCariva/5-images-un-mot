#include "includeX.h"
/*==================SDL ERREURS======================*/
int GestionError(void* pointeursdl, const char* messageErreur)
{
    if (pointeursdl == NULL)
    {
        printf("%s : %s\n",messageErreur, SDL_GetError());
        return EXIT_FAILURE;
    }    
}
/*==================TTF ERREURS======================*/
int GestionErrorTTF(void* pointeursdl, const char* messageErreur)
{
    if (pointeursdl == NULL)
    {
        printf("%s : %s\n",messageErreur, TTF_GetError());
        return EXIT_FAILURE;
    }    
}
/*==================Mixer ERREURS======================*/
int GestionErrorMix(void* pointeursdl, const char* messageErreur)
{
    if (pointeursdl == NULL)
    {
        printf("%s : %s\n",messageErreur, Mix_GetError());
        return EXIT_FAILURE;
    } 
}
/*====================Diminution du volume=================*/
void volumeDown(int* volume)
{   
    if(*volume > 0) --(*volume);
}
/*===================Augmentation du volume================*/
void volumeUp(int* volume)
{
    if(*volume < MIX_MAX_VOLUME)++(*volume);
}
/*==================TTF Ecriture text======================*/
SDL_Texture* insertText(const char* PoliceName, int PoliceSize, SDL_Color Color, const char* message, SDL_Renderer* renderer)
{
    TTF_Font* font = NULL;
    font = TTF_OpenFont(PoliceName,PoliceSize);
    if(font == NULL)
    {
        printf("Erreur au chargement du font : %s\n",TTF_GetError());
        return NULL;
    }
    SDL_Surface* text = TTF_RenderText_Blended(font,message,Color);
    if(text==NULL)
    {
        printf("Erreur au chargement de surface pour du text : %s\n",TTF_GetError());
        return NULL;
    }
    SDL_Texture* textureFont = SDL_CreateTextureFromSurface(renderer,text);
    SDL_FreeSurface(text);
    TTF_CloseFont(font);
    return textureFont;
}
/*======================Si la souris est sur une zone type rect==================*/
int isMouseOnRect(Sint32 x, Sint32 y, SDL_Rect* rect)
{
    SDL_Point point = {x,y};
    //printf("x = %d , y= %d\n",point.x,point.y);
    if (SDL_PointInRect(&point, rect) == SDL_TRUE) return 1;
    else return 0;
}
/*========================Boutton switcher=================*/
SDL_bool switcherBool(SDL_bool bool)
{
    if (bool == SDL_FALSE) return SDL_TRUE;
    else return  SDL_FALSE;
}
/*==================INSERTION D'IMAGE======================*/
SDL_Texture* loadImage(char* path, SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = IMG_Load(path);
    if(GestionError(tmp,"Erreur lors du IMGload") == EXIT_FAILURE) return NULL ;
    texture = SDL_CreateTextureFromSurface(renderer,tmp);
    SDL_FreeSurface(tmp);
    if(GestionError(texture,"Erreur lors de convertion Surface -> Texture") == EXIT_FAILURE) return NULL ;
    return texture;
}

/*Delet last caracter*/
/*
void supp(char* chaine)
{
    char temp[strlen(chaine)];
    for(int i=0; i < strlen(chaine); i++)
    {
        temp[i]=chaine[i];
    }
    strcpy(chaine,temp);
}*/

/*copy chaine*/
void copyChaine(char* dest, char* src)
{
    for (int i = 0; src[i] != '\0' ; i++)
    {
        dest[i]=src[i];
    }
}
char** splitV2(char* Pstring, const char Delim)
{   
    int compteur = 0;
    int delim_Numb = 0;
    for (int i = 0; Pstring[i] != '\0'; i++)
    {
        if (Pstring[i] == Delim)
        {
            compteur++; // comptée le nombre de délimiteur dans la chaine 
        }
    }
    char** tab =(char**)malloc(sizeof(char*)*(compteur+1)); //Une chaine au début
    char temp[200];
    for (int i= 0; Pstring[i] != '\0'; i++)
    {
        if (Pstring[i] != Delim)
        {
            temp[i]=Pstring[i];
        }
        else
        {
            tab[delim_Numb] = (char*)malloc(sizeof(char)*(strlen(temp)+1));
            strcpy(tab[delim_Numb],temp);
            strcpy(temp,"");
            delim_Numb++;
        }
    }
    return tab;
}

/*=======================================================================STRUCTURES===========================================================*/


/*===========================================================
================FONCTIONS PRELIMINAIRE=======================
============================================================*/

/*==================ACCES BRANCHES===================*/

//Sous arbre gauche
Arbre* _G(Arbre* A)
{
    return A->gauche;
}

//Sous arbre droit
Arbre* _D(Arbre* A)
{
    return A->droit;
}

/*==================ARBRE VIDE ?===================*/
int ArbreVide(Arbre* A)
{
    if (A == NULL) return 1;
    else return 0;
}

/*==================CALCULE MAX===================*/
int  Imax(int a, int b)
{
    return (a > b ) ? a : b ;
}

/*==================Nouveau Arbre===================*/
Arbre* newArbre(Enregistrement* valeur)
{
    //Allocation
    Arbre* A = (Arbre*)malloc(sizeof(A));
    //Remplissage
    A->valeur = valeur;                                                               
    A->gauche = NULL; 
    A->droit = NULL;

    return A;
}

/*===============Nouveau Enregistrement==============*/
Enregistrement* newValeur(char* MotFr, char* Mot2Langue, char* AdresseImage)
{
    Enregistrement* valeur = (Enregistrement*)malloc(sizeof(valeur));
    valeur->MotFr = (char*)malloc(sizeof(char)*strlen(MotFr));
    valeur->Mot2Langue = (char*)malloc(sizeof(char)*strlen(Mot2Langue));
    valeur->AdresseImage = (char*)malloc(sizeof(char)*strlen(AdresseImage));
    strcpy(valeur->MotFr,MotFr);
    strcpy(valeur->Mot2Langue,Mot2Langue);
    strcpy(valeur->AdresseImage,AdresseImage);
    return valeur;
}

/*===========================================================
==============FONCTIONS SPECIFIQUE TYPE AVL==================
============================================================*/

/*==================HAUTEUR AVL===================*/
int hauteur(Arbre* A)
{
    if (ArbreVide(A)) return 0;
    else return 1 + Imax(hauteur(_G(A)),hauteur(_D(A)));
}

/*==================ROTATIONS SIMPLE & DOUBLES===================*/

//Rotation simple gauche :
Arbre* _rg(Arbre* A)
{
    Arbre* aux = _D(A);
    A->droit = _G(aux);
    aux->gauche = A;
    A = aux;
    return A;
}

//Rotation simple droite :
Arbre* _rd(Arbre* A)
{
    Arbre* aux = _G(A);
    A->gauche = _D(aux);
    aux->droit = A;
    A = aux;
    return A;
}

//Rotation double RGD :
Arbre* _rgd(Arbre* A)
{
    A->gauche = _rg(A->gauche);
    return _rd(A);
}

//Rotation double RDG :
Arbre* _rdg(Arbre* A)
{
    A->droit = _rd(A->droit);
    return _rg(A);
}

/*==================DESEQUILIBRE===================*/
int desequilibre(Arbre* A)
{
    if (ArbreVide(A)) return 0;
    else return hauteur(_G(A)) - hauteur(_D(A));
}

/*=================REEQUILIBRE=====================*/
Arbre* reequilibre(Arbre* A)
{
    if (desequilibre(A) <= 1 && desequilibre(A) >= -1) return A;
    if (desequilibre(A) == 2 && desequilibre(_G(A)) == 1) return _rd(A);
    if (desequilibre(A) ==-2 && desequilibre(_D(A)) ==-1) return _rg(A);
    if (desequilibre(A) == 2 && desequilibre(_G(A)) ==-1) return _rgd(A);
    if (desequilibre(A) ==-2 && desequilibre(_D(A)) == 1) return _rdg(A);
}

/*===========================================================
====================INSERTION AVL============================
============================================================*/

Arbre* AjoutAVL(Arbre* A, Enregistrement* v)
{
    if (ArbreVide(A)) 
        return (newArbre(v));
    if (strcmp(v->MotFr,A->valeur->MotFr)<0)
        A->gauche = AjoutAVL(_G(A),v);
    else if (strcmp(v->MotFr,A->valeur->MotFr)>0) 
        A->droit = AjoutAVL(_D(A),v);
    else //si jamais l'enregistrement existe déja dans l'arbre
        return A;
    
    return reequilibre(A);
}

/*===========================================================
====================PARCOURS AVL=============================
============================================================*/
//Parcours en profendeur à main gauche type préfixe
void ParcoursAVL(Arbre* A)
{
    if (ArbreVide(A) == 0)
    {
        printf("%s --- ",A->valeur->MotFr);
        ParcoursAVL(_G(A));
        ParcoursAVL(_D(A));
    }
}


/*===========================================================
====================RECHERCHE DU MOT=========================
============================================================*/
int RechercheAVL(Arbre* A, char* MotFr, char* MotLangueChoisie, char* AdresseImage)
{
    if (ArbreVide(A)) return 0;
    else
    {
        if (strcmp(A->valeur->MotFr,MotFr) == 0)
        {
            strcpy(MotLangueChoisie,A->valeur->Mot2Langue);
            strcpy(AdresseImage,A->valeur->AdresseImage);
            return 1;
        }
        else
        {
            if (strcmp(A->valeur->MotFr,MotFr) > 0)
                return RechercheAVL(_G(A),MotFr,MotLangueChoisie,AdresseImage);
            else
                return RechercheAVL(_D(A),MotFr,MotLangueChoisie,AdresseImage);
        }
    }
}
/*===========================================================
====================FILES FUNCTIONS==========================
============================================================*/


/*====================FONCTION SPLIT=======================*/
// Retour tableau des chaines recupérer. Terminé par NULL.
// chaine : chaine à splitter
// delim : delimiteur qui sert à la decoupe
// vide : 0 : on n'accepte pas les chaines vides
//        1 : on accepte les chaines vides
char** split(char* chaine,const char* delim,int vide)
{
    
    char** tab=NULL;               //tableau de chaine, tableau resultat
    char *ptr;                     //pointeur sur une partie de
    int sizeStr;                   //taille de la chaine à recupérer
    int sizeTab=0;                 //taille du tableau de chaine
    char* largestring;             //chaine à traiter
    
    int sizeDelim=strlen(delim);   //taille du delimiteur

    largestring = chaine;          //comme ca on ne modifie pas le pointeur d'origines
    

    while( (ptr=strstr(largestring, delim))!=NULL ){
           sizeStr=ptr-largestring;
     
           //si la chaine trouvé n'est pas vide ou si on accepte les chaine vide                   
           if(vide==1 || sizeStr!=0){
               //on alloue une case en plus au tableau de chaines
               sizeTab++;
               tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
                              
               //on alloue la chaine du tableau
               tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
               strncpy(tab[sizeTab-1],largestring,sizeStr);
               tab[sizeTab-1][sizeStr]='\0';
           }
           
           //on decale le pointeur largestring  pour continuer la boucle apres le premier elément traiter
           ptr=ptr+sizeDelim;
           largestring=ptr;
    }
    
    //si la chaine n'est pas vide, on recupere le dernier "morceau"
    if(strlen(largestring)!=0){
           sizeStr=strlen(largestring);
           sizeTab++;
           tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
           tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
           strncpy(tab[sizeTab-1],largestring,sizeStr);
           tab[sizeTab-1][sizeStr]='\0';
    }
    else if(vide==1){ //si on fini sur un delimiteur et si on accepte les mots vides,on ajoute un mot vide
           sizeTab++;
           tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
           tab[sizeTab-1]=(char*) malloc( sizeof(char)*1 );
           tab[sizeTab-1][0]='\0';
           
    }
    
    //on ajoute une case à null pour finir le tableau
    sizeTab++;
    tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
    tab[sizeTab-1]=NULL;

    return tab;
}

/*===================CHARGER UN FICHIER EN TABLEAU======================*/
char** FileToTable(const char* pathFile)
{
    
    char** tab = NULL;          //Tableau de chaine resultaant
    FILE* f = fopen(pathFile,"r"); //Initialisation + ouverture fichier
    int index = 0;              //Indice en tableau
    int tailleBuffer = 0;       // taille exacte de la ligne
    char buffer[500];           
    //Test d'ouverture
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier %s en mode lecture\n",pathFile);
    }
    else
    {
        
        //char* buffer = (char*)malloc(300);   //Allo
        tab = (char**)malloc(sizeof(char*)); // on alloue au debut la taille d'une chaine de caractère
        while (fgets(buffer,500,f) != NULL) //fgets s'arret à la rencontre d'un /n ou du EOF
        {
            tailleBuffer = strlen(buffer);
            //on a passer la condition , donc y'a une nouvelle ligne
            tab[index] = (char*)malloc(sizeof(char)*(tailleBuffer+1)); //on loue une nouvelle chaine dans le tableau
            strcpy(tab[index],buffer);
            tab[index][tailleBuffer] = '\0';
            //printf("ici\n");
            index ++;
            tab = (char**)realloc(tab,sizeof(char*)*(index+1));
            tab[index] = NULL;
            //printf("%s",tab[index]);
        }                                   //Désallo
        //free(buffer);
    }
    fclose(f); //Fermeture
    return tab;
}

/*=====================Nombre Aleatoire entre A et B Inclus=====================*/
double random_function(void) 
{
    return (double) rand() / RAND_MAX;
}

int randomAB(int borne_minimale , int borne_maximale)
{   
    srand(time(NULL));
    return (int)(borne_minimale + (borne_maximale - borne_minimale) * random_function());

}
/*================================FREE AVL==================================*/
void FreeAVL(Arbre* A)
{
    if (A == NULL) return;
    FreeAVL(A->gauche);
    FreeAVL(A->droit);
    free(A);
    return;
}
/*===============================Free Table 2D==============================*/
/*
While the free() statement only appears once, it is called repeatedly (and correctly) by the loop.
The error is triggered because the free() statement is attempting to free the same address.
                        Grand merci à StackOverFlow xD
*/
void FreeTab2D(char** tab, int taille)
{
    if (tab != NULL) //l'erreur qui a failli me rendre fou... (je ne charge la structure qu'une seule fois dans la game loop donc si on décharge ce qu'on a pas besoin pour une fois plus la peine de le decharger plusieurs autres fois !!!!  free(): double free detected in tcatch2)
    {
        for(int i = 0; i < taille; i++)
        {
            free(tab[i]);
        }
        free(tab);
    }
}

/*========================SCORE====================*/
int Score(int tentative, int ScoreBasic)
{
    return floor(exp((float)(tentative*(-1))/10)*1000 + ScoreBasic);
}
/*============itoa non standard (merci la doc)============*/
char* itoa(int val, int base)
{
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

/*===========================================================
===============TEST IMPLEMENTATION DU MOTEUR=================
============================================================*/
/*
int main()
{
    //time_t start = time (NULL);
    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);
    Arbre* root = NULL;
    //Enregistrement* v1 = newValeur("a","A","a.png");
    //Enregistrement* v2 = newValeur("b","B","b.png");
    //Enregistrement* v3 = newValeur("c","C","c.png");
    char mot2[50];
    char adresse[50];
    Enregistrement** EnregistrementTab;
    char** fileTab;             //Tableau de tous les mots de notre fichier
    char** FrTab;               //Tableau mot francais
    int cpt_NbrMot = 0;         //compteur nombre de mot sert comme index pour FrTab
    fileTab = FileToTable("words.txt");
    FrTab = (char**)malloc(sizeof(char*)); // preparer les champs du tableau (un seul champ au départ)
    //printf("ici\n");
    for (int i = 0; fileTab[i] != NULL; i++)
    {
        char** splitTab = split(fileTab[i],",",0);    //chaque chaine de file tab sera séparer selon "," et on accepte pas le vide
        int taille_motFr = strlen(splitTab[0]);
        //printf("ici\n");
        //Partie creation arbre : 
        Enregistrement* v = newValeur(splitTab[0],splitTab[1],splitTab[3]); // création de l'enregistrement
        printf("motfr : %s, motdeuxiem : %s, adresse : %s \n",v->MotFr,v->Mot2Langue,v->AdresseImage);
        root = AjoutAVL(root,v); //insertion en arbre
        printf("ici\n");
        //Partie chargement du tableau FR
        FrTab[i] = (char*)malloc(sizeof(char)*(taille_motFr+1)); //Rappel une désalocation avant de quiter le jeu ! 
        strcpy(FrTab[i],splitTab[0]);
        //printf("ici\n");
        FrTab[i][taille_motFr] = '\0';
        cpt_NbrMot ++; //preparation prochain champs
        FrTab = (char**)realloc(FrTab,sizeof(char*)*(cpt_NbrMot+1));
        FrTab[cpt_NbrMot] = NULL; //tableau se terminant avec un champs null
        
        //Désallocation de mémoire : (Rappel ! faut un free(fileTab) à la fin de la grosse boucle)
        //de split toute entière : 
        for (int j = 0; splitTab[j] != NULL ; j++)
        {
            free(splitTab[j]);
        }
        free(splitTab);

        //de fileTab[i]
        free(fileTab[i]);
    }
    free(fileTab);

    printf("parcours en profondeurs ...\n");
    printf("\n");
    ParcoursAVL(root);
    printf("\n");
    printf("========================================================================================\n");
    printf("\n");
    printf("Recherche de \"a\" en cours ...\n");
    if (RechercheAVL(root,"a",mot2,adresse))
    {
        printf("recherche effectué avec succes ! le mot à la deuxième langue  est : %s , l'adresse est : %s \n",mot2,adresse);
    }
    else
    {
        printf("astaghfiro lah, lah itbtna 3la chhada \n");
    }
    printf("\n");
    printf("========================================================================================\n");
    printf("\n");
    printf("Recherche de \"Vent\" en cours ...\n");
    if (RechercheAVL(root,"Vent",mot2,adresse))
    {
        printf("recherche effectué avec succes ! le mot à la deuxième langue  est : %s , l'adresse est : %s \n",mot2,adresse);
    }
    else
    {
        printf("astaghfiro lah, lah itbtna 3la chhada \n");
    }
    printf("\n");
    printf("========================================================================================\n");
    printf("Parcours du tableau des mots francais en cours ...\n");
    for(int i = 0; FrTab[i] != NULL ; i++)
    {
        printf("%s - ",FrTab[i]);
    }
    printf("\n");
    printf("========================================================================================\n");
    clock_gettime(CLOCK_REALTIME, &stop);
    //printf ("Durée d'éxecution = %ds\n", (int) (time (NULL) - start));
    printf("Temps d'execution : %.9lfs\n",(double) ((stop.tv_nsec * 1e-9 + stop.tv_sec) - (start.tv_nsec * 1e-9 + start.tv_sec)));

    return 0;
}
*/