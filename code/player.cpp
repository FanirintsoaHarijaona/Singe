#include "player.hpp"
#include "input.hpp"
 
 
using namespace std;
using namespace sf;
 
 
//Constructeur
 
Player::Player()
{
//Chargement de la spritesheet de Rabidja
        if (!rabidjaTexture.loadFromFile("../graphics/rabidja.png"))
        {
// Erreur
        cout << "Erreur durant le chargement du spritesheet de Rabidja." << endl;
        }
        else
                rabidja.setTexture(rabidjaTexture);
 
//Initialisation des variables :
        dirX = 0;
        dirY = 0;
        life = 3;
        invincibleTimer = 0;
        x = y = h = w = 0;
        checkpointActif = false;
        respawnX = respawnY = 0;
        frameNumber = frameTimer = frameMax = 0;
        etat = direction = 0;
        timerMort = 0;
        onGround = false;
        dirX = dirY = 0;
        saveX = saveY = 0;
        Playerjump = false;
 
}
 
 
//Accesseurs
int Player::getX(void) const { return x; }
int Player::getY(void) const { return y; }
int Player::getW(void) const { return w; }
int Player::getH(void) const { return h; }
float Player::getDirX(void) const { return dirX; }
float Player::getDirY(void) const { return dirY; }
int Player::getOnGround(void) const { return onGround; }
int Player::getLife(void) const { return life; }
int Player::getVies(void) const { return vies; }
int Player::getEtoiles(void) const { return etoiles; }
int Player::getDirection(void) const { return direction; }
 
 
//Mutateurs
void Player::setX(int valeur) { x = valeur; }
void Player::setY(int valeur) { y = valeur; }
void Player::setW(int valeur) { w = valeur; }
void Player::setH(int valeur) { h = valeur; }
void Player::setDirX(float valeur) { dirX = valeur; }
void Player::setDirY(float valeur) { dirY = valeur; }
void Player::setOnGround(bool valeur) { onGround = valeur; }
void Player::setTimerMort(int valeur) { timerMort = valeur; }
void Player::setVies(int valeur) { vies = valeur; }
void Player::setEtoiles(int valeur) { etoiles = valeur; }
void Player::setCheckpoint(bool valeur) { checkpointActif = valeur; }

void Player::initialize(Map &map, bool newLevel)
{
//PV ?? 3
life = 3;
 
//Timer d'invincibilit?? ?? 0
invincibleTimer = 0;
 
//Indique l'??tat et la direction de notre h??ros
direction = RIGHT;
etat = IDLE;
 
//Indique le num??ro de la frame o?? commencer
frameNumber = 0;
 
//...la valeur de son chrono ou timer
frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
 
//... et son nombre de frames max (8 pour l'anim' IDLE
// = ne fait rien)
frameMax = 8;
 
/* Coordonn??es de d??marrage/respawn de notre h??ros */
if (checkpointActif == true)
{
x = respawnX;
y = respawnY;
}
else
{
x = map.getBeginAbscisse();
y = map.getBeginOrdonne();
}
 
//On r??initiliase les coordonn??es de la cam??ra
//si on change de niveau
if (newLevel == true)
{
map.setPositionStartAbscisse(map.getBeginAbscisse());
map.setPositionStartOrdonne(map.getBeginOrdonne());
}
 
/* Hauteur et largeur de notre h??ros */
w = PLAYER_WIDTH;
h = PLAYER_HEIGTH;
 
//Variables n??cessaires au fonctionnement de la gestion des collisions
timerMort = 0;
onGround = false;
}

