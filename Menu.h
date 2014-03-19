class Menu
{
      public:

          //Actif ?
          bool actif ;

          //Surfaces pour le menu
          SDL_Surface *images_fond,*images_nouveau, *images_quitter, *images_titre, *images_restart, *images_retour, *images_perso ;

          //Position et taille des boutons nouveau et quitter et du titre
          SDL_Rect titre ;
          SDL_Rect nouveau ;
          SDL_Rect quitter ;
          SDL_Rect restart ;
          SDL_Rect retour ;
          SDL_Rect perso ;

          //Constructeur
          Menu();

          //Destructeur
          ~Menu();

   	      //Initialisation
          bool init();

          //Gestion du clic
          void clic(int x, int y);

          //Fonction d'affichage
          void aff(SDL_Surface *screen,bool partieEnCours);

};
