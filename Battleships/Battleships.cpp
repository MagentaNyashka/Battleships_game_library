#include "Battleships.h"

std::vector<class Ship*> LIST::shipList;

Ship::Ship(const std::string Name, const double& maxSpeed, const double& maxBodyPoints, const double& maxArmourPoints, const int& crewCount, const double& x, const double& y, const double& z, const double& armourHeal) :
    Name(Name), maxSpeed(maxSpeed), maxBodyPoints(maxBodyPoints), maxArmourPoints(maxArmourPoints), BodyPoints(maxBodyPoints), ArmourPoints(maxArmourPoints), crewCount(crewCount), shipCoordinates({ x,y,z }), armourHeal(armourHeal) {}

void Ship::Move(double& x, double& y, double& z) {
    if (x > maxSpeed) {
        x = maxSpeed;
    }
    if (y > maxSpeed) {
        y = maxSpeed;
    }
    if (z > maxSpeed) {
        z = maxSpeed;
    }
    shipCoordinates.x += x;
    shipCoordinates.y += y;
    shipCoordinates.z += z;
}

double Ship::getDistance(class Ship& target) const {
    return sqrt(pow(target.shipCoordinates.x - shipCoordinates.x, 2) + pow(target.shipCoordinates.y - shipCoordinates.y, 2) + pow(target.shipCoordinates.z - shipCoordinates.z, 2));
}

void Ship::shootTheTarget(class Ship& target) {
    double Distance = getDistance(target);
    for (auto weapon : Weapons) {
        weapon->shoot(Distance, target);
    }
}

void Ship::getHit(const double& damage) {
    if (ArmourPoints < damage) {
        BodyPoints -= damage - ArmourPoints;
        ArmourPoints = 0;
    }
    else
    {
        ArmourPoints -= damage;
    }
}

double Ship::getSpeed() {
    return maxSpeed;
}

double Ship::getBodyPoints() {
    return BodyPoints;
}

void Ship::Heal() {
    ArmourPoints += armourHeal;
    if (ArmourPoints > maxArmourPoints) {
        ArmourPoints = maxArmourPoints;
    }
}

std::string Ship::info() const {
    std::string bodyInfo = "[";
    for (int i = 0; i < ArmourPoints / maxArmourPoints * 10; i++) {
        bodyInfo += char(178);
    }
    bodyInfo += "]";
    std::string armourInfo = "[";
    for (int i = 0; i < BodyPoints / maxBodyPoints * 10; i++) {
        armourInfo += char(254);
    }
    armourInfo += "]";

    std::string coordinates = "{" + std::to_string(shipCoordinates.x) + " " + std::to_string(shipCoordinates.y) + " " + std::to_string(shipCoordinates.z) + "}";

    return " " + Name + ": " + bodyInfo + " " + armourInfo + " " + coordinates + " [" + std::to_string(BodyPoints) + "/" + std::to_string(maxBodyPoints) + "] " + " [" + std::to_string(ArmourPoints) + "/" + std::to_string(maxArmourPoints) + "]" + " MAX_SPEED: " + std::to_string(maxSpeed);
}

Ship::~Ship() {
    for (auto weapon : Weapons) {
        delete weapon;
    }
}

// Implementations of Weapon methods
Laser::Laser(const double& fireRate, const double& damage) : fireRate(fireRate), damage(damage) {}

bool Laser::isHitting(const double& Distance, class Ship& target) const {
    double hitProbability = 10000 / fireRate * (Distance * 0.3) / target.getSpeed();
    if (hitProbability > 1.0)
        hitProbability = 1.0;
    if (rand() % 100 < hitProbability * 100) {
        return true;
    }
    return false;
}

void Laser::shoot(const double& Distance, class Ship& target) {
    if (isHitting(Distance, target)) {
        target.getHit(damage);
    }
}

RocketLauncher::RocketLauncher(const int& rocketCount, const double& fireRate, const double& damage, const double& rocketSpeed) :
    rocketCount(rocketCount), fireRate(fireRate), damage(damage), rocketSpeed(rocketSpeed) {}

bool RocketLauncher::isHitting(const double& Distance, class Ship& target) const {
    if (rocketCount <= 0) {
        return false;
    }
    double hitProbability = 10000 / fireRate / (Distance / 20) * (rocketSpeed * 0.2) / target.getSpeed();
    if (hitProbability > 1.0)
        hitProbability = 1.0;
    if ((rand() % 100 < hitProbability * 100) || Distance == 0) {
        return true;
    }
    return false;
}

void RocketLauncher::shoot(const double& Distance, class Ship& target) {
    if (isHitting(Distance, target)) {
        rocketCount--;
        target.getHit(damage);
    }
}

Turret::Turret(const int& ammo, const double& fireRate, const double& damage, const double& bulletSpeed) : ammo(ammo), fireRate(fireRate), damage(damage), bulletSpeed(bulletSpeed) {}