void Player::draw(Map &map, RenderWindow &window)
{
        /* Gestion du timer */
        // Si notre timer (un compte ?? rebours en fait) arrive ?? z??ro
        if (frameTimer <= 0)
        {
        //On le r??initialise
        frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
        
        //Et on incr??mente notre variable qui compte les frames de 1 pour passer ?? la suivante
        frameNumber++;
        
        //Mais si on d??passe la frame max, il faut revenir ?? la premi??re :
        if (frameNumber >= frameMax)
        frameNumber = 0;
        }
        //Sinon, on d??cr??mente notre timer
        else
        frameTimer--;
        
        
        //Ensuite, on peut passer la main ?? notre fonction
        rabidja.setPosition(Vector2f(x - map.getPositionStartAbscisse(), y - map.getPositionStartOrdonne()));
        
        //Pour conna??tre le X de la bonne frame ?? dessiner, il suffit de multiplier
        //la largeur du sprite par le num??ro de la frame ?? afficher -> 0 = 0; 1 = 40; 2 = 80...
        //On calcule le Y de la bonne frame ?? dessiner, selon la valeur de l'??tat du h??ros :
        //Aucun Mouvement (Idle) = 0, marche (walk) = 1, etc...
        //Tout cela en accord avec notre spritesheet, of course ;)
        
        //Si on a ??t?? touch?? et qu'on est invincible
        if (invincibleTimer > 0)
        {
        //On fait clignoter le h??ros une frame sur deux
        //Pour ??a, on calcule si le num??ro de la frame en
        //cours est un multiple de deux
        if (frameNumber % 2 == 0)
        {
        //Gestion du flip (retournement de l'image selon que le sprite regarde ?? droite ou ?? gauche)
        if (direction == LEFT)
        {
        //On n'a plus de flip auto en SFML, il faut donc tout calculer
                rabidja.setTextureRect(sf::IntRect((frameNumber + 1) * w,etat * h,-w, h));
                window.draw(rabidja);
        }
        else
        {
                rabidja.setTextureRect(sf::IntRect(frameNumber * w,etat * h,w, h));
                window.draw(rabidja);
        }
     }
        //Sinon, on ne dessine rien, pour le faire clignoter
   }
        
        //Sinon, on dessine normalement
        else
        {
        //Gestion du flip (retournement de l'image selon que le sprite regarde ?? droite ou ?? gauche)
             if (direction == LEFT)
                {
        //On n'a plus de flip auto en SFML, il faut donc tout calculer
                        rabidja.setTextureRect(sf::IntRect((frameNumber + 1) * w,etat * h,-w, h));
                        window.draw(rabidja);
                }
                else
                {
                        rabidja.setTextureRect(sf::IntRect(frameNumber * w,etat * h,w, h));
                        window.draw(rabidja);
                }
        }
 
}


void Player::update(Input &input, Map &map)
{
//On rajoute un timer au cas o?? notre h??ros mourrait lamentablement en tombant dans un trou...
//Si le timer vaut 0, c'est que tout va bien, sinon, on le d??cr??mente jusqu'?? 0, et l??,
//on r??initialise.
//C'est pour ??a qu'on ne g??re le joueur que si ce timer vaut 0.
if (timerMort == 0)
{
//On g??re le timer de l'invincibilit??
if (invincibleTimer > 0)
invincibleTimer--;
 
//On r??initialise notre vecteur de d??placement lat??ral (X), pour ??viter que le perso
//ne fonce de plus en plus vite pour atteindre la vitesse de la lumi??re ! ;)
//Essayez de le d??sactiver pour voir !
dirX = 0;
 
// La gravit?? fait toujours tomber le perso : on incr??mente donc le vecteur Y
dirY += GRAVITY_SPEED;
 
//Mais on le limite pour ne pas que le joueur se mette ?? tomber trop vite quand m??me
if (dirY >= MAX_FALL_SPEED)
dirY = MAX_FALL_SPEED;
 
 
//Voil??, au lieu de changer directement les coordonn??es du joueur, on passe par un vecteur
//qui sera utilis?? par la fonction mapCollision(), qui regardera si on peut ou pas d??placer
//le joueur selon ce vecteur et changera les coordonn??es du player en fonction.
if (input.getButton().left == true)
{
dirX -= PLAYER_SPEED;
//Et on indique qu'il va ?? gauche (pour le flip
//de l'affichage, rappelez-vous).
direction = LEFT;
 
//Si ce n'??tait pas son ??tat auparavant et qu'il est bien sur
//le sol (car l'anim' sera diff??rente s'il est en l'air)
if (etat != WALK && onGround == true)
{
//On enregistre l'anim' de la marche et on l'initialise ?? 0
etat = WALK;
frameNumber = 0;
frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
frameMax = 8;
}
}
 
//Si on d??tecte un appui sur la touche fl??ch??e droite
else if (input.getButton().right == true)
{
//On augmente les coordonn??es en x du joueur
dirX += PLAYER_SPEED;
//Et on indique qu'il va ?? droite (pour le flip
//de l'affichage, rappelez-vous).
direction = RIGHT;
 
//Si ce n'??tait pas son ??tat auparavant et qu'il est bien sur
//le sol (car l'anim' sera diff??rente s'il est en l'air)
if (etat != WALK && onGround == true)
{
//On enregistre l'anim' de la marche et on l'initialise ?? 0
etat = WALK;
frameNumber = 0;
frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
frameMax = 8;
}
}
 
//Si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant l'inactivit?? (Idle)
else if (input.getButton().right == false && input.getButton().left == false && onGround == true)
{
//On teste si le joueur n'??tait pas d??j?? inactif, pour ne pas recharger l'animation
//?? chaque tour de boucle
if (etat != IDLE)
{
//On enregistre l'anim' de l'inactivit?? et on l'initialise ?? 0
etat = IDLE;
frameNumber = 0;
frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
frameMax = 8;
}
}
 
 
//Et voici la fonction de saut tr??s simple :
//Si on appuie sur la touche saut et qu'on est sur le sol, alors on attribue une valeur
//n??gative au vecteur Y
//parce que sauter veut dire se rapprocher du haut de l'??cran et donc de y=0.
if (input.getButton().jump == true)
{
if (onGround == true)
{
dirY = -JUMP_HEIGHT;
onGround = false;
Playerjump = true;
}
// Si on est en saut 1, on peut faire un deuxi??me bond et on remet jump1 ?? 0
else if (Playerjump == true)
{
dirY = -JUMP_HEIGHT;
Playerjump = false;
}
input.setButton(jump, false);
}
 
 
/* R??active la possibilit?? de double saut si on tombe sans sauter */
if (onGround == true)
Playerjump = true;
 
 
//On g??re l'anim du saut
if (onGround == false)
{
//Si on est en saut 1, on met l'anim' du saut normal
if (Playerjump == true)
{
if (etat != JUMP1)
{
etat = JUMP1;
frameNumber = 0;
frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
frameMax = 2;
}
}
else
{
if (etat != JUMP2)
{
etat = JUMP2;
frameNumber = 0;
frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
frameMax = 4;
}
}
}
 
//On rajoute notre fonction de d??tection des collisions qui va mettre ??
//jour les coordonn??es de notre super lapin.
mapCollision(map);
 
//On g??re le scrolling (fonction ci-dessous)
centerScrolling(map);
 
}
 
//Gestion de la mort quand le h??ros tombe dans un trou :
//Si timerMort est diff??rent de 0, c'est qu'il faut r??initialiser le joueur.
//On ignore alors ce qui pr??c??de et on joue cette boucle (un wait en fait) jusqu'?? ce que
// timerMort == 1. A ce moment-l??, on le d??cr??mente encore -> il vaut 0 et on r??initialise
//le jeu avec notre bonne vieille fonction d'initialisation ;) !
if (timerMort > 0)
{
timerMort--;
 
if (timerMort == 0)
{
//On perd une vie
vies--;
 
// Si on est mort, on r??initialise le niveau
map.changeLevel();
initialize(map, false);
}
}
}


