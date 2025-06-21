#include "includeX.h"
int main(int argc, char *argv[])
{
    /*===========================================================
    ===================DECLARATION===============================
    ============================================================*/
    int HEIGHT_FENETRE = 0;
    int WIDTH_FENETRE = 0;
    SDL_Window* fenetre = NULL;  //Déclaration de la fenêtre
    SDL_Renderer* render = NULL; //Rendu pour la fenetre
    SDL_Event events; //evenements
    int statut = EXIT_FAILURE; //par defaut a exit failure
    //Etats liée aux evenements
    SDL_bool run = SDL_TRUE;
    SDL_bool isCloseHold = SDL_FALSE; //boutton close
    SDL_bool isSoundClick = SDL_FALSE; //boutton son
    SDL_bool isOkHold = SDL_FALSE; //boutton ok
    SDL_bool dontPut = SDL_FALSE;
    SDL_bool ReturnPressed = SDL_FALSE;       //Entré appuyé GO verif le mot saisi !
    SDL_bool isWinning = SDL_FALSE;          //Si on a gagné
    SDL_bool Retry = SDL_FALSE;             //Rejouer avec meme config/Refaire le tirage au sort
    SDL_bool RetryHolded = SDL_FALSE;
    int wichOptionHolded = 0; //selection d'option dans le menu
    int isFlagSelected = 0; //selection de flag
    int wichOptionClicked = 0; 
    int isFlagClicked = 0; //Flag clické
    SDL_bool wantToInput = SDL_FALSE;   //False : on ne veut pas écrire , true on veut écrire du text
    int volume = MIX_MAX_VOLUME / 2;
    //Structures
    Arbre* root = NULL;
    char MotLangueChoisie[50];
    char AdresseImage[50];
    char RechercheMot[100];
    char** fileTab;             //Tableau de tous les mots de notre fichier
    char** FrTab;               //Tableau mot francais
    int cpt_NbrMot = 0;         //compteur nombre de mot sert comme index pour FrTab
    int indexFr = 0;                    //l'indice du mot tiré aleatoirement dans le tableau FrTab
    int count_mots = 0;                 // nombre de mots existant dans notre fichier
    SDL_bool StructLoaded = SDL_FALSE;
    char AdressAbsolutImage[1000];
    char InputText[1000] = "Saisir un mot";  //text saisi
    int tentative = 1;              // nombre de tentative (on commence le jeu au tenta : 1)
    int ScoreSolo = 1;              //On ne peut pas mettre 0 (problème au convertion int->char)
    //MULTI :
    int PlayerTurn = 0; //le tour de quel joueur (à saisir le mot) ? (0 aucun , 1: Joueur 1, 2: Joueur 2)
    SDL_bool SearchPhase = SDL_FALSE; //True : l'image est afficher et faut deduire le mot 
    char suggestionMots[1000]; //text a afficher les suggestions de mots
    int ScoreP1 = 1;
    int ScoreP2 = 1;
    Arbre* root2 = NULL;



    /*===========================================================
    ========INITIALISATION: SDL,TTF,MIXER,WINDOW,RENDER==========
    ============================================================*/

    if(SDL_Init(SDL_INIT_VIDEO) < 0)  // initialisation de la SDL
    {
       printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
       goto Quit;
    }
    if(TTF_Init() < 0)
    {
        printf("Erreur d'initialisation de TTF : %s",TTF_GetError());
        goto Quit;
    }
    if(Mix_OpenAudio(96000,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024)<0)
    {
        printf("Erreur d'initialisation de Mixer : %s", Mix_GetError());
        goto Quit;
    }

    fenetre = SDL_CreateWindow
    (
        "Une Image Un Mot", //titre
        SDL_WINDOWPOS_CENTERED, //coordonné x int
        SDL_WINDOWPOS_CENTERED, //coordonné y int 
        WIDTH, //largeur int 
        HEIGHT, //hauteur int 
        SDL_WINDOW_RESIZABLE
    );  // Création de la fenêtre    
    SDL_GetWindowSize(fenetre,&WIDTH_FENETRE,&HEIGHT_FENETRE);
    printf("old size w : %d h : %d\n",WIDTH_FENETRE,HEIGHT_FENETRE);
    if(GestionError(fenetre,"Erreur lors de la creation d'une fenetre") == statut) goto Quit;

    render = SDL_CreateRenderer
    (
        fenetre, //rendu sur quelle fenetre ?
        -1, //le -1 permet de switch de GPU -> CPU s'il n'existe pas
        SDL_RENDERER_ACCELERATED //Uti GPU
    ); 
    if(GestionError(fenetre,"Erreur lors de la creation d'un rendu") == statut) goto Quit;

    /*===========================================================
    ===================Rect & TexTures===========================
    ============================================================*/

    //declaration Rect :
    SDL_Color fontColorSucces = {0,255,0,255};
    SDL_Color fontColorError = {255,0,0,255};
    SDL_Color fontColorMessage = {0,255,255,255};
    SDL_Color fontColorWhite = {255,255,255,255};
    SDL_Rect position; //position text
    SDL_Rect scorePosition; //position du text score
    SDL_Rect scorePositionN; //Affichage du numero du score
    //Menu position
    SDL_Rect menuPos = {0,0,0,0};//dimention du menu source
    SDL_Rect menuPosAfter = {0,0,1000,1000}; //dimension du menu apres modif
    //Boutton close position
    SDL_Rect dimClose = {0,0,0,0};
    SDL_Rect dimCloseAfter = {0,0,30,30};
    //Boutton sound position
    SDL_Rect dimSound = {0,0,0,0};
    SDL_Rect dimSoundAfter = {0,0,30,30};
    //Fr flag position
    SDL_Rect dimFrFlag = {0,0,0,0};
    SDL_Rect dimFrFlagAfter = {0,0,60,30};
    //ESP flag position
    SDL_Rect dimEspFlag = {0,0,0,0};
    SDL_Rect dimEspFlagAfter = {0,0,60,30};
    //UK flag position
    SDL_Rect dimUkFlag = {0,0,0,0};
    SDL_Rect dimUkFlagAfter = {0,0,60,30};
    int decalage_between_flags = 10; //10px
    //Apropos
    SDL_Rect dimApropos = {0,0,0,0};
    SDL_Rect dimAproposAfter = {0,0,600,700};
    //Boutton Ok
    SDL_Rect dimOk = {0,0,0,0};
    SDL_Rect dimOkAfter = {0,0,50,50};
    //Boutton Backmenu
    SDL_Rect dimBackmenu = {0,0,0,0};
    SDL_Rect dimBackmenuAfter = {0,0,50,50};
    //Boutton Retry
    SDL_Rect dimRetry = {0,0,0,0};
    SDL_Rect dimRetryAfter = {0,0,80,40};
    //ImageBoard
    SDL_Rect dimImageBoard = {0,0,0,0};
    SDL_Rect dimImageBoardAfter = {0,0,1000,750};
    //ImageGame
    SDL_Rect dimImageInside = {0,0,0,0};
    SDL_Rect dimImageInsideAfter = {0,0,388,295};
    //TexteArea
    SDL_Rect dimTextArea = {0,0,0,0};
    SDL_Rect dimTextAreaAfter = {0,0,500,200};
    //score One player
    SDL_Rect dimScoreOnePlayer = {0,0,0,0};
    SDL_Rect dimScoreOnePlayerAfter = {0,0,300,100};
    //score 2 player : 1
    SDL_Rect dimScoreTwoPlayer1 = {0,0,0,0};
    SDL_Rect dimScoreTwoPlayer1After = {0,0,300,100};
    //score 2 player : 2
    SDL_Rect dimScoreTwoPlayer2 = {0,0,0,0};
    SDL_Rect dimScoreTwoPlayer2After = {0,0,300,100};
    //Text Player 1:
    SDL_Rect player1pos;
    //Text Player 2:
    SDL_Rect player2pos;
    //Score P1 text:
    SDL_Rect score1pos;
    //Score P2 text:
    SDL_Rect score2pos;
    //Suggestion pos :
    SDL_Rect posSugg;
    //Background
    SDL_Texture* backgroundTexture = loadImage(BACKGROUND_PATH,render);
    if(GestionError(backgroundTexture,"Erreur au chargement de texture du background")==statut) goto Quit;

    

    //Menu
    SDL_Texture* menuTexture = loadImage(MENU_PATH,render);
    if(GestionError(menuTexture,"Erreur au chargement de l'image du menu")==statut) goto Quit;
    SDL_QueryTexture(menuTexture,NULL,NULL,&menuPos.w,&menuPos.h);
    menuPosAfter.x = WIDTH_FENETRE /2 - menuPosAfter.w /2;
    menuPosAfter.y = HEIGHT_FENETRE /2 - menuPosAfter.h /2 +90;
    
    //Menu Option : 1
    SDL_Texture* menuTexture1 = loadImage(MENU_PATH_OP1,render);
    if(GestionError(menuTexture1,"Erreur au chargement de l'image du menu option 1")==statut) goto Quit;
    SDL_QueryTexture(menuTexture1,NULL,NULL,&menuPos.w,&menuPos.h);
    menuPosAfter.x = WIDTH_FENETRE /2 - menuPosAfter.w /2;
    menuPosAfter.y = HEIGHT_FENETRE /2 - menuPosAfter.h /2 +90;
    //Menu Option : 2
    SDL_Texture* menuTexture2 = loadImage(MENU_PATH_OP2,render);
    if(GestionError(menuTexture2,"Erreur au chargement de l'image du menu option 2")==statut) goto Quit;
    SDL_QueryTexture(menuTexture2,NULL,NULL,&menuPos.w,&menuPos.h);
    menuPosAfter.x = WIDTH_FENETRE /2 - menuPosAfter.w /2;
    menuPosAfter.y = HEIGHT_FENETRE /2 - menuPosAfter.h /2 +90;
    //Menu Option : 3
    SDL_Texture* menuTexture3 = loadImage(MENU_PATH_OP3,render);
    if(GestionError(menuTexture3,"Erreur au chargement de l'image du menu option 3")==statut) goto Quit;
    SDL_QueryTexture(menuTexture3,NULL,NULL,&menuPos.w,&menuPos.h);
    menuPosAfter.x = WIDTH_FENETRE /2 - menuPosAfter.w /2;
    menuPosAfter.y = HEIGHT_FENETRE /2 - menuPosAfter.h /2 +90;
    //Option menu
    SDL_Rect dimOp1 = {menuPosAfter.x + 400,menuPosAfter.y + 550, 245,36}; //Option 1
    SDL_Rect dimOp2 = {dimOp1.x,dimOp1.y + 80,302,36}; //Option 2
    SDL_Rect dimOp3 = {dimOp1.x,dimOp2.y + 80,200,36}; //Option 3

    //Icones :
    //Retry not hold
    SDL_Texture* retryTexture = loadImage(pathRetry,render);
    if(GestionError(retryTexture,"Erreur au chargement de l'image retry") == statut) goto Quit;
    SDL_QueryTexture(retryTexture,NULL,NULL,&dimRetry.w,&dimRetry.h);
    dimRetryAfter.x = WIDTH_FENETRE - dimRetryAfter.w - 100; //en haut a droite de l'écran
    dimRetryAfter.y = 10;
    //Retry  hold
    SDL_Texture* retryHTexture = loadImage(pathRetryH,render);
    if(GestionError(retryHTexture,"Erreur au chargement de l'image retryH") == statut) goto Quit;
    SDL_QueryTexture(retryHTexture,NULL,NULL,&dimRetry.w,&dimRetry.h);
    dimRetryAfter.x = WIDTH_FENETRE - dimRetryAfter.w - 100; //en haut a droite de l'écran
    dimRetryAfter.y = 10;

    //close not hold
    SDL_Texture* closeTexture = loadImage(pathCLose,render);
    if(GestionError(closeTexture,"Erreur au chargement de l'image close") == statut) goto Quit;
    SDL_QueryTexture(closeTexture,NULL,NULL,&dimClose.w,&dimClose.h);
    dimCloseAfter.x = WIDTH_FENETRE - dimCloseAfter.w - 10; //en haut a droite de l'écran
    dimCloseAfter.y = 10;


    //close on hold
    SDL_Texture* closeHTexture = loadImage(pathCLoseH,render);
    if(GestionError(closeHTexture,"Erreur au chargement de l'image closeH") == statut) goto Quit;
    SDL_QueryTexture(closeHTexture,NULL,NULL,&dimClose.w,&dimClose.h);
    dimCloseAfter.x = WIDTH_FENETRE - dimCloseAfter.w - 10; //en haut a droite de l'écran
    dimCloseAfter.y = 10;

    //Ok not hold
    SDL_Texture* okTexture = loadImage(pathOk,render);
    if(GestionError(okTexture,"Erreur au chargement de l'icone Ok")==statut) goto Quit;
    SDL_QueryTexture(okTexture,NULL,NULL,&dimOk.w,&dimOk.h);
    dimOkAfter.x = WIDTH_FENETRE - dimOkAfter.w - 10; //en haut a droite de l'écran
    dimOkAfter.y = 10;

    //Ok hold
    SDL_Texture* okHTexture = loadImage(pathOkH,render);
    if(GestionError(okHTexture,"Erreur au chargement de l'icone OkH")==statut) goto Quit;
    SDL_QueryTexture(okHTexture,NULL,NULL,&dimOk.w,&dimOk.h);
    dimOkAfter.x = WIDTH_FENETRE - dimOkAfter.w - 10; //en haut a droite de l'écran
    dimOkAfter.y = 10;

    //back menu
    SDL_Texture* backmenuTexture = loadImage(pathBackmenu,render);
    if(GestionError(backmenuTexture,"Erreur au chargement de l'image BackToMenu") == statut) goto Quit;
    SDL_QueryTexture(backmenuTexture,NULL,NULL,&dimBackmenu.w,&dimBackmenu.h);
    dimBackmenuAfter.x = WIDTH_FENETRE - dimBackmenuAfter.w - 10; //en haut a droite de l'écran
    dimBackmenuAfter.y = 10;

    //sound not hold
    SDL_Texture* soundTexture = loadImage(pathSound,render);
    if(GestionError(soundTexture,"Erreur au chargement de l'image sound") == statut) goto Quit;
    SDL_QueryTexture(soundTexture,NULL,NULL,&dimSound.w,&dimSound.h);
    dimSoundAfter.x = dimCloseAfter.x - dimSound.w - decalage_between_flags; //en haut a droite de l'écran
    dimSoundAfter.y = 10;

    //sound on hold
    SDL_Texture* soundHTexture = loadImage(pathSoundH,render);
    if(GestionError(soundHTexture,"Erreur au chargement de l'image soundH") == statut) goto Quit;
    SDL_QueryTexture(soundHTexture,NULL,NULL,&dimSound.w,&dimSound.h);
    dimSoundAfter.x = dimCloseAfter.x - dimSound.w - decalage_between_flags; //en haut a droite de l'écran
    dimSoundAfter.y = 10;

    //flages:
    //FR
    SDL_Texture* FrFlagTexture = loadImage(pathFr,render);
    if(GestionError(FrFlagTexture,"Erreur au chargement de l'image flag Fr") == statut) goto Quit;
    SDL_QueryTexture(FrFlagTexture,NULL,NULL,&dimFrFlag.w,&dimFrFlag.h);
    dimFrFlagAfter.x = 10; //en haut a gauche de l'écran
    dimFrFlagAfter.y = 10;

    //ESP
    SDL_Texture* EspFlagTexture = loadImage(pathEsp,render);
    if(GestionError(EspFlagTexture,"Erreur au chargement de l'image flag Esp") == statut) goto Quit;
    SDL_QueryTexture(EspFlagTexture,NULL,NULL,&dimEspFlag.w,&dimEspFlag.h);
    dimEspFlagAfter.x = dimFrFlagAfter.x + dimFrFlagAfter.w + decalage_between_flags; //a droite de flag Fr 
    dimEspFlagAfter.y = 10;

    //UK
    SDL_Texture* UkFlagTexture = loadImage(pathUk,render);
    if(GestionError(UkFlagTexture,"Erreur au chargement de l'image flag Uk") == statut) goto Quit;
    SDL_QueryTexture(UkFlagTexture,NULL,NULL,&dimUkFlag.w,&dimUkFlag.h);
    dimUkFlagAfter.x = dimEspFlagAfter.x + dimEspFlagAfter.w + decalage_between_flags; //a droite de flag esp
    dimUkFlagAfter.y = 10;

    //Apropos
    SDL_Texture* aproposTexture = loadImage(pathApropos,render);
    if(GestionError(aproposTexture,"Erreur au chargement de l'image centrale apropos")==statut) goto Quit;
    SDL_QueryTexture(aproposTexture,NULL,NULL,&dimApropos.w,&dimApropos.h);
    dimAproposAfter.x = WIDTH_FENETRE /2 - dimAproposAfter.w /2;
    dimAproposAfter.y = HEIGHT_FENETRE /2 - dimAproposAfter.h /2;

    //ImageBoard
    SDL_Texture* imageboardTexture = loadImage(pathImageboard,render);
    if(GestionError(imageboardTexture,"Erreur au chargement du conteneur de l'image BigBoard")==statut) goto Quit;
    SDL_QueryTexture(imageboardTexture,NULL,NULL,&dimImageBoard.w,&dimImageBoard.h);
    dimImageBoardAfter.x = WIDTH_FENETRE /2 - dimImageBoardAfter.w /2;
    dimImageBoardAfter.y = HEIGHT_FENETRE /2 - dimImageBoardAfter.h /2;

    //ImageInsideBoard
    SDL_Texture* imageInside = NULL;
    
    //TextArea
    SDL_Texture* textareaTexture = loadImage(pathTextArea,render);
    if(GestionError(textareaTexture,"Erreur au chargement de l'image text area")==statut) goto Quit;
    SDL_QueryTexture(textareaTexture,NULL,NULL,&dimTextArea.w,&dimTextArea.h);
    dimTextAreaAfter.x = WIDTH_FENETRE /2 - dimTextAreaAfter.w /2;
    dimTextAreaAfter.y = HEIGHT_FENETRE - dimTextAreaAfter.h/2 - 50;
    //TextInput
    SDL_Texture* fontMenu = insertText(MENU_POLICE, 24, fontColorSucces, InputText, render);
    if(GestionError(fontMenu,"Erreur au chargement du font de saisie")==statut) goto Quit;
    SDL_QueryTexture(fontMenu,NULL,NULL,&position.w,&position.h);
    position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
    position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;


    //ScoreBoard 1 player
    SDL_Texture* scoreoneplayerTexture = loadImage(pathScoreBoard,render);
    if(GestionError(scoreoneplayerTexture,"Erreur au chargement de l'image du score SOLO")==statut) goto Quit;
    SDL_QueryTexture(scoreoneplayerTexture,NULL,NULL,&dimScoreOnePlayer.w,&dimScoreOnePlayer.h);
    dimScoreOnePlayerAfter.x = WIDTH_FENETRE /2 - dimScoreOnePlayer.w /2 + 300;
    dimScoreOnePlayerAfter.y = 0;
    //Score Text
    SDL_Texture* fontScore = insertText(MENU_POLICE, 24,fontColorWhite,"Votre Score Est :", render);
    if(GestionError(fontScore,"Erreur au chargement du font de score")==statut) goto Quit;
    SDL_QueryTexture(fontScore,NULL,NULL,&scorePosition.w,&scorePosition.h);
    scorePosition.x = dimScoreOnePlayerAfter.x + (dimScoreOnePlayerAfter.w/2 - scorePosition.w/2) ;
    scorePosition.y = 20;
    
    //Score SOLO
    SDL_Texture* fontScoreN = NULL;

    //ScoreBoard 2 player : 1
    SDL_Texture* scoretwoplayer1Texture = loadImage(pathScoreBoard,render);
    if(GestionError(scoretwoplayer1Texture,"Erreur au chargement de l'image du score MULTI 1")==statut) goto Quit;
    SDL_QueryTexture(scoretwoplayer1Texture,NULL,NULL,&dimScoreTwoPlayer1.w,&dimScoreTwoPlayer1.h);
    dimScoreTwoPlayer1After.x = 200;
    dimScoreTwoPlayer1After.y = 0;

    //ScoreBoard 2 player : 2
    SDL_Texture* scoretwoplayer2Texture = loadImage(pathScoreBoard,render);
    if(GestionError(scoretwoplayer2Texture,"Erreur au chargement de l'image du score MULTI 2")==statut) goto Quit;
    SDL_QueryTexture(scoretwoplayer2Texture,NULL,NULL,&dimScoreTwoPlayer2.w,&dimScoreTwoPlayer2.h);
    dimScoreTwoPlayer2After.x = WIDTH_FENETRE - 200 - dimScoreTwoPlayer1After.w;
    dimScoreTwoPlayer2After.y = 0;
    //Suggestion 
    SDL_Texture* fontsugg = NULL;
    //Nombre de score : 
    SDL_Texture* fontScore1 = NULL;
    SDL_Texture* fontScore2 = NULL;
    //Nom Player 1
    SDL_Texture* fontPlayer1 = insertText(MENU_POLICE, 24,fontColorWhite,"[Joueur 1]:", render);
    if(GestionError(fontPlayer1,"Erreur au chargement du font Player 1")==statut) goto Quit;
    SDL_QueryTexture(fontPlayer1,NULL,NULL,&player1pos.w,&player1pos.h);
    player1pos.x = dimScoreTwoPlayer1After.x + (dimScoreTwoPlayer1After.w/2 - player1pos.w/2) ;
    player1pos.y = 20;
    //Noms Player 2
    SDL_Texture* fontPlayer2 = insertText(MENU_POLICE, 24,fontColorWhite,"[Joueur 2]:", render);
    if(GestionError(fontPlayer2,"Erreur au chargement du font Player 2")==statut) goto Quit;
    SDL_QueryTexture(fontPlayer2,NULL,NULL,&player2pos.w,&player2pos.h);
    player2pos.x = dimScoreTwoPlayer2After.x + (dimScoreTwoPlayer2After.w/2 - player2pos.w/2) ;
    player2pos.y = 20;

    /*===========================================================
    =========================Music LOOP===========================
    ============================================================*/
    Mix_Music* main_music = Mix_LoadMUS(MAIN_MUSIC_PATH);
    if(GestionErrorMix(main_music,"Erreur au chargement de la music du jeu") == statut) goto Quit;
    Mix_PlayMusic(main_music,-1); //-1 jouer en loop
    Mix_Music* menu_selection_sound = Mix_LoadMUS(SELECTION_SOUND);
    if(GestionErrorMix(menu_selection_sound,"Erreur au chargement du sound selection menu") == statut) goto Quit;
    
    /*===========================================================
    =========================GAME LOOP===========================
    ============================================================*/
    SDL_StartTextInput(); //on permet la saisi du text
    while(run)
    {
        while(SDL_PollEvent(&events))
        {
            dontPut = SDL_FALSE;
            switch (events.type)
            {
                case SDL_WINDOWEVENT: //Boutton de fenetre
                    if(events.window.event == SDL_WINDOWEVENT_CLOSE)
                        run = SDL_FALSE;
                    else if (events.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        
                        SDL_GetWindowSize(fenetre,&WIDTH_FENETRE,&HEIGHT_FENETRE);
                        printf("new size w : %d h : %d\n",WIDTH_FENETRE,HEIGHT_FENETRE);
                        //Menu
                        menuPosAfter.x = WIDTH_FENETRE /2 - menuPosAfter.w /2;
                        menuPosAfter.y = HEIGHT_FENETRE /2 - menuPosAfter.h /2 +90;
                        //Option
                        dimOp1.x = menuPosAfter.x + 400 ; dimOp1.y = menuPosAfter.y + 550 ; dimOp1.w = 245 ; dimOp1.h = 36; //Option 1
                        dimOp2.x = dimOp1.x ; dimOp2.y = dimOp1.y + 80 ; dimOp2.w = 302 ; dimOp2.h = 36; //Option 2
                        dimOp3.x = dimOp1.x ; dimOp3.y = dimOp2.y +80 ; dimOp3.w = 200 ; dimOp3.h = 36; //Option 3
                        //CLose
                        dimCloseAfter.x = WIDTH_FENETRE - dimCloseAfter.w - 10; //en haut a droite de l'écran
                        dimCloseAfter.y = 10;
                        dimCloseAfter.x = WIDTH_FENETRE - dimCloseAfter.w - 10; //en haut a droite de l'écran
                        dimCloseAfter.y = 10;
                        //Ok
                        dimOkAfter.x = WIDTH_FENETRE - dimOkAfter.w - 10; //en haut a droite de l'écran
                        dimOkAfter.y = 10;
                        dimOkAfter.x = WIDTH_FENETRE - dimOkAfter.w - 10; //en haut a droite de l'écran
                        dimOkAfter.y = 10;
                        //Sound
                        dimSoundAfter.x = dimCloseAfter.x - dimSound.w - decalage_between_flags; //en haut a droite de l'écran
                        dimSoundAfter.y = 10;
                        dimSoundAfter.x = dimCloseAfter.x - dimSound.w - decalage_between_flags; //en haut a droite de l'écran
                        dimSoundAfter.y = 10;
                        //Flags
                        dimFrFlagAfter.x = 10; //en haut a gauche de l'écran
                        dimFrFlagAfter.y = 10;
                        dimEspFlagAfter.x = dimFrFlagAfter.x + dimFrFlagAfter.w + decalage_between_flags; //a droite de flag Fr 
                        dimEspFlagAfter.y = 10;
                        dimUkFlagAfter.x = dimEspFlagAfter.x + dimEspFlagAfter.w + decalage_between_flags; //a droite de flag esp
                        dimUkFlagAfter.y = 10;
                        //Apropos board
                        dimAproposAfter.x = WIDTH_FENETRE /2 - dimAproposAfter.w /2;
                        dimAproposAfter.y = HEIGHT_FENETRE /2 - dimAproposAfter.h /2 ;
                        //Back
                        dimBackmenuAfter.x = WIDTH_FENETRE - dimBackmenuAfter.w - 10; //en haut a droite de l'écran
                        dimBackmenuAfter.y = 10;
                        //Cadre image in game
                        dimImageBoardAfter.x = WIDTH_FENETRE /2 - dimImageBoardAfter.w /2;
                        dimImageBoardAfter.y = HEIGHT_FENETRE /2 - dimImageBoardAfter.h /2;
                        //Image in game
                        dimImageInsideAfter.x = dimImageBoardAfter.x + (dimImageBoardAfter.w - dimImageInsideAfter.w)/2;
                        dimImageInsideAfter.y = dimImageBoardAfter.y + (dimImageBoardAfter.h - dimImageInsideAfter.h)/2 - 30;
                        //Text Area
                        dimTextAreaAfter.x = WIDTH_FENETRE /2 - dimTextAreaAfter.w /2;
                        dimTextAreaAfter.y = HEIGHT_FENETRE - dimTextAreaAfter.h/ 2 - 50;
                        //TextInput
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                        //Score board
                        dimScoreOnePlayerAfter.x = WIDTH_FENETRE /2 - dimScoreOnePlayer.w /2 + 300;
                        dimScoreOnePlayerAfter.y = 0;
                        dimScoreTwoPlayer1After.x = 200;
                        dimScoreTwoPlayer1After.y = 0;
                        dimScoreTwoPlayer2After.x = WIDTH_FENETRE - 200 - dimScoreTwoPlayer1After.w;
                        dimScoreTwoPlayer2After.y = 0;
                        //text score est :
                        scorePosition.x = dimScoreOnePlayerAfter.x + (dimScoreOnePlayerAfter.w/2 - scorePosition.w/2) ;
                        scorePosition.y = 20;
                        //Le score en lui meme
                        scorePositionN.x = dimScoreOnePlayerAfter.x + (dimScoreOnePlayerAfter.w/2 - scorePositionN.w/2) ;
                        scorePositionN.y = 50;
                        //Retry:
                        dimRetryAfter.x = WIDTH_FENETRE - dimRetryAfter.w - 100; //en haut a droite de l'écran
                        dimRetryAfter.y = 10;
                        //score Joueur 2
                        score2pos.x = dimScoreTwoPlayer2After.x + (dimScoreTwoPlayer2After.w/2 - score2pos.w/2) ;
                        score2pos.y = 60;
                        //score joueur 1
                        score1pos.x = dimScoreTwoPlayer1After.x + (dimScoreTwoPlayer1After.w/2 - score1pos.w/2) ;
                        score1pos.y = 60;
                        //Nom joueur 2 : 
                        player2pos.x = dimScoreTwoPlayer2After.x + (dimScoreTwoPlayer2After.w/2 - player2pos.w/2) ;
                        player2pos.y = 20;
                        //Nom joueur 1 :
                        player1pos.x = dimScoreTwoPlayer1After.x + (dimScoreTwoPlayer1After.w/2 - player1pos.w/2) ;
                        player1pos.y = 20;
                        //Suggestion :
                        posSugg.w = 800;
                        posSugg.x = WIDTH_FENETRE/2 - posSugg.w/2 ;
                        posSugg.y = dimScoreTwoPlayer1After.h + 20;
                        //Inside image (la vrai image :)
                        dimImageInsideAfter.x = dimImageBoardAfter.x + (dimImageBoardAfter.w - dimImageInsideAfter.w)/2;
                        dimImageInsideAfter.y = dimImageBoardAfter.y + (dimImageBoardAfter.h - dimImageInsideAfter.h)/2 - 30;
                        //Text en text area:
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                    }
                    break;
                case SDL_MOUSEMOTION: //Mouvement de souris
                    printf("Mouvement de souris (%d,%d)\n",events.motion.x, events.motion.y);           //Amelioration necessaire : Englobé le tous dans une condition à 5 Block : WichOptionClicked (0,1,2,3,else(en commun))
                    //boutton close
                    if(isMouseOnRect(events.motion.x,events.motion.y,&dimCloseAfter)) isCloseHold = SDL_TRUE;
                    else isCloseHold = SDL_FALSE;
                    //boutton ok
                    if(isMouseOnRect(events.motion.x,events.motion.y,&dimOkAfter)) isOkHold = SDL_TRUE;
                    else isOkHold = SDL_FALSE;
                    //menu selection
                    if(isMouseOnRect(events.motion.x,events.motion.y,&dimOp1))
                    {
                        wichOptionHolded = 1;
                        //Mix_PlayMusic(menu_selection_sound,1);
                    }
                    else if (isMouseOnRect(events.motion.x,events.motion.y,&dimOp2))
                    {
                        wichOptionHolded = 2;
                        //Mix_PlayMusic(menu_selection_sound,1);
                    }
                    else if (isMouseOnRect(events.motion.x,events.motion.y,&dimOp3))
                    {
                        wichOptionHolded = 3;
                        //Mix_PlayMusic(menu_selection_sound,1);
                    }
                    else wichOptionHolded = 0;
                    //flag selection
                    if(isMouseOnRect(events.motion.x,events.motion.y,&dimFrFlagAfter))
                    {   
                        isFlagSelected = 1;
                    }
                    else if (isMouseOnRect(events.motion.x,events.motion.y,&dimEspFlagAfter))
                    {
                        isFlagSelected = 2;
                    }
                    else if (isMouseOnRect(events.motion.x,events.motion.y,&dimUkFlagAfter))
                    {
                        isFlagSelected = 3;
                    }
                    else isFlagSelected = 0;
                    if(isMouseOnRect(events.motion.x,events.motion.y,&dimRetryAfter))
                    {
                        RetryHolded = SDL_TRUE;
                    }
                    else{
                        RetryHolded = SDL_FALSE;
                    }
                    
                    break;
                case SDL_MOUSEBUTTONUP: //Au relachement du boutton de la souris
                    //Boutton de music
                    if(isMouseOnRect(events.button.x,events.button.y,&dimSoundAfter))
                    {
                        isSoundClick = switcherBool(isSoundClick);
                        if(isSoundClick) printf("isSoundClick est sur True\n");
                        else printf("isSoundClick est sur false\n");
                    }
                    //Boutton Close
                    if(isMouseOnRect(events.button.x,events.button.y,&dimCloseAfter))
                    {
                        if(wichOptionClicked == 0) //si on est dans la page du menu
                        {
                            run = SDL_FALSE;
                        }
                    }
                    
                    if (isMouseOnRect(events.button.x,events.button.y,&dimTextAreaAfter))
                    {
                        if(wichOptionClicked == 1 || wichOptionClicked == 2)
                        {
                            InputText[0] = '\0';
                            wantToInput = SDL_TRUE;
                            printf("textArea clicked\n");
                        }
                    }
                    if(isMouseOnRect(events.button.x,events.button.y,&dimOkAfter)) //Retour en arrière
                    {
                        if (wichOptionClicked == 1 ||wichOptionClicked == 2) //si on est en jeu on initialise le chargement de structure
                        {
                            StructLoaded = SDL_FALSE;
                            strcpy(InputText,"Saisir un mot");
                            printf("Back clicked\n");
                        }
                        wichOptionClicked = 0;
                    }
                    //Boutton Apropos
                    if(isMouseOnRect(events.button.x, events.button.y,&dimOp3)) //option 3 clické
                    {
                        if(wichOptionClicked == 0)
                        {
                            wichOptionClicked = 3;
                            printf("Option Apropos clicked\n");
                        }
                    }
                    else if (isMouseOnRect(events.button.x, events.button.y,&dimOp2)) //option 2 clické
                    {
                        if(wichOptionClicked == 0)
                        {
                            wichOptionClicked = 2;
                            printf("Option Multi clicked\n");
                        }
                    }
                    else if (isMouseOnRect(events.button.x,events.button.y,&dimOp1)) //option 1 clické
                    {
                        if(wichOptionClicked == 0)
                        {
                            wichOptionClicked = 1;
                            printf("Option Solo clicked\n");
                        }
                    }
                    //flag selection
                    if (wichOptionClicked == 0)
                    {
                        if(isMouseOnRect(events.button.x,events.button.y,&dimFrFlagAfter))
                        {   
                            isFlagClicked = 1;
                        }
                        else if (isMouseOnRect(events.button.x,events.button.y,&dimEspFlagAfter))
                        {
                            isFlagClicked = 2;
                        }
                        else if (isMouseOnRect(events.button.x,events.button.y,&dimUkFlagAfter))
                        {
                            isFlagClicked = 3;
                        }
                    }
                    //Chargement de structure 
                    if (wichOptionClicked == 1)
                    {
                        if(isMouseOnRect(events.button.x,events.button.y,&dimRetryAfter)) //Boutton retry
                        {   
                            ScoreSolo = 0;
                            StructLoaded = SDL_FALSE;
                            strcpy(InputText,"Saisir un mot");
                        }
                    }
                    if (wichOptionClicked == 2) //boutton retry
                    {
                        
                    }

                    //else isFlagClicked = 0;
                    break;
                case SDL_KEYDOWN: //Appuis sur une touche
                    //Boutton UP du clavier => augmentation du volume
                    if(events.key.keysym.sym == SDLK_UP)
                    {
                        printf("boutton UP appuyer\n");
                        volumeUp(&volume);
                        Mix_VolumeMusic(volume);
                        printf("volume : %d\n",volume);
                        dontPut = SDL_TRUE;
                    }
                    //Boutton DOWN du clavier => diminution du volume
                    if(events.key.keysym.sym == SDLK_DOWN)
                    {
                        printf("boutton DOWN appuyer\n");
                        volumeDown(&volume);
                        Mix_VolumeMusic(volume);
                        printf("volume : %d\n",volume);
                        dontPut = SDL_TRUE;
                    }
                    if(events.key.keysym.sym == SDLK_ESCAPE)
                    {
                        printf("boutton echap appuyer\n");
                        StructLoaded = SDL_FALSE; //réinitialisation de structure
                        ScoreSolo = 0;
                        ScoreP1 = 0;
                        ScoreP2 = 0;
                        strcpy(InputText,"Saisir un mot");
                        suggestionMots[0] = '\0';
                        dontPut = SDL_TRUE;
                        wichOptionClicked = 0;
                    }
                    //Cas spécial en cas de saisi :
                    if(events.key.keysym.sym == SDLK_BACKSPACE && strlen(InputText) > 0) //correction du mot
                    {   
                        InputText[0]='\0';
                        wantToInput = SDL_TRUE;
                    }
                    if( events.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL ) //Un copier coller d'un text
                    {
                        strncpy(InputText,SDL_GetClipboardText(),1000);
                        wantToInput = SDL_TRUE;
                    }
                    if ( events.key.keysym.sym == SDLK_RETURN && strlen(InputText) > 0) //Appuyer sur Entrer et y'a de l'input
                    {
                        if (wichOptionClicked == 1 || wichOptionClicked == 2) // on est en jeu la structure est deja chargé !
                        {
                            printf("Appuyer sur entré !\n");
                            ReturnPressed = SDL_TRUE;
                            dontPut = SDL_TRUE;
                        }
                    }
                    break;
                case SDL_TEXTINPUT: //detection d'une saisi ... (On ignore le copy/past)
                    if (wichOptionClicked == 1) // on n'accept les input que lorsqu'on est en jeu SOLO
                    {
                        if( !( SDL_GetModState() & KMOD_CTRL && ( events.text.text[ 0 ] == 'c' || events.text.text[ 0 ] == 'C' || events.text.text[ 0 ] == 'v' || events.text.text[ 0 ] == 'V' ) ) )
                        {
                            if (!(dontPut))
                            {
                                
                                printf("event : %s - inputAvant : %s\n",events.text.text,InputText);
                                strncat(InputText,events.text.text,1000);
                                
                                //for(int i = 0; i == strlen(InputText); i++)
                                //{
                                    //printf()
                                //}
                                
                                wantToInput = SDL_TRUE;
                            }
                        }
                    }
                    if(wichOptionClicked == 2) //mode MULTI
                    {
                        if( !( SDL_GetModState() & KMOD_CTRL && ( events.text.text[ 0 ] == 'c' || events.text.text[ 0 ] == 'C' || events.text.text[ 0 ] == 'v' || events.text.text[ 0 ] == 'V' ) ) )
                        {
                            if (!(dontPut))
                            {
                                printf("event : %s - inputAvant : %s\n",events.text.text,InputText);
                                strncat(InputText,events.text.text,1000);
                                if (SearchPhase)
                                {
                                    wantToInput = SDL_TRUE;
                                }
                                else
                                {
                                    wantToInput = SDL_FALSE;
                                }
                            }
                        }
                    }
                    break;

            }
        }
        SDL_RenderCopy(render,backgroundTexture,NULL,NULL);        //printf("menuX %d menuY %d \n",menuPosAfter.x,menuPosAfter.y);
        if(wichOptionClicked == 0) //0 : menu principale, 1 : SOLO , 2 : MULTI , 3: A PROPOS
        {
            //Retour au menu les variables du jeu doivent se réintialisé, structure dechargé
            strcpy(InputText,"Saisir un mot");
            ScoreSolo = 0;
            ScoreP1 = 0;
            ScoreP2 = 0;
            StructLoaded = SDL_FALSE;
            if(wichOptionHolded == 0)
            {
                SDL_RenderCopy(render,menuTexture,&menuPos,&menuPosAfter);
            }
            else if (wichOptionHolded == 1)
            {
                SDL_RenderCopy(render,menuTexture1,&menuPos,&menuPosAfter);
                
            }
            else if (wichOptionHolded == 2)
            {   
                SDL_RenderCopy(render,menuTexture2,&menuPos,&menuPosAfter);
            }
            else if (wichOptionHolded == 3)
            {
                SDL_RenderCopy(render,menuTexture3,&menuPos,&menuPosAfter);
            }
            if(isCloseHold)
                SDL_RenderCopy(render,closeHTexture,&dimClose,&dimCloseAfter);
            else
                SDL_RenderCopy(render,closeTexture,&dimClose,&dimCloseAfter);
            if(isSoundClick)
            {
                SDL_RenderCopy(render,soundHTexture,&dimSound,&dimSoundAfter);
                Mix_PauseMusic();
            }
            else
            {   
                SDL_RenderCopy(render,soundTexture,&dimSound,&dimSoundAfter);
                Mix_ResumeMusic();
            }
            if(isFlagSelected == 0)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
            }
            else if (isFlagSelected == 1)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
                SDL_SetRenderDrawColor(render,255,0,0,255);
                SDL_RenderDrawRect(render,&dimFrFlagAfter);

            }
            else if (isFlagSelected == 2)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
                SDL_SetRenderDrawColor(render,255,0,0,255);
                SDL_RenderDrawRect(render,&dimEspFlagAfter);
            }
            else if (isFlagSelected == 3)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
                SDL_SetRenderDrawColor(render,255,0,0,255);
                SDL_RenderDrawRect(render,&dimUkFlagAfter);
            }

            if (isFlagClicked == 1)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
                SDL_SetRenderDrawColor(render,0,255,0,255);
                SDL_RenderDrawRect(render,&dimFrFlagAfter);

            }
            else if (isFlagClicked == 2)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
                SDL_SetRenderDrawColor(render,0,255,0,255);
                SDL_RenderDrawRect(render,&dimEspFlagAfter);
            }
            else if (isFlagClicked == 3)
            {
                SDL_RenderCopy(render,FrFlagTexture,&dimFrFlag,&dimFrFlagAfter);
                SDL_RenderCopy(render,EspFlagTexture,&dimEspFlag,&dimEspFlagAfter);
                SDL_RenderCopy(render,UkFlagTexture,&dimUkFlag,&dimUkFlagAfter);
                SDL_SetRenderDrawColor(render,0,255,0,255);
                SDL_RenderDrawRect(render,&dimUkFlagAfter);
            }
        }
        else if (wichOptionClicked == 3) //Appropos clické
        {
            SDL_RenderCopy(render,aproposTexture,&dimApropos,&dimAproposAfter);
            if(isOkHold)
            {
                SDL_RenderCopy(render,okHTexture,&dimOk,&dimOkAfter);
            }
            else
            {
                SDL_RenderCopy(render,okTexture,&dimOk,&dimOkAfter);
            }
            printf("on est ici \n");
        }
        else if (wichOptionClicked == 2) //Multi clické
        {
            
            if (! StructLoaded){
                root2 = NULL;
                //En cours de construction ...
                char** fileTab;             //Tableau de tous les mots de notre fichier
                char** FrTab; 
                //Chargement de la structure ...
                count_mots = 0; // reinitialisation (on sait jamais)
                fileTab = FileToTable("motsV22.txt");   //fichier doit se terminer avec un séparateur en plus (il marque un pointeur vers une chaine NULL) 
                FrTab = (char**)malloc(sizeof(char*)); // preparer les champs du tableau (un seul champ au départ)
                for (int i = 0; fileTab[i] != NULL; i++)
                {
                    count_mots++;
                    char** splitTab = split(fileTab[i],",",1);    //chaque chaine de file tab sera séparer selon "," et on accepte pas le vide
                    int taille_motFr = strlen(splitTab[0]);
                    //Partie creation arbre : 
                    if(isFlagClicked == 0)
                    {
                        isFlagClicked = 1; // au cas où on a pas clic sur un flag on choisi celui de FR
                    }
                    Enregistrement* v = newValeur(splitTab[0],splitTab[isFlagClicked-1],splitTab[3]); // création de l'enregistrement
                    printf("[%d] : MotFr : %s, Deuxième Langue (%d) : %s, AdresseImage : %s \n",i,v->MotFr,(isFlagClicked-1),v->Mot2Langue,v->AdresseImage);
                    root2 = AjoutAVL(root2,v); //insertion en arbre
                    //Partie chargement du tableau FR
                    FrTab[i] = (char*)malloc(sizeof(char)*(taille_motFr+1)); //Rappel une désalocation avant de quiter le jeu ! 
                    strcpy(FrTab[i],splitTab[0]);
                    //printf("ici\n");
                    FrTab[i][taille_motFr] = '\0';
                    cpt_NbrMot ++; //preparation prochain champs
                    FrTab = (char**)realloc(FrTab,sizeof(char*)*(cpt_NbrMot+1));
                    FrTab[cpt_NbrMot] = NULL; //tableau se terminant avec un champs null
                }
                
                indexFr = randomAB(0,count_mots-5); //tiré au sort un indice du tableau FrTab (le -5! car on aura besoin de 5 mots , assurer qu'on est pas à la fin du tableau)
                suggestionMots[0] = '\0'; //on sait jamais
                strcat(suggestionMots,"Suggestions: ");
                for (int i = 0; i<5; i++)
                {
                    strcat(suggestionMots,"[");
                    strcat(suggestionMots,FrTab[indexFr+i]);
                    strcat(suggestionMots,"]");
                    strcat(suggestionMots," || ");
                }
                suggestionMots[strlen(suggestionMots)-3] = '\0'; // faire jolie quoi ? u_U
                printf("Suggestion : %s\n",suggestionMots);
                fontsugg = insertText(MENU_POLICE,36,fontColorWhite,suggestionMots,render);
                SDL_QueryTexture(fontPlayer1,NULL,NULL,&posSugg.w,&posSugg.h);
                posSugg.w = 800;
                posSugg.x = WIDTH_FENETRE/2 - posSugg.w/2 ;
                posSugg.y = dimScoreTwoPlayer1After.h + 20;
                

                
                StructLoaded = SDL_TRUE;
                SearchPhase = SDL_FALSE; //qui sait ?(s'assurer c bien)
                PlayerTurn = 1; // toujours le premier joueur qui va faire chercher le 2 eme  
            }
            
        
            if (ReturnPressed) //l'un des joueurs a appuyer sur entré 
            {
                if (SearchPhase == SDL_FALSE) //l'image n'est pas encore afficher, on vient de saisir le mot qu'il faut trouver
                {   
                    //réinitialisation des chaines pour le nouveau stockage (s'assurer c bien !!!)
                    MotLangueChoisie[0] = '\0';
                    AdressAbsolutImage[0] = '\0';
                    //recherche ...
                    strcpy(RechercheMot,InputText);
                    ParcoursAVL(root2);
                    if (RechercheAVL(root2,RechercheMot,MotLangueChoisie,AdressAbsolutImage) == 1)
                    {
                        //switcher les tours
                        if (PlayerTurn == 1)
                        {
                            PlayerTurn = 2;
                            //ScoreP2 = 1;    //initialisation score deuxième joueur ? NON ON VEUT ACCUMULER LE SCORE !
                        }
                        else if (PlayerTurn == 2) // pourquoi pas un else ? j'ai declarer PlayerTurn à 0, (mieux s'assurer !!)
                        {
                            PlayerTurn = 1;
                            //ScoreP1 = 1; //initialisation score premier joueur ? NON ON VEUT ACCUMULER LE SCORE !
                        }
                        //SWitch vers SearchPhase
                        SearchPhase = SDL_TRUE;
                        strcpy(InputText,"Saisir un mot");
                        printf("le mot attendu est : %s, l'adresse image sortie est : %s\n",MotLangueChoisie,AdressAbsolutImage);
                        imageInside = loadImage(AdressAbsolutImage,render);
                        if(GestionError(imageInside,"Erreur au chargement du conteneur de l'image en jeu")==statut) goto Quit;
                        SDL_QueryTexture(imageInside,NULL,NULL,&dimImageInside.w,&dimImageInside.h);
                        dimImageInsideAfter.x = dimImageBoardAfter.x + (dimImageBoardAfter.w - dimImageInsideAfter.w)/2;
                        dimImageInsideAfter.y = dimImageBoardAfter.y + (dimImageBoardAfter.h - dimImageInsideAfter.h)/2 - 30;
                    }
                    else
                    {
                        printf("mot rechercher introuvable dans la bdd {%s}\n",InputText);
                        //on réintialise l'input text 
                        strcpy(InputText,"il n y'a pas d'images pour ce mot");
                    }
                }
                else
                {
                    if(strcmp(InputText,MotLangueChoisie) == 0)
                    {
                        strcpy(InputText,"Bravo !");
                        fontMenu = insertText(MENU_POLICE, 24, fontColorSucces,InputText, render);
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                        isWinning = SDL_TRUE;
                        
                    }
                    else
                    {
                        tentative++;
                        printf("tentative encours num : %d",tentative);
                        strcpy(InputText,"Mauvaise reponse !");
                        fontMenu = insertText(MENU_POLICE, 24, fontColorError,InputText, render);
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                    }
                }
                ReturnPressed = SDL_FALSE;
            }
            //dans le cas ou l'image est deja afficher, alors on affiche le texte de saisie
            if( wantToInput )
            {
                if (SearchPhase){
                    //Text non vide
                    if( InputText[0] != '\0' )
                    {
                        //Render new text
                        fontMenu = insertText(MENU_POLICE, 24, fontColorMessage, InputText, render);
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                    }
                    //Text vide (Ahaaa !!!)
                    else
                    {
                        //Render space texture
                        fontMenu = insertText(MENU_POLICE, 24, fontColorMessage, " ", render);
                        
                    }
                }
            }
            if(isWinning)
            {
                //[Debug]
                printf("tentative gagné num : %d",tentative);
                if (PlayerTurn == 2)
                {
                    ScoreP2 += Score(tentative,100);
                    printf("le score du joueur {2} est : %d\n",ScoreP1);//[Debug]
                    fontScore2 = insertText(MENU_POLICE, 24,fontColorWhite,itoa(ScoreP2,10), render);
                    if(GestionError(fontScore2,"Erreur au chargement du font de score Num joueur 2")==statut) goto Quit;
                    SDL_QueryTexture(fontScore2,NULL,NULL,&score2pos.w,&score2pos.h);
                    score2pos.x = dimScoreTwoPlayer2After.x + (dimScoreTwoPlayer2After.w/2 - score2pos.w/2) ;
                    score2pos.y = 60;
                }
                else if (PlayerTurn == 1)
                {
                    ScoreP1 += Score(tentative,100);
                    printf("le score du joueur {2} est : %d\n",ScoreP1);//[Debug]
                    fontScore1 = insertText(MENU_POLICE, 24,fontColorWhite,itoa(ScoreP1,10), render);
                    if(GestionError(fontScore1,"Erreur au chargement du font de score Num joueur 2")==statut) goto Quit;
                    SDL_QueryTexture(fontScore1,NULL,NULL,&score1pos.w,&score1pos.h);
                    score1pos.x = dimScoreTwoPlayer1After.x + (dimScoreTwoPlayer1After.w/2 - score1pos.w/2) ;
                    score1pos.y = 60;                 
                }
                //retablire l'etat par défaut
                isWinning = SDL_FALSE;
                tentative = 1;
                SearchPhase = SDL_FALSE;
            }
            if (PlayerTurn == 1)
            {
                fontPlayer1 = insertText(MENU_POLICE, 24,fontColorSucces,"[Joueur 1]:", render);
                fontPlayer2 = insertText(MENU_POLICE, 24,fontColorWhite,"[Joueur 1]:", render);
            }
            else if (PlayerTurn == 2)
            {
                fontPlayer2 = insertText(MENU_POLICE, 24,fontColorSucces,"[Joueur 2]:", render);
                fontPlayer1 = insertText(MENU_POLICE, 24,fontColorWhite,"[Joueur 1]:", render);
            }
            SDL_RenderCopy(render,scoretwoplayer2Texture,&dimScoreTwoPlayer2,&dimScoreTwoPlayer2After);
            SDL_RenderCopy(render,scoretwoplayer1Texture,&dimScoreTwoPlayer1,&dimScoreTwoPlayer1After);
            SDL_RenderCopy(render,backmenuTexture,&dimBackmenu,&dimBackmenuAfter);
            SDL_RenderCopy(render,imageboardTexture,&dimImageBoard,&dimImageBoardAfter);
            SDL_RenderCopy(render,textareaTexture,&dimTextArea,&dimTextAreaAfter);
            SDL_RenderCopy(render,imageInside,&dimImageInside,&dimImageInsideAfter);
            if (SearchPhase == SDL_FALSE)
            {
                SDL_RenderCopy(render,fontsugg,NULL,&posSugg);
            }
            else
            {
                if(wantToInput)
                {
                    SDL_RenderCopy(render,fontMenu,NULL,&position);
                }
            }
            SDL_RenderCopy(render,fontPlayer1,NULL,&player1pos);
            SDL_RenderCopy(render,fontPlayer2,NULL,&player2pos);
            if(ScoreP1 > 0)
            {
                SDL_RenderCopy(render,fontScore1,NULL,&score1pos);
            }
            if(ScoreP2 > 0)
            {
                SDL_RenderCopy(render,fontScore2,NULL,&score2pos);
            }

            
        }
        else if (wichOptionClicked == 1) //Solo clické              FINI ! (Objectif : Amelioration graphique/texte ...)
        {   
            if (! StructLoaded )
            {
                /*=======================================================================================================================================================================*/
                char** fileTab;             //Tableau de tous les mots de notre fichier
                char** FrTab; 
                Arbre* root = NULL;
                //Chargement de la structure ...
                count_mots = 0; // reinitialisation (on sait jamais)
                fileTab = FileToTable("motsV22.txt");   //fichier doit se terminer avec un séparateur en plus (il marque un pointeur vers une chaine NULL) 
                FrTab = (char**)malloc(sizeof(char*)); // preparer les champs du tableau (un seul champ au départ)
                for (int i = 0; fileTab[i] != NULL; i++)
                {
                    count_mots++;
                    char** splitTab = split(fileTab[i],",",1);    //chaque chaine de file tab sera séparer selon "," et on accepte pas le vide
                    int taille_motFr = strlen(splitTab[0]);
                    //Partie creation arbre : 
                    if(isFlagClicked == 0)
                    {
                        isFlagClicked = 1; // au cas où on a pas clic sur un flag on choisi celui de FR
                    }
                    Enregistrement* v = newValeur(splitTab[0],splitTab[isFlagClicked-1],splitTab[3]); // création de l'enregistrement
                    printf("[%d] : MotFr : %s, Deuxième Langue (%d) : %s, AdresseImage : %s \n",i,v->MotFr,(isFlagClicked-1),v->Mot2Langue,v->AdresseImage);
                    root = AjoutAVL(root,v); //insertion en arbre
                /*=======================================================================================================================================================================*/

                    //printf("ici\n");
                    //Partie chargement du tableau FR
                    FrTab[i] = (char*)malloc(sizeof(char)*(taille_motFr+1)); //Rappel une désalocation avant de quiter le jeu ! 
                    strcpy(FrTab[i],splitTab[0]);
                    //printf("ici\n");
                    FrTab[i][taille_motFr] = '\0';
                    cpt_NbrMot ++; //preparation prochain champs
                    FrTab = (char**)realloc(FrTab,sizeof(char*)*(cpt_NbrMot+1));
                    FrTab[cpt_NbrMot] = NULL; //tableau se terminant avec un champs null
                    
                }
                //tirage d'une image aleatoire :
                indexFr = randomAB(0,count_mots); //tiré au sort un indice du tableau FrTab
                strcpy(RechercheMot,FrTab[indexFr]); //on stock le mot fr tiré au sort dans une chaine
                if (RechercheAVL(root,RechercheMot,MotLangueChoisie,AdressAbsolutImage) == 0)
                {
                    printf("Une erreur au tirage d'un mot aleatoire : le mot tiré (%s) n'existe pas en AVL \n",FrTab[indexFr]);
                    goto Quit;
                }
                printf("le mot attendu est : %s, l'adresse image sortie est : %s\n",MotLangueChoisie,AdressAbsolutImage);
                //char* Path = AdressAbsolutImage;
                //printf("path : %s",Path);
                imageInside = loadImage(AdressAbsolutImage,render);
                if(GestionError(imageInside,"Erreur au chargement du conteneur de l'image en jeu")==statut) goto Quit;
                SDL_QueryTexture(imageInside,NULL,NULL,&dimImageInside.w,&dimImageInside.h);
                dimImageInsideAfter.x = dimImageBoardAfter.x + (dimImageBoardAfter.w - dimImageInsideAfter.w)/2;
                dimImageInsideAfter.y = dimImageBoardAfter.y + (dimImageBoardAfter.h - dimImageInsideAfter.h)/2 - 30;
                StructLoaded = SDL_TRUE;
            }
            //SDL render
            //render le text si on veut
                if( wantToInput )
                {
                    //Text non vide
                    if( InputText[0] != '\0' )
                    {
                        //Render new text
                        fontMenu = insertText(MENU_POLICE, 24, fontColorMessage, InputText, render);
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                    }
                    //Text vide (Ahaaa !!!)
                    else
                    {
                        //Render space texture
                        fontMenu = insertText(MENU_POLICE, 24, fontColorMessage, " ", render);
                        
                    }
                }
                if(ReturnPressed)
                {
                    if(strcmp(InputText,MotLangueChoisie) == 0)
                    {
                        strcpy(InputText,"Bravo !");
                        fontMenu = insertText(MENU_POLICE, 24, fontColorSucces,InputText, render);
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                        isWinning = SDL_TRUE;
                    }
                    else
                    {
                        tentative++;
                        printf("tentative encours num : %d",tentative);
                        strcpy(InputText,"Mauvaise reponse !");
                        fontMenu = insertText(MENU_POLICE, 24, fontColorError,InputText, render);
                        position.x = dimTextAreaAfter.x + (dimTextAreaAfter.w - position.w)/2 ;
                        position.y = dimTextAreaAfter.y + (dimTextAreaAfter.h/2 - position.h)/2;
                    }
                    ReturnPressed = SDL_FALSE;
                }
                if(isWinning)
                {
                    printf("tentative gagné num : %d",tentative);
                    ScoreSolo = Score(tentative,100);
                    printf("le score du joueur est : %d\n",ScoreSolo);
                    fontScoreN = insertText(MENU_POLICE, 24,fontColorWhite,itoa(ScoreSolo,10), render);
                    if(GestionError(fontScoreN,"Erreur au chargement du font de score Num")==statut) goto Quit;
                    SDL_QueryTexture(fontScoreN,NULL,NULL,&scorePositionN.w,&scorePositionN.h);
                    scorePositionN.x = dimScoreOnePlayerAfter.x + (dimScoreOnePlayerAfter.w/2 - scorePositionN.w/2) ;
                    scorePositionN.y = 60;
                    //retablire l'etat par défaut
                    isWinning = SDL_FALSE;
                    tentative = 1;
                    //wichOptionClicked = 0;  //revenir au menu principal pour l'instant 
                }
            
            SDL_RenderCopy(render,scoreoneplayerTexture,&dimScoreOnePlayer,&dimScoreOnePlayerAfter);
            SDL_RenderCopy(render,backmenuTexture,&dimBackmenu,&dimBackmenuAfter);
            SDL_RenderCopy(render,imageboardTexture,&dimImageBoard,&dimImageBoardAfter);
            SDL_RenderCopy(render,textareaTexture,&dimTextArea,&dimTextAreaAfter);
            SDL_RenderCopy(render,imageInside,&dimImageInside,&dimImageInsideAfter);
            SDL_RenderCopy(render,fontMenu,NULL,&position);
            SDL_RenderCopy(render,fontScore,NULL,&scorePosition);
            if(RetryHolded)
            {
                SDL_RenderCopy(render,retryHTexture,&dimRetry,&dimRetryAfter);
            }
            else
            {
                SDL_RenderCopy(render,retryTexture,&dimRetry,&dimRetryAfter);
            }
            if(ScoreSolo > 0)
            {
                SDL_RenderCopy(render,fontScoreN,NULL,&scorePositionN);
            }
        }
        SDL_RenderPresent(render);    
    }
    statut = EXIT_SUCCESS; // tout s'est bien executer yupi !
    /*===========================================================
    =====================LIBERATION MEMOIRE======================
    ============================================================*/
