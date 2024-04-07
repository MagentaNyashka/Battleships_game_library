#pragma once


#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>

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
public:
    Ship(const std::string Name, const double& maxSpeed, const double& maxBodyPoints, const double& maxArmourPoints, const int& crewCount, const double& x, const double& y, const double& z, const double& armourHeal);
    void Move(double& x, double& y, double& z);
    double getDistance(class Ship& target) const;
    void shootTheTarget(class Ship& target);
    void getHit(const double& damage);
    double getSpeed();
    double getBodyPoints();
    void Heal();
    std::string info() const;
    ~Ship();
};

class LIST {
public:
    static std::vector<class Ship*> shipList;
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
    Frigate();
};

class Armadillo : public Ship {
public:
    Armadillo();
};

class Destroyer : public Ship {
public:
    Destroyer();
};

class Battleship : public Ship {
public:
    Battleship();
};

class UI {
public:
    static void listShips();
    static void mainMenu();
    static void createShip();
    static void moveShip();
    static void shootTarget();
    static void removeDestroyedShips();
};

#endif // BATTLESHIPS_H
