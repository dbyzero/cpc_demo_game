#include <SDL/SDL.h>
#include "Timer.h"
#include "Element.h"
#include "const.h"
#include "Force.h"
#include <vector>
#include <math.h>
#include <string>

//FORCE
Force::Force(int x, int y)
{
	velX = x;
	velY = y;
}
int Force::getVelX() {
	return velX ;
}
int Force::getVelY() {
	return velY ;
}
void Force::setVelX(int x) {
	velX = x ;
}
void Force::setVelY(int y) {
	velY = y ;
}

//ELEMENT
Element::Element(int setX, int setY, int setWidth, int setHeight, SDL_Surface* setTronche)
{
	//Initialisation des coordonnées
	x = setX;
	y = setY;

	velX = 0 ;
	velY = 0 ;

	speed = 10 ;

	auSol = false ;
	surPlateforme = false ;

	tronche = setTronche ;

	//Initialisation de la vitesse
	width = setWidth ;
	height = setHeight ;
}

void Element::handle_input(Uint8* keystates)
{
	if( keystates[ SDLK_UP ] ) {
		//velY -= height / 8;
		velY = 0 - speed ;
	}
	if( keystates[ SDLK_DOWN ] ) {
		//velY += height / 8;
		velY = speed ;
	}
	if( keystates[ SDLK_LEFT ] ) {
		//velX -= width / 8;
		velX = 0 - speed ;
	}
	if( keystates[ SDLK_RIGHT ] ) {
		//velX += width / 8;
		velX = speed ;
	}
//	if( keystates[ SDLK_o ] ) {
		//velX += width / 8;
//		y = 0 ;
//	}
	if( keystates[ SDLK_SPACE ] && (auSol || surPlateforme) ) {
		//y = y - 1  ;
		velY = -25 ;
	}
	if(keystates[ SDLK_LEFT ] == false && keystates[ SDLK_RIGHT ] == false && auSol) {
		velX = 0;
	}
	if(keystates[ SDLK_UP ] == false && keystates[ SDLK_DOWN ] == false) {
		//velY = 0;
	}

}

bool Element::checkCollision(Element &A) {

	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calcule les côtés du rectangle A
	leftA = A.x;
	rightA = A.x + A.width;
	topA = A.y;
	bottomA = A.y + A.height;

	//Calcule les côtés du rectangle B
	leftB = x;
	rightB = x + width;
	topB = y;
	bottomB = y + height;
	//Tests de collisions
	if( bottomA <= topB )
	{
		return false;
	}

	if( topA >= bottomB )
	{
		return false;
	}

	if( rightA <= leftB )
	{
		return false;
	}

	if( leftA >= rightB )
	{
		return false;
	}

	//Si des collisions ont été détectées
	return true;
}

bool Element::getAuSol() {
	return auSol ;
}

bool Element::getSurPlateforme() {
	return surPlateforme ;
}

int Element::getX() {
	return x ;
}

int Element::getY() {
	return y ;
}

int Element::getWidth() {
	return width ;
}

int Element::getHeight() {
	return height ;
}

int Element::getVelX() {
	return velX ;
}

int Element::getVelY() {
	return velY ;
}

int Element::getSpeed() {
	return speed ;
}

void Element::setSpeed(int Speed) {
	speed = Speed ;
}

void Element::move() {
	x += velX;
	y += velY;
	if(x < 0)
	{
		x = 0 ;
		//x = SCREEN_WIDTH - width ;
	}
	if(y < 0)
	{
		//y = SCREEN_HEIGHT - height ;
		y = 0 ;
	}
	if(x >= (SCREEN_WIDTH - width))
	{
		x = SCREEN_WIDTH - width ;
	}
	//au sol
	if(y >= (SCREEN_HEIGHT - height))
	{
		y = SCREEN_HEIGHT - height ;
		velY = 0 ;
		auSol = true ;
	} else {
		auSol = false ;
	}
}


SDL_Surface* Element::getTronche() {
	return tronche ;
}

void Element::setTronche(SDL_Surface* setTronche) {
	tronche = setTronche ;
}

void Element::addForce(Force force) {

	velX += force.getVelX();
	velY += force.getVelY();
}

