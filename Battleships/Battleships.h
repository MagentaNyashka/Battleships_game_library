#pragma once

#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <cassert>

enum teams
{
    RED = 0, GREEN, BLUE, YELLOW
};

class Weapon {
public:
    virtual void shoot(const double& Distance, class Ship& target) = 0;
    virtual bool isHitting(const double& Distance, class Ship& target) const = 0;
};

class Ship {
protected:
    std::string Name;
    double maxSpeed;
    double maxBodyPoints;
    double maxArmourPoints;
    double BodyPoints;
    double ArmourPoints;
    int crewCount;
    struct Coordinates
    {
        double x;
        double y;
        double z;
    };
    Coordinates shipCoordinates;
    std::vector<Weapon*> Weapons;
    double armourHeal;
    bool madeMove;
    int team;
public:
    Ship(const std::string Name, const double& maxSpeed, const double& maxBodyPoints, const double& maxArmourPoints, const int& crewCount, const double& armourHeal, const int& team);
    void Move(double& x, double& y, double& z);
    double getDistance(class Ship& target) const;
    void shootTheTarget(class Ship& target);
    void getHit(const double& damage);
    double getSpeed();
    double getBodyPoints();
    void Heal();
    std::string info() const;
    void refreshMove();
    bool moveStatus() const;
    void makeMove();
    ~Ship();
};

class TEAM;

class LIST {
public:
    static std::vector<TEAM*> Teams;
    static int teamsCount;
};

class TEAM {
private:
    double credits;
    double creditsPerTurnMultiplier;
    double shieldRegenMultiplier;
    int maxShips;
public:
    TEAM(const int& i);
    std::vector<Ship*> shipList;
};

class Laser : public Weapon {
private:
    double fireRate;
    double damage;
public:
    Laser(const double& fireRate, const double& damage);
    bool isHitting(const double& Distance, class Ship& target) const override;
    void shoot(const double& Distance, class Ship& target) override;
};

class RocketLauncher : public Weapon {
private:
    int rocketCount;
    double fireRate;
    double damage;
    double rocketSpeed;
public:
    RocketLauncher(const int& rocketCount, const double& fireRate, const double& damage, const double& rocketSpeed);
    bool isHitting(const double& Distance, class Ship& target) const override;
    void shoot(const double& Distance, class Ship& target) override;
};

class Turret : public Weapon {
private:
    int ammo;
    double fireRate;
    double damage;
    double bulletSpeed;
public:
    Turret(const int& ammo, const double& fireRate, const double& damage, const double& bulletSpeed);
    bool isHitting(const double& Distance, class Ship& target) const override;
    void shoot(const double& Distance, class Ship& target) override;
};

class NUKER : public Weapon {
private:
    double damage;
public:
    NUKER(const double& damage);
    bool isHitting(const double& Distance, class Ship& target) const override;
    void shoot(const double& Distance, class Ship& target) override;
};

class Frigate : public Ship {
public:
    Frigate(const int& team);
};

class Armadillo : public Ship {
public:
    Armadillo(const int& team);
};

class Destroyer : public Ship {
public:
    Destroyer(const int& team);
};

class Battleship : public Ship {
public:
    Battleship(const int& team);
};

class UI {
public:
    static void listShips(const int& turn);
    static void mainMenu();
    static void createShip(const int& turn);
    static void moveShip(const int& turn);
    static void shootTarget(const int& turn);
    static void removeDestroyedShips();
    static void refreshMoves();
};

#endif // BATTLESHIPS_H
