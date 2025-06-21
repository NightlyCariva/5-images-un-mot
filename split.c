#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Retour tableau des chaines recupérer. Terminé par NULL.
// chaine : chaine à splitter
// delim : delimiteur qui sert à la decoupe
// vide : 0 : on n'accepte pas les chaines vides
//        1 : on accepte les chaines vides
char** split(char* chaine,const char* delim,int vide){
    
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

/*=======================Récuperation des mots=========================*/
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




int main(){
    char** fileTab;
    char** FrTab;
    /*
    printf("Chaine initiale : %s \n",str);
    
    tab=split(str,"|",0);
    //affichage du resultat
    for(i=0;tab[i]!=NULL;i++) {
       printf("%d : %s\n",i,tab[i]);
       //au passge je désalloue les chaines
       free(tab[i]);
    }
    free(tab);
    
    printf("\n");
    
    tab=split(str,"|",1);
    //affichage du resultat
    for(i=0;tab[i]!=NULL;i++) {
       printf("%d : %s\n",i,tab[i]);
       //au passge je désalloue les chaines
       free(tab[i]);
    }
    free(tab);
    */

   fileTab = FileToTable("words.txt"); //
   //printf("ici\n");
   for (int i = 0; fileTab[i] != NULL; i++)
   {
        
        //printf("[%d] : %s \n",i,FrTab[i]);
        free(FrTab[i]);
   }
   free(FrTab);
}