//AVATAR
Avatar::Avatar(int setX, int setY, int setWidth, int setHeight,
                   SDL_Surface* setTronche, int setTypeAnim) :
                   Element(setX, setY, setWidth, setHeight, setTronche) {
    numSlide = 0 ;
    deplacement_gauche = false ;
    mort = false ;
    statusAnim = 0 ;
    acceleration = 0 ;
    typeAnim = setTypeAnim ;
        switch(typeAnim) {
            case 6 :
                for(int i = 0;i < 6;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 85 * i;
                      sprite.y = 0 ;
                      sprite.w = 85 ;
                      sprite.h = 50 ;
                      walk.push_back(sprite) ;
                }
                for(int i = 0;i < 6;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 85 * i ;
                      sprite.y = 55 ;
                      sprite.w = 85 ;
                      sprite.h = 50 ;
                      walk.push_back(sprite) ;
                }
                for(int i = 0;i < 6;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 85 * i ;
                      sprite.y = 110 ;
                      sprite.w = 85 ;
                      sprite.h = 50 ;
                      walk.push_back(sprite) ;
                }
                for(int i = 0;i < 6;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 85 * i ;
                      sprite.y = 165 ;
                      sprite.w = 85 ;
                      sprite.h = 50 ;
                      walk.push_back(sprite) ;
                }
                for(int i = 0;i < 4;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 85 * i ;
                      sprite.y = 220 ;
                      sprite.w = 85 ;
                      sprite.h = 60 ;
                      walk.push_back(sprite) ;
                }
                break ;
           case 8 :
                for(int i = 0;i < 8;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 20 * i ;
                      sprite.y = 0 ;
                      sprite.w = 20 ;
                      sprite.h = 22 ;
                      walk.push_back(sprite) ;
                }
                for(int i = 0;i < 8;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 20 * i ;
                      sprite.y = 22 ;
                      sprite.w = 20 ;
                      sprite.h = 22 ;
                      walk.push_back(sprite) ;
                }
           break ;
           case 4 :
                    for(int i = 0;i < 4;++i) {
                          SDL_Rect sprite  ;
                          sprite.x = 38 * i + 1;
                          sprite.y = 130 ;
                          sprite.w = 31 ;
                          sprite.h = 30 ;
                          walk.push_back(sprite) ;
                    }
            break ;
            case 2 :

                //anim marche gauche
                for(int i = 0;i < 4;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 30 * i ;
                      sprite.y = 49 ;
                      sprite.w = 30 ;
                      sprite.h = 48 ;
                      walk.push_back(sprite) ;
                }
                //anime marche droite
                for(int i = 0;i < 4;++i) {
                      SDL_Rect sprite  ;
                      sprite.x = 30 * i ;
                      sprite.y = 0 ;
                      sprite.w = 30 ;
                      sprite.h = 48 ;
                      walk.push_back(sprite) ;
                }
            break ;
            case 1 :
            //anim marche droite
            for(int i = 0;i < 4;++i) {
                  SDL_Rect sprite  ;
                  sprite.x = 30 * i ;
                  sprite.y = 0 ;
                  sprite.w = 30 ;
                  sprite.h = 48 ;
                  walk.push_back(sprite) ;
            }
            //anim marche gauche
            for(int i = 0;i < 4;++i) {
                  SDL_Rect sprite  ;
                  sprite.x = 30 * i ;
                  sprite.y = 49 ;
                  sprite.w = 30 ;
                  sprite.h = 48 ;
                  walk.push_back(sprite) ;
            }
            //anim marche gauche
            for(int i = 0;i < 4;++i) {
                  SDL_Rect sprite  ;
                  sprite.x = 30 * i ;
                  sprite.y = 98 ;
                  sprite.w = 30 ;
                  sprite.h = 48 ;
                  walk.push_back(sprite) ;

            }
            //anim marche gauche
            for(int i = 0;i < 4;++i) {
                  SDL_Rect sprite  ;
                  sprite.x = 30 * i ;
                  sprite.y = 147 ;
                  sprite.w = 30 ;
                  sprite.h = 48 ;
                  walk.push_back(sprite) ;
           }
           //en l'air
           for(int i = 0;i < 4;++i) {
                  SDL_Rect sprite  ;
                  sprite.x = 30 * i ;
                  sprite.y = 196 ;
                  sprite.w = 30 ;
                  sprite.h = 48 ;
                  walk.push_back(sprite) ;
           }
           break ;
    }
    cycleWalk = 35 ;
    compteurAnim.start() ;
}

