#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.hpp"
#include "input.hpp"

class Player{
     struct Point{
            int x;
            int y;
        };
    public:
        Player();
        //Accesseurs
        int getX() const;
        int getY() const;
        int getW() const;
        int getH() const;
        float getDirX() const;
        float getDirY() const;
        int getOnGround() const;
        int getLife() const;
        int getVies() const;
        int getEtoiles() const;
        int getDirection() const;

        //Mutateurs
        void setX(int valeur);
        void setY(int valeur);
        void setW(int valeur);
        void setH(int valeur);
        void setDirX(float valeur);
        void setDirY(float valeur);
        void setOnGround(bool valeur);
        void setTimerMort(int valeur);
        void setVies(int valeur);
        void setEtoiles(int valeur);
        void setCheckpoint(bool valeur);

        //Autres méthodes de fonctionnement du personnage
        void initialize(Map &map,bool newLevel);
        void draw(Map &map,sf::RenderWindow &window);
        void update(Input &input,Map &map);
        void centerScrolling(Map &map);
        void mapCollision(Map &map);
        Point segment2segment(int Ax0,int Ay0,int Bx0,int By0,int Cx0,int Cy0,int Dx0,int Dy0);
        void getSlopeSegment(int tx,int ty,int pente,Point &s1,Point &s2);
        int slopeEquation(int pente,double *a,double *b);
        int checkSlope(Map &map);

    private:
        //Points de vie/santé+chrono d'invicibilité
        int life;
        int invincibleTimer;

        //Vie et étoiles(100étoiles=1 vie)
        int vies;
        int etoiles;

        //Coordonnées du sprite
        int x;
        int y;

        //Largeur,hauteur du sprite
        int h;
        int w;

        //Checkpoint pour le héros (actif ou non)
        bool checkpointActif;
        //Coordonnées de réapparition(respawn)
        int respawnX;
        int respawnY;

        // Variables utiles pour l'animation :
        // Numéro de la frame (= image) en cours + timer
        int frameNumber, frameTimer, frameMax;
        // Nombre max de frames, état du sprite et direction
        // dans laquelle il se déplace (gauche / droite)
        int etat, direction;
        
        // Variables utiles pour la gestion des collisions :
        //Est-il sur le sol, chrono une fois mort
        int timerMort;
        bool onGround;
        
        //Vecteurs de déplacement temporaires avant détection
        //des collisions avec la map
        float dirX, dirY;
        //Sauvegarde des coordonnées de départ
        int saveX, saveY;
        
        //Variable pour le double saut
        bool Playerjump;
        
        //Gestion des pentes par Stephantasy
        float dirXmem, dirYmem;
        int posXmem, posYmem;
        int wasOnGround;
        int wasOnSlope;
        
        //Spritesheet de Rabidja
        sf::Texture rabidjaTexture;
        sf::Sprite rabidja;
        
        
        
        /******************/
        /* Constantes */
        /******************/
        
        
        //Nombre max de levels
        const int LEVEL_MAX = 2;
        
        /* Taille maxi de la map : 400 x 150 tiles */
        const int MAX_MAP_X = 400;
        const int MAX_MAP_Y = 150;
        
        /* Taille d'une tile (32 x 32 pixels) */
        const int TILE_SIZE = 32;
        
        /* Constante pour l'animation */
        const int TIME_BETWEEN_2_FRAMES_PLAYER = 4;
        
        /* Taille du sprite de notre héros (largeur = width et hauteur = heigth) */
        const int PLAYER_WIDTH = 40;
        const int PLAYER_HEIGTH = 50;
        
        //Vitesse de déplacement en pixels du sprite
        const int PLAYER_SPEED = 4;
        
        //Valeurs attribuées aux états/directions
        const int IDLE = 0;
        const int WALK = 1;
        const int JUMP1 = 2;
        const int JUMP2 = 3;
        const int DEAD = 4;
        
        const int RIGHT = 1;
        const int LEFT = 2;
        
        //Constantes définissant la gravité et la vitesse max de chute
        const double GRAVITY_SPEED = 0.6;
        const int MAX_FALL_SPEED = 15;
        const int JUMP_HEIGHT = 10;
        
        // Taille de la fenêtre : 800x480 pixels
        const int SCREEN_WIDTH = 800;
        const int SCREEN_HEIGHT = 480;
        
        //Constantes pour les limites de la caméra avant scrolling
        const int LIMITE_X = 400;
        const int LIMITE_Y = 220;
        const int LIMITE_W = 100;
        const int LIMITE_H = 80;
        
        //Enum pour les boutons
        enum{ up, down, right, left, attack, jump, enter };
        
        
        
        /*************************/
        /* VALEURS DES TILES */
        /************************/
        
        // Constante définissant le seuil entre les tiles traversables
        // (blank) et les tiles solides
        const int BLANK_TILE = 99;
        
        //Plateformes traversables
        const int TILE_TRAVERSABLE = 80;
        
        //Tiles Power-ups
        const int TILE_POWER_UP_DEBUT = 77;
        const int TILE_POWER_UP_FIN = 79;
        const int TILE_POWER_UP_COEUR = 78;
        
        //Autres Tiles spéciales
        const int TILE_RESSORT = 125;
        const int TILE_CHECKPOINT = 23;
        const int TILE_MONSTRE = 136;
        const int TILE_PIKES = 127;
        
        //Tiles plateformes mobiles
        const int TILE_PLATEFORME_DEBUT = 130;
        const int TILE_PLATEFORME_FIN = 131;
        
        // Tiles pentes à 26.5° ; BenH = de BAS en HAUT ; HenB = De HAUT en BAS
        const int TILE_PENTE_26_BenH_1 = 69;
        const int TILE_PENTE_26_BenH_2 = 70;
        const int TILE_PENTE_26_HenB_1 = 71;
        const int TILE_PENTE_26_HenB_2 = 72;

        };

#endif