#ifndef Creature_hpp
#define Creature_hpp

#include "Weapon.hpp"
#include "Armour.hpp"
#include "BaseStats.hpp"
#include "Inventory.hpp"
#include <string>
#include <vector>
#include <cmath>
#include <array>
#include <unistd.h>
using namespace std;

const array<string, 1> actions = {
"ATTACK",
};

enum class TurnEffect {
  FLEE,
  NONE,
  CONTINUE,
};

class Creature : public BaseStats
{

protected:
  //Inventory inventory;

private:
  double maxHealth;
  double currentHealth;
  double baseDamage;
  Weapon equippedWeapon;
  Armour equippedArmour;

public:
  Inventory inventory;

  Creature(BaseStats stats = BaseStats(), Weapon equippedWeapon = Weapon(), Armour equippedArmour = Armour(), double maxHealth = 100, double baseDamage = 10, Inventory inventory = Inventory()) : BaseStats(stats), equippedWeapon(equippedWeapon), equippedArmour(equippedArmour), maxHealth(maxHealth), baseDamage(baseDamage), currentHealth(maxHealth), inventory(inventory) {}

  double getDamage()
  {
    return this->equippedWeapon.getDamage() + this->baseDamage;
  }

  vector<string> getAvailableActions() {
    vector<string> actions = {"ATTACK", "FLEE"};
    if (this->inventory.size() > 0)
      actions.push_back("ITEM");
    return actions;
  }

  double getCurrentHealth()
  {
    return this->currentHealth;
  }

  double getCriticalChance()
  {
    double cc = this->criticalChance + this->equippedWeapon.getCriticalChance();
    if (cc > 100)
    {
      cc = 100;
    }
    return cc;
  }

  double getCriticalDamage()
  {
    return this->criticalDamage + this->equippedWeapon.getCriticalDamage();
  }

  double getAccuracy()
  {
    return (this->equippedArmour.getAccuracy() + this->equippedWeapon.getAccuracy() + this->accuracy) / 3;
  }

  double getDealtDamage(double damage)
  {
    damage *= (1-(this->equippedArmour.getDefence()/100));
    if (damage < 1)
      damage = 1;
    return damage;
  }

  void equipWeapon(Weapon w)
  {
    this->equippedWeapon = w;
  }

  void equipArmour(Armour a)
  {
    this->equippedArmour = a;
  }

  bool isHit()
  {
    double target = rand() % 100 + 1;
    if (this->getAccuracy() < target)
    {
      return false;
    }
    return true;
  }

  void takeDamage(double damage)
  {
    damage = this->getDealtDamage(damage);
    this->currentHealth -= damage;
    if (this->currentHealth <= 0)
    {
      cout << this->getName() << " has died." << endl;
      this->currentHealth = 0;
      sleep(1);
    }
  }

  bool isCritical()
  {
    srand(time(NULL));
    int target = rand() % 100 + 1;
    if (this->getCriticalChance() < target)
    {
      return false;
    }
    return true;
  }

  void giveHealth(double amt) {
    double oldHealth = this->currentHealth;
    this->currentHealth += amt;
    if (this->currentHealth > this->maxHealth)
      this->currentHealth = this->maxHealth;
    double healed = this->currentHealth - oldHealth;
    cout << this->getName() << " healed " << healed << "HP" << endl;
    sleep(1);
  }

  void attack(Creature &victim)
  {
    if (this->isHit())
    {
      double damageDealt;
      if (this->isCritical())
      {
        damageDealt = this->getDamage() * (this->getCriticalDamage() / 100 + 1);
        cout << this->getName() << " landed a critical hit on " << victim.getName() << "!" << endl;
      }
      else
      {
        damageDealt = this->getDamage();
      }
      cout << this->getName() << " dealt " << victim.getDealtDamage(damageDealt) << " damage to " << victim.getName() << endl;
      victim.takeDamage(damageDealt);
    }
    else
    {
      cout << this->getName() << " attempted to attack " << victim.getName() << ", but missed." << endl;
    }
    sleep(1);
  }

  void printInfo()
  {
    cout << "Name: " << this->getName() << endl;
    cout << "HP ";
    int redSquares = ceil(this->getCurrentHealth() / this->maxHealth * 10);
    int blackSquares = 10 - redSquares;
    for (int i = 0; i < redSquares; i++) {
       cout << "????"; 
    }
    for (int i = 0; i < blackSquares; i++) {
      cout << "???";
    }
    cout << endl;
    cout << "Health: " << this->getCurrentHealth() << " / " << this->getMaxHealth() << endl;
    cout << "Damage: " << this->getDamage() << endl;
    cout << "Defence: " << this->equippedArmour.getDefence() << endl;
    if (this->equippedWeapon.getName() != "None")
    {
      cout << "Weapon: " << this->equippedWeapon.getName() << endl;
    }
    if (this->equippedArmour.getName() != "None") {
      cout << "Armour: " << this->equippedArmour.getName() << endl;
    }
    cout << "Critical Chance: " << this->getCriticalChance() << "%" << endl;
    cout << "Critical Damage: " << this->getCriticalDamage() << "%" << endl;
    cout << "Accuracy: " << this->getAccuracy() << "%" << endl;
    cout << endl;
    //this->inventory.printInventory();
 }

  bool isDead() {
    return this->currentHealth == 0;
  }

  // get health percentage (current/max)
  // multiply it by 10
  // ceil it
  // thats the number of red squares

  void useItem(string name) {
    cout << this->getName() << " used a " << name << "." << endl;
    sleep(1);
    if (name == "Health Potion") {
      this->giveHealth(40);
    }
    this->inventory.removeItem(name);
  }

  double getMaxHealth() {
    return this->maxHealth;
  }

};

#endif