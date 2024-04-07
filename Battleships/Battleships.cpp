#include "Battleships.h"

std::vector<TEAM*> LIST::Teams;
int LIST::teamsCount;

TEAM::TEAM(const int& i) : credits(10), creditsPerTurnMultiplier(1), shieldRegenMultiplier(1), maxShips(4) {
    TEAM::shipList.push_back(new Frigate(i));
}

Ship::Ship(const std::string Name, const double& maxSpeed, const double& maxBodyPoints, const double& maxArmourPoints, const int& crewCount, const double& x, const double& y, const double& z, const double& armourHeal, const int& team) :
    Name(Name), maxSpeed(maxSpeed), maxBodyPoints(maxBodyPoints), maxArmourPoints(maxArmourPoints), BodyPoints(maxBodyPoints), ArmourPoints(maxArmourPoints), crewCount(crewCount), shipCoordinates({ x,y,z }), armourHeal(armourHeal), team(team) {}

void Ship::Move(double& x, double& y, double& z) {
    if (!madeMove) {
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
}

double Ship::getDistance(class Ship& target) const {
    return sqrt(pow(target.shipCoordinates.x - shipCoordinates.x, 2) + pow(target.shipCoordinates.y - shipCoordinates.y, 2) + pow(target.shipCoordinates.z - shipCoordinates.z, 2));
}

bool Ship::moveStatus() const {
    return madeMove;
}

void Ship::makeMove() {
    madeMove = true;
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
    std::string ifMadeMove = (madeMove) ? "[+] " : "[#] ";
    return " " + ifMadeMove + Name + ": " + bodyInfo + " " + armourInfo + " " + coordinates + " [" + std::to_string(BodyPoints) + "/" + std::to_string(maxBodyPoints) + "] " + " [" + std::to_string(ArmourPoints) + "/" + std::to_string(maxArmourPoints) + "]" + " MAX_SPEED: " + std::to_string(maxSpeed);
}

void Ship::refreshMove() {
    madeMove = false;
}

Ship::~Ship() {
    for (auto weapon : Weapons) {
        delete weapon;
    }
}

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
    for (TEAM* team : LIST::Teams) {
        for (Ship* ship : team->shipList) {
            if (ship->getDistance(target) < 500) {
                ship->getHit(damage);
            }
        }
    }
    target.getHit(damage);
}

Frigate::Frigate(const int& team) : Ship("Frigate", 100, 100, 100, 3, 100, 100, 100, 10, team) {
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Laser(20, 5));
}

Armadillo::Armadillo(const int& team) : Ship("Armadillo", 25, 400, 800, 6, -100, -100, -100, 100, team) {
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
    Weapons.push_back(new Turret(500, 10, 10, 100));
}

Destroyer::Destroyer(const int& team) : Ship("Destroyer", 40, 200, 400, 7, 0, 0, 0, 50, team) {
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
    Weapons.push_back(new RocketLauncher(100, 2, 50, 100));
}