v
oid Player::centerScrolling(Map &map)
{
// Nouveau scrolling ?? sous-bo??te limite :
//Pour ??viter les effets de saccades dus ?? une cam??ra qui se
//centre automatiquement et constamment sur le joueur (ce qui
//peut en rendre malade certains...), on cr??e une "bo??te" imaginaire
//autour du joueur. Quand on d??passe un de ses bords (en haut, en bas,
//?? gauche ou ?? droite), on scrolle.
//Mais l?? encore, au lieu de centrer sur le joueur, on d??place simplement
//la cam??ra jusqu'?? arriver au joueur. On a chang?? ici la valeur ?? 4 pixels
//pour que le jeu soit plus rapide.
int cxperso = x + w / 2;
int cyperso = y + h / 2;
int xlimmin = map.getBeginAbscisse() + LIMITE_X;
int xlimmax = xlimmin + LIMITE_W;
int ylimmin = map.getBeginOrdonne() + LIMITE_Y;
int ylimmax = ylimmin + LIMITE_H;
 
//Effet de retour en arri??re quand on est mort :
//Si on est tr??s loin de la cam??ra, plus loin que le bord
//de la map, on acc??l??re le scrolling :
        if (cxperso < map.getBeginAbscisse())
        {
                map.setPositionStartAbscisse(map.getBeginAbscisse() - 30);
        }
 
//Si on d??passe par la gauche, on recule la cam??ra
        else if (cxperso < xlimmin)
        {
                map.setPositionStartAbscisse(map.getBeginAbscisse() - 4);
        }
 
//Effet de retour en avant quand on est mort (au
//cas o?? le joueur s'amuse ?? faire le niveau ?? rebours
//apr??s une checkpoint) :
//Si on est tr??s loin de la cam??ra, plus loin que le bord
//de la map, on acc??l??re le scrolling :
        if (cxperso > map.getBeginAbscisse() + SCREEN_WIDTH)
        {
                map.setPositionStartAbscisse(map.getBeginAbscisse() + 30);
        }
 
//Si on d??passe par la droite, on avance la cam??ra
        else if (cxperso > xlimmax)
        {
                map.setPositionStartAbscisse(map.getBeginAbscisse() + 4);
        }
 
//Si on arrive au bout de la map ?? gauche, on stoppe le scrolling
        if (map.getBeginAbscisse() < 0)
        {
                map.setPositionStartAbscisse(0);
        }
 
//Si on arrive au bout de la map ?? droite, on stoppe le scrolling ?? la
//valeur Max de la map - la moiti?? d'un ??cran (pour ne pas afficher du noir).
        else if (map.getBeginAbscisse() + SCREEN_WIDTH >= map.getMaxAbscisse())
        {
                map.setPositionStartAbscisse(map.getMaxAbscisse() - SCREEN_WIDTH);
        }
 
//Si on d??passe par le haut, on remonte la cam??ra
        if (cyperso < ylimmin)
        {
                map.setPositionStartOrdonne(map.getBeginOrdonne() - 4);
        }
 
//Si on d??passe par le bas, on descend la cam??ra
        if (cyperso > ylimmax)
        {
//Sauf si on tombe tr??s vite, auquel cas, on acc??l??re la cam??ra :
                if (dirY >= MAX_FALL_SPEED - 2)
                {
                        map.setPositionStartOrdonne(map.getBeginOrdonne() + MAX_FALL_SPEED + 1);
                }
                else
                {
                        map.setPositionStartOrdonne(map.getBeginOrdonne() + 4);
                }
        }
 
//Si on arrive au bout de la map en haut, on stoppe le scrolling
        if (map.getBeginAbscisse() < 0)
        {
                map.setPositionStartOrdonne(0);
        }
 
//Si on arrive au bout de la map en bas, on stoppe le scrolling ?? la
//valeur Max de la map - la moiti?? d'un ??cran (pour ne pas afficher du noir).
        else if (map.getBeginOrdonne() + SCREEN_HEIGHT >= map.getMaxOrdonne())
        {
                map.setPositionStartAbscisse(map.getMaxOrdonne() - SCREEN_HEIGHT);
        }
 
}


