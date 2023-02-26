#include "Personnage.hpp"
#include <string>
#include <iostream>

Assassin::Assassin(std::string name)
{
    vitesse = 3;
    vie = 100;
    agility = 1;
    banshee = 1;
  
}

void Assassin::Presentation() const
{
  //Affiche à la personne qu'elle est assassin.
}
