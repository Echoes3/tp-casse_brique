#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_texture;
     SDL_Surface *g_surface;


}game;

typedef struct gameState{

    int g_bRunning;
    int left;
    int right;


}gameState;


typedef struct coordonnees{

    double x;
    double y;
     int left;
    int right;
    int state;




}coordonnees;

typedef struct balle{

   coordonnees position;
    double vitesse;
    double hauteur;
    double largeur;
    int haut_bas;
    int gauche_droite;

}balle;

typedef struct brique{

    coordonnees position;
    double hauteur;
    double largeur;
    int destroy;

}brique;

enum collision{

    haut,bas,gauche,droite
};






int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *raquette, coordonnees *balle);
void blocraquette(coordonnees *dep);
void cutBitmapTexture(game *myGame,gameState state,coordonnees *dep,coordonnees *mvt);
void delay(unsigned int frameLimit);
void destroyTexture(game *myGame);
void destroy(game *myGame);
void mvtballe (game *myGame,gameState state,coordonnees *mvt,coordonnees *balle, coordonnees *dep);
void RemplissageTab (game *myGame,gameState state,int argc, char *argv[],brique *position);
void InitBalle(balle *Balle);
void AfficheBalle(game *myGame,gameState state,balle *Balle);
void CollisionBalle(balle *Balle, coordonnees *dep);
void DeplacementBalle(balle *Balle);
void InitBriques(brique Briques[5][5]);
void AfficheBriques(game *myGame,gameState state, brique Briques[5][5]);




#endif // HEADER_H_INCLUDED