Battleship::Battleship(const int& team) : Ship("Cutie >///<", 500, 1000, 2000, 10, 100, -100, -100, 200, team) {
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

void UI::listShips(const int& turn) {
    system("cls");
    for (int i = 0; i < LIST::Teams[turn]->shipList.size(); i++) {
        std::cout << "[" << i << "]" << LIST::Teams[turn]->shipList[i]->info() << std::endl;
    }
}

void UI::mainMenu() {
    std::cout << "How many teams(max 4)? ";
    std::cin >> LIST::teamsCount;
    assert(LIST::teamsCount <= 4 && LIST::teamsCount > 1);
    for (int i = 0; i < LIST::teamsCount; i++) {
        LIST::Teams.push_back(new TEAM(i));
    }
    bool isOver = false;
    int turn = RED;
    do {

        for (auto team : LIST::Teams) {
            if (team->shipList.size() == 0) {
                isOver = true;
            }
        }
        if (turn >= LIST::teamsCount) {
            turn = RED;
        }
        std::string current_team;
        switch (turn)
        {
        case RED:
            current_team = "RED";
            break;
        case GREEN:
            current_team = "GREEN";
            break;
        case BLUE:
            current_team = "BLUE";
            break;
        case YELLOW:
            current_team = "YELLOW";
            break;
        default:
            break;
        }
        int option = -1;
        int i = 0;
        do {
            system("cls");
            std::cout << current_team << " turn" << std::endl;
            std::cout << "Choose your move: " << std::endl;
            std::cout << "1. Create a ship\n";
            std::cout << "2. Move a ship\n";
            std::cout << "3. Shoot a target\n";
            std::cout << "4. LIST ships\n";
            std::cout << "5. Next turn\n";
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 1:
                createShip(turn);
                break;
            case 2:
                moveShip(turn);
                break;
            case 3:
                shootTarget(turn);
                break;
            case 4:
                listShips(turn);
                std::cout << "Back[0]: ";
                std::cin >> i;
                break;
            case 5:
                UI::refreshMoves();
                turn++;
                break;
            default:
                break;
            }
        } while (option != 0);
    } while (!isOver);
}

void UI::createShip(const int& turn) {
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
        LIST::Teams[turn]->shipList.push_back(new Frigate(turn));
        break;
    case 2:
        LIST::Teams[turn]->shipList.push_back(new Armadillo(turn));
        break;
    case 3:
        LIST::Teams[turn]->shipList.push_back(new Destroyer(turn));
        break;
    case 4:
        LIST::Teams[turn]->shipList.push_back(new Battleship(turn));
        break;
    default:
        break;
    }
}

void UI::moveShip(const int& turn) {
    system("cls");
    int option = -1;
    listShips(turn);
    std::cout << "Choose a ship: ";
    std::cin >> option;
    if (option < 0 || option >= LIST::Teams[turn]->shipList.size()) {
        std::cout << "Wrong ship" << std::endl;
    }
    else
    {
        double x, y, z;
        std::cout << "X Y Z" << std::endl;
        std::cin >> x >> y >> z;
        LIST::Teams[turn]->shipList[option]->Move(x, y, z);
        LIST::Teams[turn]->shipList[option]->makeMove();
    }
}

void UI::shootTarget(const int& turn) {
    system("cls");
    int option1 = -1;
    listShips(turn);
    std::cout << "Choose a ship: ";
    std::cin >> option1;
    system("cls");
     int team = -1;
     std::cout << "Choose a team: \n";
     for (int i = 0; i < LIST::Teams.size(); i++){
         if (i != turn) {
             switch (i)
             {
             case RED:
                 std::cout << "0. RED" << std::endl;
             case GREEN:
                 std::cout << "1. GREEN" << std::endl;
             case BLUE:
                 std::cout << "2. BLUE" << std::endl;
             case YELLOW:
                 std::cout << "3. YELLOW" << std::endl;
             default:
                 break;
             }
         }
     }
     std::cout << "Team: ";
     std::cin >> team;
     std::cout << "Choose a target: ";
     listShips(team);
     std::cout << "Target: ";
     int target = -1;
     std::cin >> target;
     LIST::Teams[turn]->shipList[option1]->shootTheTarget(*LIST::Teams[team]->shipList[target]);
     LIST::Teams[turn]->shipList[option1]->makeMove();
     UI::removeDestroyedShips();
}

void UI::removeDestroyedShips() {
    for (TEAM* team : LIST::Teams) {
        for (auto it = team->shipList.begin(); it != team->shipList.end();) {
            if ((*it)->getBodyPoints() <= 0) {
                delete* it;
                it = team->shipList.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void UI::refreshMoves() {
    for (TEAM* team : LIST::Teams) {
        for (Ship* ship : team->shipList) {
            ship->refreshMove();
        }
    }
}