void Avatar::handle_input(Uint8* keystates)
{
/*	if( keystates[ SDLK_UP ] ) {
		//velY -= height / 8;
		velY = 0 - speed ;
	}*/
	if( keystates[ SDLK_DOWN ] ) {
		//velY += height / 8;
		velY = speed ;
	}
	if( keystates[ SDLK_LEFT ] ) {
		//velX -= width / 8;
		if(velX <= (0 - speed)) {
             velX = 0 - speed ;
        } else {
             velX = velX - (speed/10) ;
        }
	}
	if( keystates[ SDLK_RIGHT ] ) {
		if(velX >= (speed)) {
             velX = speed ;
        } else {
             velX = velX + (speed/10) ;
        }
	}
	//if( keystates[ SDLK_o ] ) {
		//velX += width / 8;
	//	y = 0 ;
	//}
	if( keystates[ SDLK_SPACE ] && (auSol || surPlateforme) ) {
		//y = y - 1  ;
		velY = -25 ;
	}
	if(keystates[ SDLK_LEFT ] == false && keystates[ SDLK_RIGHT ] == false && auSol) {
		velX = 0;
	}
}

void Avatar::move(vector<Element> &Elements,vector<Missile> &Missiles,vector<Missile> &badMissiles, Uint8* keystates ) {
	bool libre = true ;
	x += velX ;
	if(surPlateforme && !keystates[ SDLK_LEFT ] && !keystates[ SDLK_RIGHT ]) {
         if(velX > (0 - speed/5) && (velX < speed/5)) {
              velX = 0 ;
         }
         if(velX < 0 ) {
              velX += speed/5 ;
         }
         if(velX > 0) {
              velX -= speed/5 ;
         }
    }
	int i,size ;
	for(i = 0, size = Elements.size(); i < size; ++i) {
		if(checkCollision(Elements[i])) {
            velX = 0 ;
			// collision droite
			if(x > Elements[i].getX()) {
               // velX = 0 ;
				x = Elements[i].getX() + Elements[i].getWidth();
			// collision gauche
			} else {
                x = Elements[i].getX() - width;
			}
		}
	}
	for(i = 0, size = Missiles.size(); i < size; ++i) {
		if(checkCollision(Missiles[i])) {
            velX = 0 ;
			// collision droite
			if(x > Missiles[i].getX()) {
               // velX = 0 ;
				x = Missiles[i].getX() + Missiles[i].getWidth();
			// collision gauche
			} else {
                x = Missiles[i].getX() - width;
			}
		}
	}
	y += velY ;
    surPlateforme = false ;
	for(i = 0, size = Elements.size(); i < size; ++i) {
        if(checkCollision(Elements[i])) {
            velY = 0 ;
			//collision haut
			if(y < Elements[i].getY()){
				y = Elements[i].getY() - height ;
				surPlateforme = true ;
			//collision bas
            } else {
				y = Elements[i].getY() + Elements[i].getHeight();
	     	}
		}
	}
	for(i = 0, size = Missiles.size(); i < size; ++i) {
        if(checkCollision(Missiles[i])) {
            velY = 0 ;
			//collision haut
			if(y < Missiles[i].getY()){
				y = Missiles[i].getY() - height ;
				surPlateforme = true ;
			//collision bas
            } else {
				y = Missiles[i].getY() + Missiles[i].getHeight();
	     	}
		}
	}
	if(x < 0)
	{
		x = 0 ;
		//velX = 0 ;
	}
	if(y < 0)
	{
		y = 0 ;
		velY = 0 ;
	}
	if(x >= (LEVEL_WIDTH - width))
	{
		x = LEVEL_WIDTH - width ;
	}
	if((y >= (LEVEL_HEIGHT - height)))
	{
		y = LEVEL_HEIGHT - height ;
		velY = 0 ;
		mort = true ;
	}

	//COLLISIONS AVEC LES MISSILES
	for(i = 0, size = badMissiles.size(); i < size; ++i) {
		if(checkCollision(badMissiles[i])) {
			// collision droite
			if(x  > badMissiles[i].getX() + badMissiles[i].getWidth()) {
				mort = true ;
			}
			// collision gauche
			if(x + width < badMissiles[i].getX()) {
				mort = true ;
			}
			//collision haut
			if(y < badMissiles[i].getY()){
				mort = true ;
			}
			// collision bas
			if(y + height > badMissiles[i].getY() + badMissiles[i].getHeight()) {
				mort = true ;
			}
		}
	}
}

