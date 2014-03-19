#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_mixer.h>
#include <tinyxml.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include "const.h"
#include "Timer.h"
#include "Element.h"
#include "Avatar.h"
#include "Force.h"
#include "Window.h"
#include "Menu.h"

using namespace std ;

/**************
* DECLARATION *
**************/
/*** Fenetre et Event ***/
//Event
SDL_Event event ;
//Etat des touches
Uint8* keystates = SDL_GetKeyState( NULL );
//Variable de fenêtrage
bool windowed = true ;
//Calcul pour le FPS
FPSmanager fps_manager;
int frame = 0  ;
//Caméra
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//variable de sortie
bool quit = false;
//si on est dans un menu
Menu menu ;
//statut
bool partieEnCours = false ;

//font
TTF_Font *font ;
//La couleur du Font
SDL_Color textColor = { 220, 220, 220 };
//message touche d
SDL_Surface *touche_d = NULL;
//message touche s
SDL_Surface *touche_s = NULL;
//message touche x
SDL_Surface *touche_x = NULL;
//message touche echap
SDL_Surface *touche_echap = NULL;
//message touche space
SDL_Surface *touche_space = NULL;
//message touche flèche
SDL_Surface *touche_fleche = NULL;
//message touche g
SDL_Surface *touche_g = NULL;
//message touche f
SDL_Surface *touche_f = NULL;
//message touche p
SDL_Surface *touche_p = NULL;
//message touche h
SDL_Surface *touche_h = NULL;


/*** Images ***/
//l'écran
SDL_Surface* screen = NULL ;
//image vide pour les objets invisibles
SDL_Surface* image_null = NULL ;
//création des images du jeu
SDL_Surface* background = NULL;
SDL_Surface* block = NULL;

/*** Son ***/
Mix_Music *music = NULL;
Mix_Chunk *tir = NULL;

/*** Tableau ***/
//missile
vector<Missile> Missiles;
vector<Missile> BadMissiles;
//Itérateur missile
vector<Missile>::iterator itMissiles;
//décors
vector<Element> Elements;
//Avatars
vector<Avatar> Avatars;
//Itérateur avatars
vector<Avatar>::iterator itAvatars;
//Ennemis
vector<Avatar> Ennemies;
//Itérateur ennemis
vector<Avatar>::iterator itEnnemies;

/*** Forces ***/
Force Gravite(0,2) ;


/*** Timer ***/
Timer fps;
Timer update;


/***********
* FONCTION *
***********/

//fonction d'assainissage des images
SDL_Surface *load_image( string filename ) {
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = SDL_LoadBMP( filename.c_str() );
	if( loadedImage != NULL ) {
		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
	}
    if( optimizedImage != NULL )
    {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF );
           	//On met tous les pixels de couleur R 0, G 0xFF, B 0xFF transparents
	        SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
	        return optimizedImage;
    }
}

//ajouter un calque
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

