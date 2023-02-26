#include <string>

class Personnage
{
public:

  Personnage();

  void Action();
  
  void ChoixEnemy();
  
  void Presentation() const;

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

class Garde : public Personnage
{
public:
  Garde(std::string name);//std::string name);
  void Presentation() const;
};

class Gardelourd : public Personnage
{
public: Gardelourd(std::string name);
  void Presentation() const;
};

class Assassin : public Personnage
{
public:
  Assassin(std::string name);
  void Presentation() const;
};

class Cible : public Personnage
{
public:
  Cible(std::string name);
  void Presentation() const;
};

//class Combat : public Personnage
//{
//public:
//Combat(Personnage &cbt1, Personnage &cbt2);
//};
