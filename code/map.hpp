#ifndef MAP_HPP
#define MAP_HPP

//bibliothèque que l'on utilisera
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//définition de la classe Map
class Map{
    public:
        Map();
        //Accesseurs
        int getBeginAbscisse() const;
        int getBeginOrdonne() const;
        int getPositionStartAbscisse() const;
        int getPositionStartOrdonne() const;
        int getMaxAbscisse() const;
        int getMaxOrdonne() const;
        int getTile(int y,int x)  const;
        int getLevel() const;
        //Mutateur
        void setLevel(int value);
        void setPositionStartAbscisse(int value);
        void setPositionStartOrdonne(int value);
        void setTile(int y, int x, int value);
        //methods 

        void drawBackground(sf::RenderWindow &window);
        void loadMap(std::string nomMap);
        void draw(int layer,sf::RenderWindow &window);
        void changeLevel();
        void testDefilement();

    private:
        //numero du tileset à afficher
        int tilesetShowed;
        //coordonnées de départ du personnage, lors du commencement du niveau
        int beginX;
        int beginY;
        //coordonnées de début lorsqu'on doit dessiner la map
        int PositionStartAbscisse;
        int PositionStartOrdonne;
        //coordonnées fin de la map
        int maxAbscisse;
        int maxOrdonne;
        //tableau à double dimension représentant la map de tiles
        int tile[150][400];
        //deuxième couche
        int tile2[150][400];
        //troisième couche
        int tile3[150][400];

        //Timer et numéro du tileset à afficher pour animer la map
        int mapTimer;
        int tilesetNumber;

        //Numéro du niveau en cours
        int level;

        //Background
        sf::Texture backgroundTexture;
        sf::Sprite background;

        //Tilesets
        sf::Texture tileset1Texture;
        sf::Sprite tileset1;
        sf::Texture tileset1BTexture;
        sf::Sprite tileset1B;


    //Constantes
        const int SCREEN_WIDTH=800;
        const int SCREEN_HEIGHT=480;

        //Taille maxi de la map
        const int MAX_MAP_X=400;
        const int MAX_MAP_Y=150;

        //taille d'une tile
        const int TILE_SIZE=32;
    
        //constante pour l'animation
        const int TIME_BETWEEN_2_FRAMES=20;
      

    //Valeurs des tiles

        //Constante définissant le seuil entre les tiles traversables
        //(blank ) et les tiles solides
        const int BLANK_TILE=99;
        //Plateformes traversables
        const int TILE_TRAVERSABLE=80;
        //Tiles Power-ups
        const int TILE_POWER_UP_DEBUT=77;
        const int TILE_POWER_UP_FIN=79;
        const int TILE_POWER_UP_COEUR=78;
        //Autres tiles spéciales
        const int TILE_RESSORT=125;
        const int TILE_CHECKPOINT=23;
        const int TILE_MONSTRE=136;
        const int TILE_PIKES=127;
        //Tiles plateformes mobiles
        const int TILE_PLATEFORME_DEBUT=130;
        const int TILE_PLATEFORME_FIN=131;
        //Tiles pentes à 26.5° ; BenH=de BAS en HAUT ;HenB=de Haut en Bas
        const int TILE_PENTE_26_BenH_1=69;
        const int TILE_PENTE_26_BenH_2=70;
        const int TILE_PENTE_26_HenB_1=71;
        const int TILE_PENTE_26_HenB_2=72;
 };
 #endif