//initialisation de l'application
bool init() {
    if( TTF_Init() == -1 ) {
    	return false;
    }
    camera.x = 0 ;
    //Initialisation de la fenêtre écran
    //screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN );
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,  SDL_SWSURFACE | SDL_RESIZABLE );
    //création de la fenêtre de base
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		return EXIT_FAILURE;
	}
	SDL_WM_SetCaption( "CPC le jeu", NULL );


    //Initialisation des compteurs Framerate
    SDL_initFramerate(&fps_manager);
    SDL_setFramerate(&fps_manager,FRAMES_PER_SECOND);

    //Initialisation du son
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }
    //Chargement des effets sonores
    //laser = Mix_LoadWAV( "ressource/niveau1.wav" );

    //DEBUT chargement des collisions (fichier xml)
    TiXmlDocument blocks("ressource/level_1/blocks.xml");
    if(!blocks.LoadFile()){
         cerr << "erreur lors du chargement de block" << endl;
         cerr << "error #" << blocks.ErrorId() << " : " << blocks.ErrorDesc() << endl;
         return 1;
    }
    TiXmlHandle hdl(&blocks);
    TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();
    if(!elem){
	          cerr << "le noeud à atteindre n'existe pas" << endl;
	          return 2;
    }
    while (elem){
        int x,y,width,height ;
        elem->Attribute("x",&x) ;
        elem->Attribute("y",&y) ;
        elem->Attribute("width",&width) ;
        elem->Attribute("height",&height) ;
        SDL_Surface* image ;
        //image = load_image( elem->Attribute("image")) ;
        //todo
        //if((image = load_image( "ressource/slipman.bmp"))) {
        image = NULL ;
       // }
        Element sol(x,y,width,height,image) ;
        Elements.push_back(sol);
    	elem = elem->NextSiblingElement(); // iteration
    }
    //FIN chargement des collisions (fichier xml)

    //initialisation des images
    background = load_image( "ressource/level_1/level.bmp" ) ;
    block = load_image( "ressource/block.bmp" ) ;


    //Ajout de l'avatar
    Avatar half(400,330,30,48,load_image( "ressource/half.bmp" ),1) ;
    half.setSpeed(10) ;
    //ajout au tableau
    Avatars.push_back(half);

    //DEBUT chargement des collisions (fichier xml)
    TiXmlDocument ennemies("ressource/level_1/ennemies.xml");
    if(!ennemies.LoadFile()){
         cerr << "erreur lors du chargement ennemie" << endl;
         cerr << "error #" << ennemies.ErrorId() << " : " << ennemies.ErrorDesc() << endl ;
         return 1;
    }
    TiXmlHandle hdl_ennemies(&ennemies);
    TiXmlElement *elem_ennemies = hdl_ennemies.FirstChildElement().FirstChildElement().Element() ;
    if(!elem_ennemies){
	          cerr << "le noeud à atteindre n'existe pas" << endl;
	          return 2;
    }
    while (elem_ennemies){
        int x,y,width,height ;
        elem_ennemies->Attribute("x",&x) ;
        elem_ennemies->Attribute("y",&y) ;
        elem_ennemies->Attribute("width",&width) ;
        elem_ennemies->Attribute("height",&height) ;
        Avatar mechant(x,y,width,height,load_image( "ressource/coin.bmp" ),8) ;
        Ennemies.push_back(mechant);
    	elem_ennemies = elem_ennemies->NextSiblingElement(); // iteration
    }
    //FIN chargement des collisions (fichier xml)



    //démarrage des compteurs
    update.start();
    fps.start();

    //création de l'interface
    //menu.init() ;

    //suppresion du curseur
    SDL_ShowCursor(0) ;

    //lancement du systeme de font

	//Ouverture du Font
	font = TTF_OpenFont( "ressource/font.ttf", 28 );
    font = TTF_OpenFont( "ressource/font.ttf", 28 );
    return true ;
}

bool stop() {

    //vider les tableaux
    Elements.clear() ;
    Ennemies.clear() ;
    Avatars.clear() ;
    Missiles.clear() ;

    //arrêter la musique
    music = NULL ;

    //arrêt des compteurs
    update.stop();
    fps.stop();
    partieEnCours = false ;

}

bool restart() {
    stop() ;
	init() ;
    partieEnCours = true ;
    return true ;
}

//libération des variables
void free() {
	SDL_FreeSurface( screen );
	Mix_FreeMusic( music );
	Mix_CloseAudio();
}


