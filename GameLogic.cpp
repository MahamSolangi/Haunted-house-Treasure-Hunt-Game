#include "gamelogic.h"

// Item
Item::Item(std::string name, int power) : name(name), power(power) {}

// Inventory::Node
Inventory::Node::Node(Item item) : item(item), next(nullptr) {}

// Inventory
Inventory::Inventory() : head(nullptr) {}

void Inventory::addItem(Item item) {
    Node* newNode = new Node(item);
    newNode->next = head;
    head = newNode;
}

void Inventory::showItems(QTextEdit* outputArea) {
    Node* current = head;
    while (current) {
        outputArea->append("Item: " + QString::fromStdString(current->item.name) + ", Power: " + QString::number(current->item.power));
        current = current->next;
    }
}

bool Inventory::hasItem(const std::string& itemName) {
    Node* current = head;
    while (current) {
        if (current->item.name == itemName) return true;
        current = current->next;
    }
    return false;
}

void Inventory::removeItem(const std::string& itemName) {
    Node* current = head;
    Node* previous = nullptr;
    while (current) {
        if (current->item.name == itemName) {
            if (previous) {
                previous->next = current->next;
            } else {
                head = current->next;
            }
            delete current;
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Enemy
Enemy::Enemy(std::string name, int health, int attackPower) : name(name), health(health), attackPower(attackPower) {}

// Room
Room::Room(std::string description) : description(description) {}

// DungeonGraph
void DungeonGraph::addRoom(Room* room) {
    rooms.push_back(room);
}

void DungeonGraph::connectRooms(Room* room1, Room* room2) {
    room1->adjacentRooms.push_back(room2);
    room2->adjacentRooms.push_back(room1);
}

Room* DungeonGraph::getRoomByDescription(const std::string& desc) {
    for (Room* room : rooms) {
        if (room->description == desc) {
            return room;
        }
    }
    return nullptr;
}

void DungeonGraph::visualizeDungeon(QTextEdit* outputArea) {
    outputArea->append("Dungeon Map:");
    for (Room* room : rooms) {
        outputArea->append("[" + QString::fromStdString(room->description) + "]");
        for (Room* adjacent : room->adjacentRooms) {
            outputArea->append(" -- " + QString::fromStdString(adjacent->description));
        }
    }
}

// Player
Player::Player(std::string name, int health) : name(name), health(health) {}

void Player::pickUpItem(Item item) {
    inventory.addItem(item);
}

void Player::showInventory(QTextEdit* outputArea) {
    inventory.showItems(outputArea);
}

void Player::takeDamage(int damage, QTextEdit* outputArea) {
    health -= damage;
    outputArea->append("Player took " + QString::number(damage) + " damage. Health is now: " + QString::number(health));
}

bool Player::isAlive() {
    return health > 0;
}