void Player::mapCollision(Map &map)
{
 
        int i, x1, x2, y1, y2;
        dirXmem = dirX;
wasOnGround = onGround;
dirYmem = dirY;
posXmem = x;
posYmem = y;
// Contr??le des pentes
checkSlope(map);
/* D'abord, on consid??re le joueur en l'air jusqu'?? temps
d'??tre s??r qu'il touche le sol */
        onGround = 0;
 
/* Ensuite, on va tester les mouvements horizontaux en premier
(axe des X). On va se servir de i comme compteur pour notre boucle.
En fait, on va d??couper notre sprite en blocs de tiles pour voir
quelles tiles il est susceptible de recouvrir.
On va donc commencer en donnant la valeur de Tile_Size ?? i pour qu'il
teste la tile o?? se trouve le x du joueur mais aussi la suivante SAUF
dans le cas o?? notre sprite serait inf??rieur ?? la taille d'une tile.
Dans ce cas, on lui donnera la vraie valeur de la taille du sprite
Et on testera ensuite 2 fois la m??me tile. Mais comme ??a notre code
sera op??rationnel quelle que soit la taille de nos sprites ! */
 
        if (h > TILE_SIZE)
                i = TILE_SIZE;
        else
                i = h;
 
 
//On lance alors une boucle for infinie car on l'interrompra selon
//les r??sultats de nos calculs
        for (;;)
        {
//On va calculer ici les coins de notre sprite ?? gauche et ??
//droite pour voir quelle tile ils touchent.
                x1 = (x + dirX) / TILE_SIZE;
                x2 = (x + dirX + w - 1) / TILE_SIZE;
 
//M??me chose avec y, sauf qu'on va descendre au fur et ?? mesure
//pour tester toute la hauteur de notre sprite, gr??ce ?? notre
//fameuse variable i.
                y1 = (y) / TILE_SIZE;
                y2 = (y + i - 1) / TILE_SIZE;
 
//De l??, on va tester les mouvements initi??s dans updatePlayer
//gr??ce aux vecteurs dirX et dirY, tout en testant avant qu'on
//se situe bien dans les limites de l'??cran.
                if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
                {
//Si on a un mouvement ?? droite
                        if (dirX > 0)
                        {
//On v??rifie si les tiles recouvertes sont solides
        if (map.getTile(y1, x2) > BLANK_TILE || map.getTile(y2, x2) > BLANK_TILE)
{
// Si c'est le cas, on place le joueur aussi pr??s que possible
// de ces tiles, en mettant ?? jour ses coordonn??es. Enfin, on
//r??initialise son vecteur d??placement (dirX).
 
x = x2 * TILE_SIZE;
x -= w + 1;
dirX = 0;
}
}
 
//M??me chose ?? gauche
else if (dirX < 0)
{
if (map.getTile(y1, x1) > BLANK_TILE || map.getTile(y2, x1) > BLANK_TILE)
{
x = (x1 + 1) * TILE_SIZE;
dirX = 0;
}
}
 
}
 
//On sort de la boucle si on a test?? toutes les tiles le long de la hauteur du sprite.
if (i == h)
{
break;
}
 
//Sinon, on teste les tiles sup??rieures en se limitant ?? la heuteur du sprite.
i += TILE_SIZE;
 
if (i > h)
{
i = h;
}
}
 
 
//On recommence la m??me chose avec le mouvement vertical (axe des Y)
if (w > TILE_SIZE)
i = TILE_SIZE;
else
i = w;
 
 
for (;;)
{
x1 = (x) / TILE_SIZE;
x2 = (x + i) / TILE_SIZE;
 
y1 = (y + dirY) / TILE_SIZE;
y2 = (y + dirY + h) / TILE_SIZE;
 
if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
{
if (dirY > 0)
{
// D??placement en bas
//Gestion des plateformes traversables : elles se situent juste avant
//les tiles bloquantes dans notre tileset (dont la valeur butoire est
//BLANK_TILE). Il suffit donc d'utiliser le num??ro de la premi??re tile
//traversable au lieu de BLANK_TILE pour bloquer le joueur,
//seulement quand il tombe dessus (sinon, il passe au-travers
//et le test n'est donc pas effectu?? dans les autres directions
if (map.getTile(y2, x1) > TILE_TRAVERSABLE || map.getTile(y2, x2) > TILE_TRAVERSABLE)
{
//Si la tile est une plateforme ou une tile solide, on y colle le joueur et
//on le d??clare sur le sol (onGround).
y = y2 * TILE_SIZE;
y -= h;
dirY = 0;
onGround = 1;
}
}
 
else if (dirY < 0)
{
// D??placement vers le haut
if (map.getTile(y1, x1) > BLANK_TILE || map.getTile(y1, x2) > BLANK_TILE)
{
y = (y1 + 1) * TILE_SIZE;
dirY = 0;
}
}
}
 
//On teste la largeur du sprite (m??me technique que pour la hauteur pr??c??demment)
if (i == w)
{
break;
}
 
i += TILE_SIZE;
 
if (i > w)
{
i = w;
}
}
 
/* Maintenant, on applique les vecteurs de mouvement si le sprite n'est pas bloqu?? */
x += dirX;
y += dirY;
 
//Et on contraint son d??placement aux limites de l'??cran.
if (x < 0)
{
x = 0;
}
 
else if (x + w >= map.getMaxAbscisse())
{
//Si on touche le bord droit de l'??cran, on passe au niveau sup
map.setLevel(map.getLevel() + 1);
 
//Si on d??passe le niveau max, on annule et on limite le d??placement du joueur
if (map.getLevel() > LEVEL_MAX)
{
map.setLevel(LEVEL_MAX);
x = map.getMaxAbscisse() - w - 1;
}
 
//Sinon, on passe au niveau sup, on charge la nouvelle map et on r??initialise le joueur
else
{
//On d??sactive le checkpoint
checkpointActif = 0;
 
map.changeLevel();
initialize(map, true);
}
}
 
//Maintenant, s'il sort de l'??cran par le bas (chute dans un trou sans fond), on lance le timer
//qui g??re sa mort et sa r??initialisation (plus tard on g??rera aussi les vies).
if (y > map.getMaxOrdonne())
{
timerMort = 60;
}
}



