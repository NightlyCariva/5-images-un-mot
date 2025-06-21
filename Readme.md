--------------------------------------------------------------------------------------------------
---------------------------------------- CONFIGURATION -------------------------------------------
--------------------------------------------------------------------------------------------------

Remarque : Vous Pouvez voir les Videos Pour plus d'informations (ou pour une partie spécifique) :

Video P1 => Les instalations, Rappel sur mes Ajouts (étape 4), Visualisation Mode Debug, Jeu SOLO
Video P2 => Jeu MULTI, Contenu & Role des fichiers .Txt
Video P3 => Presentation de la version stable et comment l'utiliser
Video P4 => Extraction des Images & Contenu de chaque fichier/Dossier
										________________________
---------------------------------------|POUR LE LANCEMENT DU JEU|---------------------------------
				        				¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
1-Telecharger les modules suivant : 
 _____
| SDL | 	
 ¯¯¯¯¯
=> sudo apt-get install libsdl2-dev
 _________
|SDL_IMAGE| 	
 ¯¯¯¯¯¯¯¯¯
=> sudo apt-get install libsdl2-image-dev
 _______
|SDL_TTF|
 ¯¯¯¯¯¯¯
=> sudo apt-get install libsdl2-ttf-dev
 _________
|SDL_MIXER|
 ¯¯¯¯¯¯¯¯¯
=> sudo apt-get install libsdl2-mixer-dev

				     				  ___________________________________
-------------------------------------|POUR LE TEST DU GENERATEUR D'IMAGES|-------------------------
				      				  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
Le script permettant de telechager/renomer/repertorier les images est fait Avec Selenium sur Python (3.7):
 ________________________
| PYTHON V3 OU ULTERIEUR |
 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
=> sudo apt-get install python3.7
 _____
| PIP | 	
 ¯¯¯¯¯
1-Telechargement de pip :
=> curl -O https://bootstrap.pypa.io/get-pip.py

2-Execution du script telecharger (instalation de pip)
=>python3 get-pip.py --user
 __________
| Selenium | 	
 ¯¯¯¯¯¯¯¯¯¯
=>pip install selenium
 _________________________
| Undetected_ChromeDriver | 	
 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
=>pip install undetected-chromedriver

Remarque :Pour plus d'info sur cette version du webDriver: https://github.com/ultrafunkamsterdam/undetected-chromedriver#readme

--------------------------------------------------------------------------------------------------
----------------------------------------UTILISATION-----------------------------------------------
--------------------------------------------------------------------------------------------------

					________________
---------------------------------------|LANCEMENT DU JEU|-----------------------------------------
				        ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ 
 ________________________________________________________________________________________________________________________________
|							Remarque								 |
|					/!\ Il Existe 2 Versions du jeu (deux fichiers F1.c) /!\				 |
| 																 |
|	En effet comme indiqué lors du dernier rendu (étape 4), j'ai ajouté d'autres fonctinnalitées "comfort" (pour images)	 |
|Malheuresement j'ai pris froid à la dernière semaine et je suis tombé malade j'ai pas pu terminer tous les testes sur ces ajouts|
| j'ai donc laisser une version "Stable" du jeu qui ne contiennent pas ces fonctionnalités dans le chemin StableVersion/F1.c     |
| ce fichier contient le jeu en essentiel (toutes les fonctionnalitées décrites dans les précedents rapports), et auquelle	 |
|				j'ai effectué plusieur test (tous semble fonctionner)						 |
| 																 |
| 	Cependant le fichier dans le dossier principale du jeu SAE/F1.c s'agit du jeu avec l'ajout des nouveaux fonctions :      |
| 1-Avoir plusieurs images pour un seul mot (5 images pour chaque mot)								 |
| 2-Pouvoir switch d'images pour le meme mot avec deux boutton "<= =>" en boucle						 |
| 3-Ajouter +7000Mots (35000 images) (Construction du script : SAE/PythonImagesScripts/ImagesD.py)				 |
| 4-Pouvoir Controller les affichages sur le terminal (Mode Debug)								 |
|  /!\Les images ont besoin de verification, nom valable,taille de la chaine... (chose qui prends du temps en vue le volume)/!\  |
|________________________________________________________________________________________________________________________________|


1- Le make contient la compilation du programme principal à savoir : "F1.c"

=>Vous pouvez choisir la version que vous voulez lancer en changeant juste le fichier F1.c par le deuxième dans StableVersion

2-Pour démarrer le jeu il suffit donc de lancé l'executable de F1 apres le make

3- Afin de vous faciliter l'analyse du code, il existe une multitudes d'informations controlable via "F1.c" s'affichant via Terminal
=>Pour Controller Ce que vous voulez voir/Ne pas voir : cmd+F -> cherchez //DEBUG MODE:  (ligne 58)
				/!\(Valable uniquement pour la dernière version)/!\
 ________________________________________________________________________________________________________________________________
