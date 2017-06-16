#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "header.h"

#define SPEED 3
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480


//Option de build dans les linker setting mettre les libSDL2main.a libSDL2.dll.a
//Dans search directory
//Option de build compiler mettre les include
//Option de build linker mettre les lib

//mettre dans build linker -lmingw32




int main(int argc, char *argv[])
{

     game myGame;
     gameState state;



    //Pour les 60 fps
    unsigned int frameLimit = SDL_GetTicks() + 16;
    coordonnees dep;
     dep.x=290;
    int raquette;
    balle Balle;
     brique Briques[6][5];
       coordonnees balle;
     coordonnees mvt;
     coordonnees position;
     mvt.x=1;
     mvt.y=1;
     int tableau[64];

      balle.x=SCREEN_WIDTH/2;

     balle.y=SCREEN_HEIGHT/2;


    if(init("Casse brique",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame)){

                state.g_bRunning=1;


    }else{

            return 1;//something's wrong

    }


    state.right=1;
    state.left=0;
    InitBalle(&Balle);
    InitBriques(Briques);

    while(state.g_bRunning){

           handleEvents(&state,&dep,&raquette,&balle);
           blocraquette(&dep);
           cutBitmapTexture(&myGame,state,&dep,&mvt);
            CollisionBalle(&Balle,&dep);
           DeplacementBalle(&Balle);
           AfficheBalle(&myGame,state,&Balle);
           AfficheBriques(&myGame,state,Briques);

        // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;


    }

        destroy(&myGame);

        SDL_Quit();




  return 0;
}


int init(char *title, int xpos,int ypos,int height, int width,int flags,game *myGame){


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,height,width,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_ACCELERATED);
                SDL_SetRenderDrawColor(myGame->g_pRenderer, 0, 0, 0, 255);
                SDL_RenderClear(myGame->g_pRenderer);

            }


    }else{


        return 0;
    }




    return 1;


}

void handleEvents(gameState *state, coordonnees *dep, coordonnees *raquette, coordonnees *balle){


    SDL_Event event;

     int i;
     i=dep;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_LEFT:dep->state = 1; break;

                                case SDLK_RIGHT:dep->state = 2; break;



                            }
                            break;

        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
                case SDLK_LEFT:dep->state =0; break;

                case SDLK_RIGHT:dep->state = 0; break;
            }

            ;break;

        default:break;

        }
    }



}

void blocraquette(coordonnees *dep){


 if (dep->state == 1 && dep->x>19) {

         dep->x-=6;
 }
 if (dep->state==2 && dep->x<SCREEN_WIDTH -142) {

         dep->x+=6;


}
}


void InitBriques(brique Briques[5][5]){

    int i;
    int j;

    for (i=0;i<5;i++){
        for (j=0;j<5;j++){

            Briques[i][j].hauteur=60;
            Briques[i][j].largeur=62;
            Briques[i][j].position.x=32+i*Briques[i][j].largeur; //16 pour le side
            Briques[i][j].position.y=j*Briques[i][j].hauteur;

        }
    }


}


void AfficheBriques(game *myGame,gameState state, brique Briques[5][5]){


        int i;
        int j;

        SDL_Rect rectangleDest;
        SDL_Rect rectangleSource;


        myGame->g_surface = IMG_Load("./assets/brique.png");//Chargement de l'image bitmap

         if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", SDL_GetError());
            // handle error
        }

        if(myGame->g_surface){

                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){



                    for (i=0;i<5;i++){
                        for (j=0;j<5;j++){

                            //gestion des animations
                            rectangleSource.x=0; //x=0 y=0 => rouge    x=64 y=0 =>jaune     x=0 y=24 => vert   x=64 y=24 =>bleu
                            rectangleSource.y=24;
                            rectangleSource.w=Briques[i][j].largeur;
                            rectangleSource.h=Briques[i][j].hauteur;



                            //Définition du rectangle dest pour dessiner Bitmap
                            rectangleDest.x=Briques[i][j].position.x;//debut x
                            rectangleDest.y=Briques[i][j].position.y;//debut y
                            rectangleDest.w=rectangleSource.w; //Largeur
                            rectangleDest.h=rectangleSource.h; //Hauteur

                            SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);
                        }
                    }

                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }

    destroyTexture(myGame);


}

void InitBalle(balle *Balle){

    Balle->position.x=SCREEN_WIDTH/2-24;
    Balle->position.y=SCREEN_HEIGHT/2-24;
    Balle->vitesse=5;
    Balle->hauteur=24;
    Balle->largeur=24;
    Balle->haut_bas=0;
    Balle->gauche_droite=0;


}

void AfficheBalle(game *myGame,gameState state,balle *Balle){

        SDL_Rect rectangleDest;
        SDL_Rect rectangleSource;


        myGame->g_surface = IMG_Load("./assets/ball.png");//Chargement de l'image bitmap

         if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", SDL_GetError());
            // handle error
        }

        if(myGame->g_surface){

                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                    //gestion des animations
                    rectangleSource.x=0;
                    rectangleSource.y=0;
                    rectangleSource.w=Balle->largeur;
                    rectangleSource.h=Balle->hauteur;



                    //Définition du rectangle dest pour dessiner Bitmap
                    rectangleDest.x=Balle->position.x;//debut x
                    rectangleDest.y=Balle->position.y;//debut y
                    rectangleDest.w=rectangleSource.w; //Largeur
                    rectangleDest.h=rectangleSource.h; //Hauteur

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);

                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }

    destroyTexture(myGame);


}


