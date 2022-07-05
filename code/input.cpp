 
#include "input.hpp"
 
using namespace std;
using namespace sf;
 
//Constructeur
Input::Input()
{
button.left = button.right = button.up = button.down = button.jump =
button.attack = button.enter = false;
}
 
 
//Accesseur
Input::Button Input::getButton() const { return button; }
 
 
//Mutateur
void Input::setButton(int bouton, bool etat)
{
    switch (bouton)
    {
        case up:
            button.up = etat;
            break;
 
        case down:
            button.down = etat;
            break;
 
        case right:
            button.right = etat;
            break;
 
        case left:
            button.left = etat;
            break;
 
        case attack:
            button.attack = etat;
            break;
 
        case jump:  
            button.jump = etat;
            break;
 
        case enter:
            button.enter = etat;
            break;
    }
}
 
 
//Fonctions
void Input::gestionInputs(RenderWindow &window)
{
//Pour l'instant, on ne gère que le clavier.
//On gèrera les joysticks plus tard, s'il y en a
//un de branché.
//Pour l'instant, cette fonction n'est donc pas indispensable.
    getInput(window);
}
 
 
void Input::getInput(RenderWindow &window)
{
 
// Tant qu'il y a des évènements à traiter...
    while (window.pollEvent(event))
    {
// on regarde le type de l'évènement...
        switch (event.type)
        {
// On ferme la fenêtre
            case Event::Closed :
                window.close();
                break;
 
// Touche pressée
            case Event::KeyPressed:
                switch (event.key.code) // La touche qui a été pressée
                {
                    case Keyboard::Escape: // Echap
                        window.close();
                        break;
 
                    case Keyboard::J:
                        button.jump = true;
                        break;
 
                    case Keyboard::A:
                        button.attack = true;
                        break;
 
                    case Keyboard::Left:
                        button.left = true;
                        break;
 
                    case Keyboard::Right:
                        button.right = true;
                        break;
 
                    case Keyboard::Down:
                        button.down = true;
                        break;
 
                    case Keyboard::Up:
                        button.up = true;
                        break;
 
 
                    case Keyboard::Return:
                        button.enter = true;
                        break;
 
                    default:
                        break;
                    }   
                break;
 
// Touche relâchée
            case Event::KeyReleased:
                switch (event.key.code)
                {
                    case Keyboard::J:
                        button.jump = false;
                        break;
 
                    case Keyboard::Left:
                        button.left = false;
                        break;
 
                    case Keyboard::Right:
                        button.right = false;
                        break;
 
                    case Keyboard::Down:
                        button.down = false;
                        break;
 
                    case Keyboard::Up:
                        button.up = false;
                        break;
 
 
                    case Keyboard::Return:
                        button.enter = false;
                        break;
 
                    default:
                        break;
                    }
            break;
 
// on ne traite pas les autres types d'évènements
            default:
            break;
        }
    }
}