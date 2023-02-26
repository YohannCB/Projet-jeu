#ifndef DEF_PERSONNAGE
#define DEF_PERSONNAGE
#include <string>
#include <iostream>

class Personnage
{
public:

  Personnage();

  void Action();
  
  void ChoixEnemy();

  void Combat(int enemy, int classe);
  
  void recevoirDegats(int nbDegats);

  void attaquer(Personnage &cible, int nbDegats);

  bool estVivant();

  //protected:

  int vie;
  int vitesse;
  int force;
  int agility;
  int banshee;
};

class Sasuke : public Personnage
{
public:
  Sasuke();
};

class Naruto : public Personnage
{
public:
  Naruto();
};


//class Combat : public Personnage
//{
//public:
//Combat(Personnage &cbt1, Personnage &cbt2);
//};
#endif
