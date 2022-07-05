#ifndef INPUT_HPP
#define INPUT_HPP
 
#include <SFML/Graphics.hpp>
 
 
class Input
{
 
//Structures
struct Button { 
    bool left;
    bool right; 
    bool up;
    bool down; 
    bool jump;
    bool attack; 
    bool enter; 
};
 
public:
 
//Constructeur
    Input();
 
//Accesseur
    Button getButton() const;
 
//Mutateur
    void setButton(int bouton, bool etat);
 
//méthodes de gestion des entrées
    void gestionInputs(sf::RenderWindow &window);
    void getInput(sf::RenderWindow &window);
 
 
private:
 
//Variables de la classe en accès privé
    sf::Event event;
    Button button;
 
//Enum pour les boutons
    enum{ up, down, right, left, attack, jump, enter };
 
};
#endif 