int main( int argc, char *argv[ ] )
{
    //initialisation du menu

    //initialisation du moteur
    init() ;
    menu.init() ;


    //*******************************
    /*     BOUCLE DU PROGRAMME     */
    //*******************************
    int sexY = 0 ;
	while( quit == false ) {

            switch(event.type) {
                              case SDL_QUIT:
                                   quit = true ;
                                   break;
                              default: break;
            }

           if(menu.actif) {
                  menu.aff(screen,partieEnCours) ;
                  while( SDL_PollEvent( &event ) ) {
                         if( keystates[ SDLK_r ] ) {
                              menu.actif = false ;
                          }
                          if( keystates[ SDLK_a ] ) {
                              quit = true ;
                          }
                          if( keystates[ SDLK_n ] ) {
                              restart() ;
							  Gravite.setVelX(0) ;
                              menu.actif = false ;
                          }
                          if( keystates[ SDLK_ESCAPE ] ) {
                              restart() ;
                              quit = true ;
                          }
                  }
           } else {
                   //initialisation de la musique

                    //S'il y a pas de musique en cours
                    if( music == NULL )
                    {
                        music = Mix_LoadMUS( "ressource/level_1/music.wav" );
                        Mix_PlayMusic( music, -1 ) ;
                    }

                   //déplacement caméra
                   if(SCREEN_WIDTH + camera.x >= LEVEL_WIDTH && (Avatars[0].getX() > LEVEL_WIDTH - (SCREEN_WIDTH / 2))) {
                        camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
                   } else if(camera.x <= 0 && (Avatars[0].getX() < (SCREEN_WIDTH / 2))) {
                        camera.x = 0 ;
                   } else {
                        camera.x =  Avatars[0].getX() - (SCREEN_WIDTH / 2)  ;
                   }

                   //récuperation des événements clavier
                   while( SDL_PollEvent( &event ) && !(menu.actif)) {
                          //Si l'utilisateur a cliqué sur le X de la fenêtre


                          if( keystates[ SDLK_ESCAPE ] ) {
                              menu.actif = true ;
                          }

                          if( keystates[ SDLK_h ] ) {
                              for (itEnnemies = Ennemies.begin(); itEnnemies != Ennemies.end(); itEnnemies++)
                                  {
                                        (*itEnnemies).mort = false ;

                                  }
                          }

                          if( keystates[ SDLK_d ] ) {
                              if(!Avatars[0].deplacement_gauche) {
                                 Avatar mechant(Avatars[0].getX(),Avatars[0].getY(),20,22,load_image( "ressource/coin.bmp" ),8) ;
                                 Force force(15,-10) ;
                                 mechant.addForce(force) ;
                                 Ennemies.push_back(mechant);
                              } else {
                                 Avatar mechant(Avatars[0].getX(),Avatars[0].getY(),20,22,load_image( "ressource/coin.bmp" ),8) ;
                                 Force force(-15,-10) ;
                                 mechant.addForce(force) ;
                                 Ennemies.push_back(mechant);
                              }
                          }
                          if( keystates[ SDLK_s ] ) {
                                 Avatar mechant(Avatars[0].getX(),Avatars[0].getY(),30,48,load_image( "ressource/half.bmp" ),2) ;
                                 mechant.deplacement_gauche = false ;
                                 mechant.setSpeed(10) ;
                                 Force force(15,-10) ;
                                 mechant.addForce(force) ;
                                 Ennemies.push_back(mechant);
                          }

                          if( keystates[ SDLK_x ] &&  !Avatars[0].mort) {
                              if(Avatars[0].deplacement_gauche) {
                                     Missile banana((Avatars[0].getX() - 25 ),Avatars[0].getY() + 10, 15,15, load_image( "ressource/banana.bmp" )) ;
                                     Force force(-12,-20) ;
                                     banana.addForce(force) ;
                                     Missiles.push_back(banana);

                              } else {
                                     Missile banana(Avatars[0].getX() + 40,Avatars[0].getY() + 10, 15,15, load_image( "ressource/banana.bmp" )) ;
                                     Force force(12,-20) ;
                                     banana.addForce(force) ;
                                     Missiles.push_back(banana);
                              }

                          }
                          if( keystates[ SDLK_f ] ) {
                              if(windowed) {
                                     screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN );
                                     windowed = false ;
                              } else {
                                     screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,  SDL_SWSURFACE | SDL_RESIZABLE );
                                     windowed = true ;
                              }
                          }
                          if( keystates[ SDLK_g ] ) {
                              Gravite.setVelX(4) ;
                          }
                          if( keystates[ SDLK_p ] )
                          {
                                //Si la musique est en pause
                                if( Mix_PausedMusic() == 1 )
                                {
                                    //On enlève la pause (la musique repart ou elle en était)
                                    Mix_ResumeMusic();
                                }
                                //Si la musique est en train de jouer
                                else
                                {
                                    //On met en pause la musique
                                    Mix_PauseMusic();
                                }
                          }
                  }

                  //Ouverture du Font
                  font = TTF_OpenFont( "ressource/font.ttf", 28 );
                  apply_surface( 0, 0, background, screen, &camera );
                  touche_p = TTF_RenderText_Solid( font, "echap : Menu", textColor );
                  apply_surface( 10, 520, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "P : Demarrer/Arreter la musique", textColor );
                  apply_surface( 10, 550, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "Flêches : Se deplacer", textColor );
                  apply_surface( 10, 580, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "espace : Sauter", textColor );
                  apply_surface( 10, 610, touche_p, screen );
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "D : Lancer un poussin", textColor );
                  apply_surface( 10, 640, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "x : Lancer une banane qui tue les poussins", textColor );
                  apply_surface( 10, 670, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "S : Se cloner", textColor );
                  apply_surface( 10, 700, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "H : Sort de resurection des ennemies", textColor );
                  apply_surface( 10, 730, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  touche_p = TTF_RenderText_Solid( font, "G : Petite bourrasque", textColor );
                  apply_surface( 10, 760, touche_p, screen);
                  SDL_FreeSurface(touche_p);
                  TTF_CloseFont(font);

                  Uint8 *keystates = SDL_GetKeyState( NULL );
                  //traitements des avatars

                  for (itAvatars = Avatars.begin(); itAvatars != Avatars.end(); itAvatars++)
                  {
                        if((*itAvatars).mort) {
                                continue;
                        }
                        //calcul de l'accélération
                        if((*itAvatars).getVelX() > 0) {
                               (*itAvatars).acceleration = (*itAvatars).getVelX() ;
                        } else {
                               (*itAvatars).acceleration = 0 - (*itAvatars).getVelX() ;
                        }
                        //calcul de la vitesse d'animation
                        (*itAvatars).cycleWalk =  60 - (2.5 * (*itAvatars).acceleration) ;
                        //Récupération du slide des avatars
                        (*itAvatars).setSlide(keystates) ;
                        //application des forces
                        (*itAvatars).addForce(Gravite) ;
                        //réaction des claviers
                        (*itAvatars).handle_input(keystates) ;
                        //déplacement des éléments
                        (*itAvatars).move(Elements,Missiles,BadMissiles,keystates) ;
                        //dessinage des images
                        apply_surface( (*itAvatars).getX() - camera.x,(*itAvatars).getY() - camera.y, (*itAvatars).getTronche(), screen, &(*itAvatars).walk[(*itAvatars).numSlide] );
                  }
                    //traitements des avatars

                  for (itEnnemies = Ennemies.begin(); itEnnemies != Ennemies.end(); itEnnemies++)
                  {
                        if((*itEnnemies).mort) {
                                continue;
                        }
                        //calcul de l'accélération
                        if((*itEnnemies).getVelX() > 0) {
                               (*itEnnemies).acceleration = (*itEnnemies).getVelX() ;
                        } else {
                               (*itEnnemies).acceleration = 0 - (*itEnnemies).getVelX() ;
                        }
                        //Récuperation du slide des avatars
                        (*itEnnemies).setSlide(keystates) ;
                        //application des forces
                        (*itEnnemies).addForce(Gravite) ;
                        //déplacement des éléments
                        (*itEnnemies).autoMove(Elements,Missiles) ;
                        //dessin des images
                        apply_surface( (*itEnnemies).getX() - camera.x,(*itEnnemies).getY() - camera.y, (*itEnnemies).getTronche(), screen, &(*itEnnemies).walk[(*itEnnemies).numSlide] );
                  }

                  //traitements des éléments
                  for ( int i = 0, size = Elements.size(); i < size; ++i )
                  {
                      //apply_surface( Elements[i].getX() - camera.x, Elements[i].getY() - camera.y, Elements[i].getTronche(), screen);
                  }

                  //traitements des missiles

                  for (itMissiles = Missiles.begin(); itMissiles != Missiles.end(); itMissiles++)
                  {
                      if((*itMissiles).destroy || (*itMissiles).getAuSol()) {
                                continue;
                      }
                      (*itMissiles).setSlide() ;
                      (*itMissiles).addForce(Gravite) ;
                      (*itMissiles).move(Elements,Missiles) ;
                      apply_surface( ((*itMissiles).getX() - camera.x),((*itMissiles).getY() - camera.y), (*itMissiles).getTronche(), screen, &(*itMissiles).walk[(*itMissiles).numSlide]);
                  }
          }

          //calcul du FPS
          if( update.get_ticks() > 1 )
          {
                char caption[ 64 ];
                sprintf( caption, "CPC le jeu : %.0f FPS acceleration:%d",
                         (float)frame / ( fps.get_ticks() / 1000.f ),Avatars[0].acceleration);
                SDL_WM_SetCaption( caption, NULL );
                update.start();
          }
          //après avoir dessiné dans le dos de l'écran on le retourne
          if( SDL_Flip( screen ) == -1 ) {
		      return EXIT_FAILURE;
          }

          //attente passive pour le framerate
          SDL_framerateDelay(&fps_manager);
          frame++;
          if(Avatars[0].mort) {
             menu.actif = true ;
          }
	}

    free() ;
	SDL_Quit();

	return EXIT_SUCCESS;

}