void Avatar::autoMove(vector <Element> &Elements,vector<Missile> &Missiles) {
    if(velX == 0) {
         velX = deplacement_gauche ? -5 : 5 ;
         deplacement_gauche = !deplacement_gauche ;
    }

    int i,size ;
    y += velY ;
    surPlateforme = false ;
	for(i = 0, size = Elements.size(); i < size; ++i) {
        while(checkCollision(Elements[i])) {
            //velY = 0 ;

			//collision haut
			if(velY >= 0){
                y = Elements[i].getY() - height ;

                //y = Elements[i].getY() - height ;
			//collision bas
            } else {
				y = Elements[i].getY() + Elements[i].getHeight();
				//y  = 0 ;
				surPlateforme = true ;
	     	}
	     	velY = 0 - velY ;
		}

	}
    x += velX;
	for(i = 0, size = Elements.size(); i < size; ++i) {
		while(checkCollision(Elements[i])) {
            velX = 0 ;
			// collision droite
			if(x > Elements[i].getX()) {
               // velX = 0 ;
				x = Elements[i].getX() + Elements[i].getWidth();
			// collision gauche
			} else {
                x = Elements[i].getX() - width;
			}
		}
	}

	if(x < 0)
	{
		x = 0 ;
		//velX = 0 ;
	}
	if(y < 0)
	{
		y = 0 ;
		velY = 0 ;
	}
	if(x >= (LEVEL_WIDTH - width))
	{
		x = LEVEL_WIDTH - width ;
	}
	if((y >= (LEVEL_HEIGHT - height)))
	{
		y = LEVEL_HEIGHT - height ;
		velY = 0 ;
		mort = true ;
	}

	//COLLISIONS AVEC LES MISSILES
	for(i = 0, size = Missiles.size(); i < size; ++i) {
		if(checkCollision(Missiles[i])) {
			// collision droite
			if(x  > Missiles[i].getX() + Missiles[i].getWidth()) {
				mort = true ;
			}
			// collision gauche
			if(x + width < Missiles[i].getX()) {
				mort = true ;
			}
			//collision haut
			if(y < Missiles[i].getY()){
				mort = true ;
			}
			// collision bas
			if(y + height > Missiles[i].getY() + Missiles[i].getHeight()) {
				mort = true ;
			}
		}
	}
}

//changement de sprite
void Avatar::setSlide(Uint8* keystates) {
     if( compteurAnim.get_ticks() > cycleWalk ) {
         switch(typeAnim) {
          case 8 :
              if(deplacement_gauche) {
                      statusAnim = 1 ;
                      if (numSlide >= 7) {
                            numSlide = 0 ;
                      } else {
                            numSlide++  ;
                      }
                } else {
                      statusAnim = 2 ;
                      if (numSlide >= 15 || numSlide < 8 ) {
                            numSlide = 8 ;
                      } else {
                            numSlide++ ;
                      }
                }
                compteurAnim.start();
                break ;

          case 2 :
               if(deplacement_gauche) {
                      statusAnim = 2 ;
                      if (numSlide >= 7 || numSlide < 4 ) {
                            numSlide = 4 ;
                      } else {
                            numSlide++ ;
                      }
                } else {
                      statusAnim = 1 ;
                      if (numSlide >= 3) {
                            numSlide = 0 ;
                      } else {
                            numSlide++  ;
                      }
                }
                compteurAnim.start();
                break ;

          case 1 :
              //va a droite
              if (keystates[ SDLK_RIGHT ] && getSurPlateforme()) {
                  statusAnim = 1 ;
                  if (numSlide >= 3) {
                      numSlide = 0 ;
                  } else {
                      numSlide++  ;
                  }
                  deplacement_gauche = false ;
              }

              //va a gauche
              if (keystates[ SDLK_LEFT ] && getSurPlateforme()) {
                  statusAnim = 2 ;
                  if (numSlide >= 7 || numSlide < 4 ) {
                      numSlide = 4 ;
                  } else {
                      numSlide++ ;
                  }
                  deplacement_gauche = true ;
              }

              //sur place
              if (!keystates[ SDLK_LEFT ] && !keystates[ SDLK_RIGHT ] && getSurPlateforme()) {
                  if(statusAnim == 2 || statusAnim == 4 || statusAnim == 6) {
                      statusAnim = 4 ;
                      if (numSlide >= 15 || numSlide < 12 ) {
                          numSlide = 12 ;
                      } else {
                          numSlide++ ;
                      }
                      deplacement_gauche = true ;
                  } else {
                      statusAnim = 3 ;
                      if (numSlide >= 11 || numSlide < 8 ) {
                          numSlide = 8 ;
                      } else {
                          numSlide++ ;
                      }
                      deplacement_gauche = false ;
                  }
              }
              //en l'air
              if (!getSurPlateforme()) {
                      if(keystates[ SDLK_RIGHT ]) {
                               statusAnim = 5 ;
                               deplacement_gauche = false ;
                      }
                      if(keystates[ SDLK_LEFT ]) {
                               statusAnim = 6 ;
                               deplacement_gauche = true ;
                      }
                      if (statusAnim == 1 || statusAnim == 3 || statusAnim == 5) {
                              statusAnim = 5 ;
                              if(numSlide == 16) {
                                  numSlide = 17 ;
                              } else {
                                  numSlide = 16 ;
                              }
                              deplacement_gauche = false ;
                      }
                      if (statusAnim == 2 || statusAnim == 4 || statusAnim == 6 ) {
                              statusAnim = 6 ;
                              if(numSlide == 18) {
                                  numSlide = 19 ;
                              } else {
                                  numSlide = 18 ;
                              }
                              deplacement_gauche = true ;
                      }
              }

              compteurAnim.start();
              break ;
        }
    }
}