bool Turret::isHitting(const double& Distance, class Ship& target) const {
    if (ammo <= 0) {
        return false;
    }
    double hitProbability = 100 / fireRate / (Distance / 20) * (bulletSpeed * 0.3) / target.getSpeed();
    if (hitProbability > 1.0)
        hitProbability = 1.0;
    if ((rand() % 100 < hitProbability * 100) || Distance == 0) {
        return true;
    }
    return false;
}

void Turret::shoot(const double& Distance, class Ship& target) {
    if (isHitting(Distance, target)) {
        target.getHit(damage * 3);
    }
    ammo -= 3;
}

NUKER::NUKER(const double& damage) : damage(damage) {}

bool NUKER::isHitting(const double& Distance, class Ship& target) const {
    return true;
}

void NUKER::shoot(const double& Distance, class Ship& target) {
    for (auto ship : LIST::shipList) {
        if (ship->getDistance(target) < 500) {
            ship->getHit(damage);
        }
    }
    target.getHit(damage);
}

// Implementations of Frigate, Armadillo, Destroyer, Battleship constructors
Frigate::Frigate() : Ship("Frigate", 100, 100, 100, 3, 100, 100, 100, 10) {
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Laser(20, 5));
}

Armadillo::Armadillo() : Ship("Armadillo", 25, 400, 800, 6, -100, -100, -100, 100) {
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
}

Destroyer::Destroyer() : Ship("Destroyer", 40, 200, 400, 7, 0, 0, 0, 50) {
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
}

Battleship::Battleship() : Ship("Cutie >///<", 500, 1000, 2000, 10, 100, -100, -100, 200) {
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Laser(20, 5));
    Weapons.push_back(new Laser(20, 5));
    Weapons.push_back(new Laser(20, 5));
    Weapons.push_back(new Laser(20, 5));
    Weapons.push_back(new NUKER(9999));
}

// Implementations of UI methods
void UI::listShips() {
    system("cls");
    for (int i = 0; i < LIST::shipList.size(); i++) {
        std::cout << "[" << i << "]" << LIST::shipList[i]->info() << std::endl;
    }
}

void UI::mainMenu() {
    int option = -1;
    int i = 0;
    do {
        system("cls");
        std::cout << "Choose your move: " << std::endl;
        std::cout << "1. Create a ship\n";
        std::cout << "2. Move a ship\n";
        std::cout << "3. Shoot a target\n";
        std::cout << "4. List ships\n";
        std::cout << "Option: ";
        std::cin >> option;
        switch (option)
        {
        case 1:
            createShip();
            break;
        case 2:
            moveShip();
            break;
        case 3:
            shootTarget();
            break;
        case 4:
            listShips();
            std::cout << "Back[0]: ";
            std::cin >> i;
        default:
            break;
        }
    } while (option != 0);
}

void UI::createShip() {
    system("cls");
    int option = -1;
    std::cout << "Choose a ship to create:" << std::endl;
    std::cout << "1. Frigate\n";
    std::cout << "2. Armadillo\n";
    std::cout << "3. Destroyer\n";
    std::cout << "Option: ";
    std::cin >> option;
    switch (option)
    {
    case 1:
        LIST::shipList.push_back(new Frigate);
        break;
    case 2:
        LIST::shipList.push_back(new Armadillo);
        break;
    case 3:
        LIST::shipList.push_back(new Destroyer);
        break;
    case 4:
        LIST::shipList.push_back(new Battleship);
        break;
    default:
        break;
    }
}

void UI::moveShip() {
    system("cls");
    int option = -1;
    listShips();
    std::cout << "Choose a ship: ";
    std::cin >> option;
    if (option < 0 || option >= LIST::shipList.size()) {
        std::cout << "Wrong ship" << std::endl;
    }
    else
    {
        double x, y, z;
        std::cout << "X Y Z" << std::endl;
        std::cin >> x >> y >> z;
        LIST::shipList[option]->Move(x, y, z);
    }
}

void UI::shootTarget() {
    system("cls");
    int option1 = -1;
    listShips();
    std::cout << "Choose a ship: ";
    std::cin >> option1;
    if (option1 < 0 || option1 >= LIST::shipList.size()) {
        std::cout << "Wrong ship" << std::endl;
    }
    else
    {
        system("cls");
        int option2 = -1;
        listShips();
        std::cout << "Choose a target: ";
        std::cin >> option2;
        if (option2 < 0 || option2 >= LIST::shipList.size()) {
            std::cout << "Wrong ship" << std::endl;
        }
        else
        {
            LIST::shipList[option1]->shootTheTarget(*LIST::shipList[option2]);
            UI::removeDestroyedShips();
        }
    }
}

void UI::removeDestroyedShips() {
    for (auto it = LIST::shipList.begin(); it != LIST::shipList.end();) {
        if ((*it)->getBodyPoints() <= 0) {
            delete* it;
            it = LIST::shipList.erase(it);
        }
        else {
            ++it;
        }
    }
}