void DeplacementBalle(balle *Balle){

    if (Balle->haut_bas==1){

        Balle->position.y+=Balle->vitesse;

    }
    else if (Balle->haut_bas==0){

        Balle->position.y-=Balle->vitesse;

    }

    if (Balle->gauche_droite==1){

        Balle->position.x+=Balle->vitesse;
    }
    else if (Balle->gauche_droite==0){

        Balle->position.x-=Balle->vitesse;
    }

}


void CollisionBalle(balle *Balle, coordonnees *dep){

    if (Balle->position.x>=SCREEN_WIDTH-40){

        Balle->gauche_droite=0;

    }else if (Balle->position.x<=16){

        Balle->gauche_droite=1;
    }

//    if ((Balle->position.y+24>=dep->coordonnees.y) && (Balle->position.x>=dep->coordonnees.x) && (Balle->position.x+24<=dep->coordonnees.x+128)){

  //    Balle->haut_bas=0;

  // }else if (Balle->position.y<=0){

  //      Balle->haut_bas=1;
   // }



}




void cutBitmapTexture(game *myGame,gameState state,coordonnees *dep,coordonnees *mvt){

        SDL_Rect rectangleDest;
        SDL_Rect rectangleSource;
        SDL_Rect sideDest;
        SDL_Rect sideSource;
        SDL_Rect side1Dest;
        SDL_Rect side1Source;
        SDL_Rect balleDest;
        SDL_Rect balleSource;
        SDL_Rect side2Dest;
        SDL_Rect side2Source;
        SDL_Rect briqueDest;
        SDL_Rect briqueSource;


        myGame->g_surface = IMG_Load("./assets/paddle.png");//Chargement de l'image png


        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                    rectangleSource.x=0;//128*(int)((SDL_GetTicks()/100)%6);
                    rectangleSource.y=0;
                    rectangleSource.w=128;//1 image = w:128 et h:82
                    rectangleSource.h=30;


                SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                    //Définition du rectangle dest pour dessiner Bitmap
                    rectangleDest.x= dep->x;//debut x
                    rectangleDest.y=450;//debut y
                    rectangleDest.w=rectangleSource.w; //Largeur
                    rectangleDest.h=rectangleSource.h; //Hauteur

                SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&rectangleSource,&rectangleDest);



                     myGame->g_surface = IMG_Load("./assets/side.png");//Chargement de l'image png

        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                    sideSource.x=1;
                    sideSource.y=1;
                    sideSource.w=15;//1 image = w:128 et h:82
                    sideSource.h=500;

                SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                     sideDest.x=0;
                     sideDest.y=1;
                     sideDest.w=15;
                     sideDest.h=500;

                SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&sideSource,&sideDest);


                             myGame->g_surface = IMG_Load("./assets/side.png");//Chargement de l'image png

        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                    side1Source.x=1;
                    side1Source.y=1;
                    side1Source.w=15;//1 image = w:128 et h:82
                    side1Source.h=500;

                SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                    sideDest.x=705;
                    sideDest.y=1;
                    sideDest.w=15;
                    sideDest.h=500;

                SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&sideSource,&sideDest);

    /*myGame->g_surface = IMG_Load("./assets/ball.png");//Chargement de l'image png

        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                    myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                    SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite



                    if(myGame->g_texture){


                      balleSource.x=0;
                      balleSource.y=0;
                      balleSource.w=128;//1 image = w:128 et h:82
                      balleSource.h=30;

                    SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                      balleDest.x=dep->x;
                      balleDest.y=425;
                      balleDest.w=25;
                      balleDest.h=25;

                    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&balleSource,&balleDest);*/





                     myGame->g_surface = IMG_Load("./assets/sidehor.png");//Chargement de l'image png


        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                   side2Source.x=15;//128*(int)((SDL_GetTicks()/100)%6);
                    side2Source.y=0;
                    side2Source.w=700;//1 image = w:128 et h:82
                    side2Source.h=15;


                SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                    //Définition du rectangle dest pour dessiner Bitmap
                    side2Dest.x=15 ;//debut x
                    side2Dest.y=0;//debut y
                    side2Dest.w=690; //Largeur
                    side2Dest.h=15; //Hauteur

                SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,&side2Source,&side2Dest);


         /*       myGame->g_surface = IMG_Load("./assets/brique.png");//Chargement de l'image png


        if(!myGame->g_surface) {
            fprintf(stdout,"IMG_Load: %s\n", IMG_GetError());
            // handle error
        }

        if(myGame->g_surface){


                 myGame->g_texture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_surface); // Préparation du sprite
                 SDL_FreeSurface(myGame->g_surface); // Libération de la ressource occupée par le sprite

                if(myGame->g_texture){

                    briqueSource.x=0;//128*(int)((SDL_GetTicks()/100)%6);
                    briqueSource.y=0;
                    briqueSource.w=64;//1 image = w:128 et h:82
                    briqueSource.h=24;


                SDL_QueryTexture(myGame->g_texture,NULL,NULL,NULL,NULL);

                    //Définition du rectangle dest pour dessiner Bitmap
                   briqueDest.x= 20;//debut x
                     briqueDest.y=20;//debut y
                     briqueDest.w=70; //Largeur
                    briqueDest.h=30; //Hauteur

                SDL_RenderCopy(myGame->g_pRenderer,myGame->g_texture,& briqueSource,& briqueDest);*/












                    SDL_RenderPresent(myGame->g_pRenderer); // Affichage

                    SDL_RenderClear(myGame->g_pRenderer);

                }

                else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
        }


    destroyTexture(myGame);

}
        }
            }

        }
}
        }
}










void delay(unsigned int frameLimit){
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}




void destroy(game *myGame){

    //Destroy render
    if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);


    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}



void destroyTexture(game *myGame){

    //Destroy texture
    if(myGame->g_texture!=NULL)
            SDL_DestroyTexture(myGame->g_texture);


}
