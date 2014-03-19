#include <vector>

using namespace std ;

//FORCE
class Force
{
	private :
		int  velX, velY ;
	public:
		Force(int,int);
		int getVelX();
		int getVelY();
		void setVelX(int velX) ;
		void setVelY(int velY) ;
};

//ELEMENT
class Element
{
	protected :
		int x, y, width, height, velX, velY, speed;
		bool auSol ;
		bool surPlateforme ;

		SDL_Surface* tronche ;
	public:
		Element(int x ,int y,int w ,int h, SDL_Surface* tronche);
		void handle_input(Uint8* keystates) ;
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		int getVelX();
		int getVelY();
		int getSpeed();
		bool getAuSol();
		bool getSurPlateforme();
		void setSpeed(int Speed) ;
		void move() ;
		void setTronche(SDL_Surface* tronche) ;
		bool checkCollision(Element &A) ;
		SDL_Surface* getTronche() ;
		void addForce(Force force) ;
};

//Missile
class Missile : public Element {             
	public :
		Missile(int x , int y, int width, int height, SDL_Surface* tronche);
		void move(vector<Element> &Elements,vector<Missile> &Missiles) ;
		void setVelX(int givedVelX) ;
		void setSlide() ;
		void Missile::addForce(Force force) ;
		
		bool destroy ;
		double cycleWalk ;
		int numSlide ;
		Timer compteurAnim ;
		vector<SDL_Rect> walk ;
		
};

//AVATAR
class Avatar : public Element {             
	public :
		Avatar(int x , int y, int width, int height, SDL_Surface* tronche, int typeAnim);
		void move(vector <Element> &Decord,vector<Missile> &Missiles,vector<Missile> &badMissiles,Uint8* keystates) ;
		void setSlide(Uint8* keystates) ;
		void handle_input(Uint8* keystates) ;
		void autoMove(vector <Element> &Decord,vector<Missile> &Missiles) ;
		double cycleWalk ;
		int numSlide ;
		int typeAnim ;
		int statusAnim ;
		int acceleration ;
		bool deplacement_gauche ;
		
		bool mort ;
		
		Timer compteurAnim ;
		vector<SDL_Rect> walk ;
};

