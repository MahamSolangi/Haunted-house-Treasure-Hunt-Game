#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include <list>
#include <string>
#include <QTextEdit>

// Game entities
class Item {
public:
    std::string name;
    int power;
    Item(std::string name, int power);
};

class Inventory {
private:
    struct Node {
        Item item;
        Node* next;
        Node(Item item);
    };
    Node* head;
public:
    Inventory();
    void addItem(Item item);
    void showItems(QTextEdit* outputArea);
    bool hasItem(const std::string& itemName);
    void removeItem(const std::string& itemName);
};

class Enemy {
public:
    std::string name;
    int health;
    int attackPower;
    Enemy(std::string name, int health, int attackPower);
};

class Room {
public:
    std::string description;
    std::vector<Room*> adjacentRooms;
    std::list<Item> items;
    std::list<Enemy> enemies;
    Room(std::string description);
};

class DungeonGraph {
private:
    std::vector<Room*> rooms;
public:
    void addRoom(Room* room);
    void connectRooms(Room* room1, Room* room2);
    Room* getRoomByDescription(const std::string& desc);
    void visualizeDungeon(QTextEdit* outputArea);
};

class Player {
public:
    std::string name;
    int health;
    Inventory inventory;
    Player(std::string name, int health);
    void pickUpItem(Item item);
    void showInventory(QTextEdit* outputArea);
    void takeDamage(int damage, QTextEdit* outputArea);
    bool isAlive();
};

#endif // GAMELOGIC_H