|							Attention								 |
|		/!\ "Normalement" et sauf erreur de ma part j'ai bien fait attention au rennomage des images  /!\		 |
|     /!\ Cependant (qui sait ?) il se peut que vous tomberiez sur une image avec un nom inadéquat (exp: Base de donné.png)  /!\ |	
|    /!\ Dans ce cas vous pouvez relancer le jeu ou modifier le nom de l'image dans le fichier wordsV22 et en renomant l'image/!\|
|			/!\ A noté que les images se trouve dans le repertoire images/words_images/!\				 |
|	/!\ Les logs (erreurs,boutton/touche clické,réponse attendu, adresse image loadé ... sont afficher au terminal /!\	 |
|________________________________________________________________________________________________________________________________|

--------------------------------------------------------------------------------------------------
----------------------------------------CONTENU DU DOSSIER----------------------------------------
--------------------------------------------------------------------------------------------------

				      	--Node.c--
C'est un Fichier C contenant que les fonctions utilisé pour la construction de la structure du jeu 
A savoir : 
	=> Des fonctions Pour construction d'AVL.
	=> Des fonctions pour chargement gestion du fichier (charger fichier en tableau, tirage au sort ...etc)

				        --Split.c--
C'est un Fichier C contenant principalement des fonctions utilisé pour la construction des fonctions necessaire au traitement du fichier. 
	=> Des fonctions pour chargement gestion du fichier (fonction Split et fonction pour charger ficher dans un tableau)

				        --Fonction.c / includeX.h--
C'est le Header principal du jeu celui-ci regroupe tous les fonctions utilisé (apres test de fonctionnement) qui sont utiliser comme fonction sur le jeu

					--Dossier Images--
Ce dossier regroupe l'enssemble des images chargé sur le jeu il est construit comme suit : 

      _[Images]_
     |		|
     |	        |
     |		|___[words_images]>>>>Contient les images correspondant au mots du dictionnaire pour le jeu en StableVersion
     |		|
     |		|
     |		|___[menu_states]>>>>Contient tous les images necessaire aux interaction avec le menu du jeu
     |		|
     |		|
     |		|___[ingame]>>>>Contient les images du board pour le jeu (ScorsBoard,Zone de text, Cadre Image...)
     |		|
     |		|
     |		|___[icones]>>>>Contient les icones/states du jeu (icones Exit,Retry,Back,Ok,...etc)
     |		|
     |		|
     |          |___[Apropos]>>>>Contient les images relative à l'option Apropos
     |		|
     |		|
     |		|___[Save]>>>>Contient les images utiliser pour le jeu dans la dernière version
     |		|
     |__________|

				--Dossier Police--
Ce dossier contient les polices utiliser en jeu (pour le moment 04/12 : Groovy.ttf et Roboto.ttf)

				--Dossier Sounds--
Ce dossier contient la music principal du jeu ainsi que des soundsEffect.

				--PDF Algorithme&Compléxité--
Ce PDF contient les algorthimes nécessaire pour la construction de la structure.(ATTENTION: il ne contient pas tous les fonctions, que ceux necessaire pour l'AVL et ceux pouvant etre algorithmé
			(pas d'algo pour traitement de fichier etc)
				
				--Dossier PythonImagesScripts--
Ce dossier contient le script utiliser pour la réccupération des images
				
				--Dossier Downloads--
En cas de test du script generateur d'image, les images telecharger seront telecharger sur ce dossier

--------------------------------------------------------------------------------------------------
----------------------------------!!! REMARQUES : 04/12 !!!---------------------------------------
--------------------------------------------------------------------------------------------------

*_ Pour le moment et en vu du manque du temps pour traiter tous les images(Verification/rennomage...etc):
	Le jeu charge en lui 180 Images/mots, Ce nombre devra passer à 1000 d'ici le dépot du projet complet.
*_Si vous le souhaiter vous pouvez tester uniquement les fonctions correspondante à la construction de l'architecture:
	En effet les fichiers Node.c et split.c contient en eux un main (servis pour des tests) que vous pouvez décommanter et éxecuté.
*_ATTENTION: le PDF Algorithme&Compléxité ne contient que les algo necessaire pour l'AVL et ceux pouvant etre algorithmé (pas d'algo pour traitement de fichier/allocation mémoire etc).

--------------------------------------------------------------------------------------------------
----------------------------------!!! REMARQUES : 17/12 !!!---------------------------------------
--------------------------------------------------------------------------------------------------

*_ Vous trouverez plusieurs fichier.txt pour les mots , en effet :
	Mon but est de passer à +7000 mots toute en donnant la possibilité à l'utilisateur de d'avoir au minimum 5 images par mot
	Le fichier words.txt contient ~7000 mots cependant ils ne sont pas tous trié (il faut enlevé les mots longs, avec des espaces, charactère spéciales ...etc)
	Une chose que j'ai pas pu finir à temps (pour des raisons hors de mon sort)
	j'ai donc fait en sorte de trouver une solution milieu:
	 =>Finir la version Faite en 04/12 en y ajoutant le mode multi joueur (une image par mot, et que 180 mots)
	 =>Faire une nouvelle version contenant les ajouts (bouttons,fonctions,script... etc)
	 =>Réccupérer une liste de mots sur https://www.listedemots.com/
         =>Telecharger tous les images correspondante a ces mots dossier SAE/images/save
	 =>Ne prendre que ~1700 mots (8500 images) dont je suis +/- sur de leurs fonctionnements
	 =>Diviser les fichiers en : words.txt (tous les mots), wordsV22.txt(1700 mots traité), motsV22.txt(Pour la version stable)
*_Les Remarques du 04/12 restent valables
