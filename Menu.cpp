#include <SDL/SDL.h>
#include <string>
#include <tinyxml.h>
#include <iostream>
#include <tinyxml.h>
#include <stdio.h>
#include "Menu.h"

//Constructeur
Menu::Menu() {

    actif = true ;
}

Menu::~Menu()
{
    SDL_FreeSurface(images_fond) , images_fond = NULL;
    SDL_FreeSurface(images_titre) , images_titre = NULL;
    SDL_FreeSurface(images_quitter) , images_quitter = NULL;
    SDL_FreeSurface(images_restart) , images_restart = NULL;
}

//Gestion du clic
void Menu::clic(int x, int y)
{

}

bool Menu::init() {


    TiXmlDocument info_menu("ressource/menu/menu.xml");
    if(!info_menu.LoadFile()){
        return false;
    }
    TiXmlHandle hdl(&info_menu);
    TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();
    if(!elem){
	     return false;
    }/*

        int x,y,width,height ;

        elem->Attribute("width",&width) ;
        elem->Attribute("height",&height) ;
        SDL_Surface* image ;
        */

    //variable de création
    std::string nom ;
    int x,y,width,height ;

    //On charge l'image de fond
    nom = elem->Attribute("image");
    images_fond = SDL_LoadBMP(nom.c_str()) ;
	elem = elem->NextSiblingElement() ;

    //On récupère la surface de l'image "Titre"
    nom = elem->Attribute("image");
    images_titre = SDL_LoadBMP(nom.c_str()) ;
	elem->QueryIntAttribute("x",&x) ;
	elem->QueryIntAttribute("y",&y) ;
	 //Initialisation des positions du menu
    titre.x = x ;
	titre.y = y ;
	elem = elem->NextSiblingElement() ;

    //On récupère la surface de l'image "Nouveau"
    nom = elem->Attribute("image");
    images_nouveau = SDL_LoadBMP(nom.c_str()) ;
	elem->QueryIntAttribute("x",&x) ;
	elem->QueryIntAttribute("y",&y) ;
    //Initialisation des positions du menu
    nouveau.x = x ;
	nouveau.y = y ;
	elem = elem->NextSiblingElement() ;

    //On récupère la surface de l'image "Quitter"
    nom = elem->Attribute("image");
    images_quitter = SDL_LoadBMP(nom.c_str()) ;
	elem->QueryIntAttribute("x",&x) ;
	elem->QueryIntAttribute("y",&y) ;
    //Initialisation des positions du menu
    quitter.x = x ;
	quitter.y = y ;
	elem = elem->NextSiblingElement() ;

	//On récupère la surface de l'image "Retour"
    nom = elem->Attribute("image");
    images_retour = SDL_LoadBMP(nom.c_str()) ;
	elem->QueryIntAttribute("x",&x) ;
	elem->QueryIntAttribute("y",&y) ;
    //Initialisation des positions du menu
    retour.x = x ;
	retour.y = y ;
	elem = elem->NextSiblingElement() ;

	//On récupère la surface de l'image "Perso"
    nom = elem->Attribute("image");
    images_perso = SDL_LoadBMP(nom.c_str()) ;
	elem->QueryIntAttribute("x",&x) ;
	elem->QueryIntAttribute("y",&y) ;
    //Initialisation des positions du menu
    perso.x = x ;
	perso.y = y ;
	elem = elem->NextSiblingElement() ;

	//On récupère la surface de l'image "Restart"
    nom = elem->Attribute("image");
    images_restart = SDL_LoadBMP(nom.c_str()) ;
	elem->QueryIntAttribute("x",&x) ;
	elem->QueryIntAttribute("y",&y) ;
    //Initialisation des positions du menu
    restart.x = x ;
	restart.y = y ;
	elem = elem->NextSiblingElement() ;

    //Mise en place de la transparence
    images_nouveau = SDL_DisplayFormat( images_nouveau );
    Uint32 colorkey = SDL_MapRGB( images_nouveau->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_nouveau,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
    images_quitter = SDL_DisplayFormat( images_quitter );
    colorkey = SDL_MapRGB( images_titre->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_titre,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
    images_titre = SDL_DisplayFormat( images_titre );
    colorkey = SDL_MapRGB( images_quitter->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_quitter,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
    images_fond = SDL_DisplayFormat( images_fond );
    colorkey = SDL_MapRGB( images_fond->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_fond,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
    images_retour = SDL_DisplayFormat( images_retour );
    colorkey = SDL_MapRGB( images_retour->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_retour,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
    images_restart = SDL_DisplayFormat( images_restart );
    colorkey = SDL_MapRGB( images_restart->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_restart,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
    images_perso = SDL_DisplayFormat( images_perso );
    colorkey = SDL_MapRGB( images_perso->format, 0xFF, 0, 0xFF );
    SDL_SetColorKey(images_perso,SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );



    //On récupère la taille des images
    titre.w = images_titre->w;
    titre.h = images_titre->h;

    nouveau.w = images_nouveau->w;
    nouveau.h = images_nouveau->h;

    quitter.w = images_quitter->w;
    quitter.h = images_quitter->h;

    //On retourne vrai*/
    return true;
}

//Fonction d'affichage
void Menu::aff(SDL_Surface *screen, bool partieEnCours)
{
        SDL_BlitSurface(images_fond,NULL,screen,NULL);
        SDL_BlitSurface(images_titre,NULL,screen,&titre);

        if(partieEnCours) {
            SDL_BlitSurface(images_retour,NULL,screen,&retour);
            SDL_BlitSurface(images_nouveau,NULL,screen,&restart);
        } else {
            SDL_BlitSurface(images_nouveau,NULL,screen,&nouveau);
        }
        SDL_BlitSurface(images_perso,NULL,screen,&perso);
        SDL_BlitSurface(images_quitter,NULL,screen,&quitter);

}