//MISSILE

Missile::Missile(int setX, int setY, int setWidth, int setHeight,
                   SDL_Surface* setTronche) :
                   Element(setX, setY, setWidth, setHeight, setTronche) {
     destroy = false ;
     numSlide = 0 ;
      for(int i = 0;i < 8;++i) {
          SDL_Rect sprite  ;
          sprite.x = 20 * i;
          sprite.y = 0 ;
          sprite.w = 20 ;
          sprite.h = 520 ;
          walk.push_back(sprite) ;
     }
     compteurAnim.start();
}

void Missile::setVelX(int givedVelX) {
     velX = givedVelX ;
}

void Missile::setSlide() {
     int valAbsoluteVelx = velX > 0 ? velX : 0 - velX ;
     cycleWalk = (180/pow(valAbsoluteVelx,1.1)) + 20 ;
     if( compteurAnim.get_ticks() > cycleWalk ) {
              if (numSlide >= 7) {
                  numSlide = 0 ;
              } else {
                  numSlide++  ;
              }
             // cycleWalk =
              compteurAnim.start();
     }

}

void Missile::move(vector<Element> &Elements,vector<Missile> &Missiles) {

	if(y < 0)
	{
		y = 0 ;
	}
	//au sol
	if(y >= (SCREEN_HEIGHT - height))
	{
		y = SCREEN_HEIGHT - height ;
		velY = 0 ;
		auSol = true ;
	} else {
		auSol = false ;
	}

	int i,size ;

    y += velY ;
    surPlateforme = false ;
	for(i = 0, size = Elements.size(); i < size; ++i) {
        while(checkCollision(Elements[i])) {
            //velY = 0 ;

			//collision haut
			if(velY >= 0){
                y = Elements[i].getY() - height ;

                //y = Elements[i].getY() - height ;
			//collision bas
            } else {
				y = Elements[i].getY() + Elements[i].getHeight();
				//y  = 0 ;
				surPlateforme = true ;
	     	}
	     	velY = 0 - velY/2 ;
            velX = velX/2 ;
		}

	}
    x += velX;
	for(i = 0, size = Elements.size(); i < size; ++i) {
		while(checkCollision(Elements[i])) {
            velX = 0 ;
			// collision droite
			if(x > Elements[i].getX()) {
               // velX = 0 ;
				x = Elements[i].getX() + Elements[i].getWidth();
			// collision gauche
			} else {
                x = Elements[i].getX() - width;
			}
		}
	}


}

void Missile::addForce(Force force) {

	velX += force.getVelX();
	velY += force.getVelY();
}