Player::Point Player::segment2segment(int Ax0, int Ay0, int Bx0, int By0, int Cx0, int Cy0, int Dx0, int Dy0)
{
//Adaptation de la fonction ??crite par Stephantasy en SFML2
 
// Cette fonciton permet de savoir si 2 segments se touchent
// En param??tres, les coordonn??es des points du segment AB et du segment CD
 
double Sx;
double Sy;
 
double Ax = Ax0;
double Ay = Ay0;
double Bx = Bx0;
double By = By0;
double Cx = Cx0;
double Cy = Cy0;
double Dx = Dx0;
double Dy = Dy0;
 
Player::Point point;
point.x = -1;
point.y = -1;
 
if (Ax == Bx)
{
if (Cx == Dx)
return point;
else
{
double pCD = (Cy - Dy) / (Cx - Dx);
Sx = Ax;
Sy = pCD*(Ax - Cx) + Cy;
}
}
else
{
if (Cx == Dx)
{
double pAB = (Ay - By) / (Ax - Bx);
Sx = Cx;
Sy = pAB*(Cx - Ax) + Ay;
}
else if ((Ax == Cx && Ay == Cy) || (Ax == Dx && Ay == Dy))
{
// si le point de d??part de la trajectoire du Sprite est
// sur le point de d??part ou d'arriv??e du segment de la pente,
// on renvoie ce point comme ??tant l'intersection.
Sx = Ax;
Sy = Ay;
}
else
{
double pCD = (Cy - Dy) / (Cx - Dx);
double pAB = (Ay - By) / (Ax - Bx);
double oCD = Cy - pCD*Cx;
double oAB = Ay - pAB*Ax;
Sx = (oAB - oCD) / (pCD - pAB);
Sy = pCD*Sx + oCD;
}
}
 
if ((Sx<Ax && Sx<Bx) | (Sx>Ax && Sx>Bx) | (Sx<Cx && Sx<Dx) | (Sx>Cx && Sx>Dx)
| (Sy<Ay && Sy<By) | (Sy>Ay && Sy>By) | (Sy<Cy && Sy<Dy) | (Sy>Cy && Sy>Dy))
return point;
 
point.x = Sx;
point.y = Sy;
return point;
}
 
 
void Player::getSlopeSegment(int tx, int ty, int pente, Point &s1, Point &s2)
{
//Adaptation de la fonction ??crite par Stephantasy en SFML2
 
// Cette fonction renvoie les valeurs (x, y) des points :
// - s1 = Point en bas ?? gauche de la pente pass?? en param??tre
// - s2 = Point en haut ?? droite de la pente pass?? en param??tre
// Ces points sont une position sur la Map.
 
int cy, dy;
 
if (pente == TILE_PENTE_26_BenH_1)
{
cy = 0;
dy = 16;
}
else if (pente == TILE_PENTE_26_BenH_2)
{
cy = 16;
dy = 32;
}
else if (pente == TILE_PENTE_26_HenB_1)
{
cy = 32;
dy = 16;
}
else if (pente == TILE_PENTE_26_HenB_2)
{
cy = 16;
dy = 0;
}
else
{
cout << "### ERROR - getSlopeSegment() - Pente non connue ! ###" << endl;
}
 
 
// On ajoute la distance depuis le d??but/haut de la Map
s1.x = tx*TILE_SIZE;
s1.y = (ty + 1)*TILE_SIZE - cy;
s2.x = (tx + 1)*TILE_SIZE;
s2.y = (ty + 1)*TILE_SIZE - dy;
}
 
 
int Player::slopeEquation(int pente, double *a, double *b)
{
//Adaptation de la fonction ??crite par Stephantasy en SFML2
 
const double xLeft = 0;
const double xRight = 32.0;
int yLeft, yRight;
 
// On retourne son ??quation de la pente
// Diagonale ?? 26.5??
 
if (pente == TILE_PENTE_26_BenH_1)
{
// D??but et fin de la pente dans la Tile (en Y)
yLeft = 0;
yRight = 16;
}
else if (pente == TILE_PENTE_26_BenH_2)
{
yLeft = 16;
yRight = 32;
}
else if (pente == TILE_PENTE_26_HenB_1)
{
yLeft = 32;
yRight = 16;
}
else if (pente == TILE_PENTE_26_HenB_2)
{
yLeft = 16;
yRight = 0;
}
else
{
cout << "### ERROR - getSlopeSegment() - Pente non connue ! ###" << endl;
return 0;
}
 
// On d??termine l'??quation
double cd = (yRight - yLeft) / (xRight - xLeft); // Coefficient directeur
double oo = yLeft - cd * 0; // Ordonn??e ?? l'origine
*a = cd;
*b = oo;
 
return 1;
}
 
 
int Player::checkSlope(Map &map)
{
//Adaptation de la fonction ??crite par Stephantasy en SFML2
 
/*
* 2014/12/21 by stephantasy
*
* Fonction permettant de placer correctement le Sprite sur une Tile de type "pente".
*
* ATTENTION ! S'assurer que les Tiles "pentes" soient < BLANK_TILE dans le TileSet.
* En effet, "mapCollision" doit consid??rer que le Sprite peut traverser ces Tiles
* autant sur le plan horizontal que vertical.
*/
 
// Initialisation
int isOnSlope, goOnSlope, goOnSlopeUp, goOnSlopeDown;
isOnSlope = goOnSlope = goOnSlopeUp = goOnSlopeDown = 0;
int diagOffSet = 0;
int yc;
int resetWasOnSlope = 0, checkWasOnSlope = 1;
 
// Si on ne touche plus le sol, on ne se soucis plus de savoir qu'on ??tait sur une pente.
if (wasOnGround == 0)
{
wasOnSlope = 0;
}
 
// On r??cup??re la position du Sprite (?? noter qu'on effectue les tests avec le point "en bas au centre" du Sprite)
int posIniX = posXmem + w / 2;
int xa = posIniX / TILE_SIZE;
int posIniY = posYmem + h - 1;
int ya = posIniY / TILE_SIZE;
 
// On r??cup??re la destination du Sprite
int posEndX = posIniX + dirXmem;
int xb = posEndX / TILE_SIZE;
int posEndY = posIniY + 1 + dirYmem;
int yb = posEndY / TILE_SIZE;
 
// Est-ce qu'on est sur une pente ?
if (map.getTile(ya, xa) >= TILE_PENTE_26_BenH_1 && map.getTile(ya, xa) <= TILE_PENTE_26_HenB_2)
{
isOnSlope = map.getTile(ya, xa);
}
 
// Est-ce qu'on va sur une pente ?
if (map.getTile(yb, xb) >= TILE_PENTE_26_BenH_1 && map.getTile(yb, xb) <= TILE_PENTE_26_HenB_2)
{
goOnSlope = map.getTile(yb, xb);
}
 
// Est-ce que la Tile au-dessus de la destination du Sprite est une pente ?
if (map.getTile(yb - 1, xb) >= TILE_PENTE_26_BenH_1 && map.getTile(yb - 1, xb) <= TILE_PENTE_26_HenB_2)
{
goOnSlopeUp = map.getTile(yb - 1, xb);
}
 
// Est-ce que la Tile au-dessous de la destination du Sprite est une pente ?
// La subtilit?? ici est qu'on est (normalement) d??j?? sur une pente, mais que le Sprite se
// d??place si vite, qu'on ne voit pas que la Tile suivante est encore une pente !
// En fait, ce n'est pas grave, c'est juste un peu plus r??aliste de "coller" le Sprite au sol,
// plut??t que de laisser le Sprite "flotter" dans les airs jusqu'au sol, quelques pixels plus loin...
// (C'est surtout vrai pour les Tiles ?? pentes raides ou ?? grande vitesse)
else if (map.getTile(yb + 1, xb) >= TILE_PENTE_26_BenH_1 && map.getTile(yb + 1, xb) <= TILE_PENTE_26_HenB_2)
{
goOnSlopeDown = map.getTile(yb + 1, xb);
}
 
// Si on se dirige vers une pente
if (goOnSlope > 0)
{
double a, b;
 
// On r??cup??re l'??quation de la pente
if (!slopeEquation(goOnSlope, &a, &b)){ return 0; }
 
// On determine la position en x du Sprite dans la Tile
int xPos = posEndX - xb*TILE_SIZE;
 
// On calcule sa position en y
int yPos = a * xPos + b;
 
// On borne le ypos ?? 31
if (yPos > 31) { yPos = 31; }
 
// On calcul l'Offset entre le haut de la Tile et le sol de la pente
diagOffSet = TILE_SIZE - yPos;
 
// La Tile "pente" est ?? la m??me hauteur que la Tile o?? se trouve le Sprite
yc = yb;
 
// Le Sprite est ?? pr??sent sur une pente
wasOnSlope = goOnSlope;
 
// Puisqu'on traite le Sprite sur la pente,
// inutile de traiter le Sprite quittant la pente
checkWasOnSlope = 0;
}
 
// S'il y a une pente au dessus de celle o?? on va
// (c'est ?? dire la Tile juste ?? c??t?? du Sprite, car avec la gravit??,
// on "pointe" toujours la Tile en dessous)
else if (goOnSlopeUp > 0)
{
double a, b;
if (!slopeEquation(goOnSlopeUp, &a, &b)){ return 0; }
int xPos = posEndX - xb*TILE_SIZE;
int yPos = a * xPos + b;
if (yPos > 31) { yPos = 31; }
diagOffSet = TILE_SIZE - yPos;
 
// La Tile "pente" est 1 Tile au-dessus de la Tile o?? se trouve le Sprite
yc = yb - 1;
 
wasOnSlope = goOnSlopeUp;
checkWasOnSlope = 0;
}
 
// Si on tombe ici, c'est que le Sprite ne va pas sur une pente mais qu'il est sur une pente.
else if (isOnSlope > 0)
{
// Si on est en l'air,
if (wasOnGround == 0)
{
 
// Il faut v??rifier si le Sprite doit ??tre stopp?? par la pente.
// Pour cela, on contr??le si la trajectoire du sprite croise le sol de la pente.
// On v??rifie donc si ces segments se croisent et si oui, en quel point.
Player::Point segmentD, segmentF;
 
// On r??cup??re le segment de la pente
getSlopeSegment(xa, ya, isOnSlope, segmentD, segmentF);
 
// On r??cup??re la position du point de collision entre les segments (s'il y a lieu, sinon -1)
Player::Point point = segment2segment(posIniX, posIniY, posEndX, posEndY, segmentD.x, segmentD.y, segmentF.x, segmentF.y);
 
// Pas d'intersection
if (point.x == -1)
{
// On applique les valeurs de d??part afin d'??viter d'??tre repouss?? par la Tile
// solide (par mapCollision) lorsqu'on quitte une pente en sautant
x = posXmem;
dirX = dirXmem;
return 0;
}
 
else if (point.x < -1)
{
// Erreur dans la fonction "segment2segment()", on ne doit pas retourner de valeur < -1 !
cout << "ERROR - segment2segment() - Sprite aux coordonn??es n??gatives !\n" << endl;
x = posXmem;
dirX = dirXmem;
return 0;
}
 
// On positionne le Sprite
x = point.x - w / 2;
dirX = 0;
y = point.y;
y -= h;
 
// Si le Sprite est dans la phase ascendante du saut, on le laisse poursuivre
// Sinon, on le stoppe et on l'indique comme ??tant au sol.
if (dirY > 0)
{
dirY = 0;
onGround = 1;
}
 
wasOnSlope = isOnSlope;
 
return 1;
}
 
// Si on est sur le sol, on v??rifie si la Tile suivante, et en desssous, est une pente.
// Dans ce cas, on d??place le Sprite sur la pente,
else
{
 
if (goOnSlopeDown > 0)
{
double a, b;
if (!slopeEquation(goOnSlopeDown, &a, &b)){ return 0; }
int xPos = posEndX - xa*TILE_SIZE;
 
//Ici, xPos ??tant sur la Tile suivante, on retranche une Tile pour avoir le bon yPos
if (dirXmem > 0)
{
xPos -= TILE_SIZE;
}
else
{
xPos += TILE_SIZE;
}
 
int yPos = a * xPos + b;
if (yPos > 31) { yPos = 31; }
diagOffSet = TILE_SIZE - yPos;
yc = yb + 1;
wasOnSlope = isOnSlope;
checkWasOnSlope = 0;
}
 
// sinon on fait la transition en douceur avec "entity->wasOnSlope" ("checkWasOnSlope" restant ?? true)
}
 
}
 
// Finalement, si on est pas sur une pente, qu'on ne va pas sur une pente
// mais qu'on y ??tait le tour d'avant, on force une sortie en douceur
if (wasOnSlope > 0 && checkWasOnSlope)
{
// Si on quitte une mont??e
if ((dirXmem > 0 && wasOnSlope == TILE_PENTE_26_BenH_2) ||
(dirXmem < 0 && wasOnSlope == TILE_PENTE_26_HenB_1))
{
yc = ya;
}
 
// Si on quitte une descente
else
{
if ((dirXmem > 0 && wasOnSlope == TILE_PENTE_26_HenB_2) ||
(dirXmem < 0 && wasOnSlope == TILE_PENTE_26_BenH_1))
{
yc = ya + 1;
}
}
 
resetWasOnSlope = 1;
}
 
// Si on "est" ou si on "quitte" une pente (donc que wasOnSlope > 0)
if (wasOnSlope > 0)
{
// On calcul l'??cart entre le sol de la pente et la position du Sprite
// Si l'??cart est plus grand que la vitesse de chute, on continue de laisser tomber le Sprite
if (wasOnGround == 0)
{
int newPos = yc * TILE_SIZE + diagOffSet;
int ecart = newPos - posIniY;
 
if (ecart > dirYmem)
{
y = posYmem;
dirY = dirYmem;
onGround = 0;
return 0;
}
}
 
// On positionne le Sprite sur la pente
x = posXmem;
dirX = dirXmem;
y = yc * TILE_SIZE + diagOffSet;
y -= h;
dirY = 0;
onGround = 1;
 
// On n'oublie pas de remettre wasOnSlope ?? 0 si n??c??ssaire
if (resetWasOnSlope)
{
wasOnSlope = 0;
}
 
return 1;
}
 
return 0;
 
}