Quit:
    //fontScore1
    
    //SDL QUIT
    if (fontsugg != NULL) SDL_DestroyTexture(fontsugg);
    if (fontPlayer2 != NULL) SDL_DestroyTexture(fontPlayer2);
    if (fontPlayer1 != NULL) SDL_DestroyTexture(fontPlayer1);
    if (fontScore2 != NULL) SDL_DestroyTexture(fontScore2);
    if (fontScore1 != NULL) SDL_DestroyTexture(fontScore1);
    if (fontScoreN != NULL) SDL_DestroyTexture(fontScoreN);
    if (fontMenu != NULL) SDL_DestroyTexture(fontMenu);
    if (retryTexture != NULL) SDL_DestroyTexture(retryTexture);
    if (retryHTexture != NULL) SDL_DestroyTexture(retryHTexture);
    if (imageInside != NULL) SDL_DestroyTexture(imageInside);
    if (scoreoneplayerTexture != NULL) SDL_DestroyTexture(scoreoneplayerTexture);
    if (scoretwoplayer1Texture != NULL) SDL_DestroyTexture(scoretwoplayer1Texture);
    if (scoretwoplayer2Texture != NULL) SDL_DestroyTexture(scoretwoplayer2Texture);
    if (textareaTexture != NULL) SDL_DestroyTexture(textareaTexture);
    if (imageboardTexture != NULL) SDL_DestroyTexture(imageboardTexture);
    if (backmenuTexture != NULL) SDL_DestroyTexture(backmenuTexture);
    if (okHTexture != NULL) SDL_DestroyTexture(okHTexture);
    if (okTexture != NULL) SDL_DestroyTexture(okTexture);
    if (aproposTexture != NULL) SDL_DestroyTexture(aproposTexture);
    if (menu_selection_sound != NULL) Mix_FreeMusic(menu_selection_sound);
    if (main_music != NULL) Mix_FreeMusic(main_music);
    if (UkFlagTexture != NULL) SDL_DestroyTexture(UkFlagTexture);
    if (EspFlagTexture != NULL) SDL_DestroyTexture(EspFlagTexture);
    if (FrFlagTexture != NULL) SDL_DestroyTexture(FrFlagTexture);
    if (soundHTexture != NULL) SDL_DestroyTexture(soundHTexture);
    if (soundTexture != NULL) SDL_DestroyTexture(soundTexture);
    if (closeHTexture != NULL) SDL_DestroyTexture(closeHTexture);
    if (closeTexture != NULL) SDL_DestroyTexture(closeTexture);
    if (menuTexture != NULL) SDL_DestroyTexture(menuTexture);
    if (menuTexture1 != NULL) SDL_DestroyTexture(menuTexture1);
    if (menuTexture2 != NULL) SDL_DestroyTexture(menuTexture2);
    if (menuTexture3 != NULL) SDL_DestroyTexture(menuTexture3);
    if (backgroundTexture != NULL) SDL_DestroyTexture(backgroundTexture);
    if (render != NULL) SDL_DestroyRenderer(render);
    if (fenetre != NULL) SDL_DestroyWindow(fenetre);
    SDL_StopTextInput();
    Mix_CloseAudio(); //on quitte le mixer
    TTF_Quit(); //on quitte TTF
    SDL_Quit();  //on quitte la SDL
    return statut;
}