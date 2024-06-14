#include "mainwindow.h"
#include <QInputDialog>
#include <QPixmap>
#include <QDebug>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set up the main window
    this->setWindowTitle("Dungeon Adventure");
    mapWindow = new QMainWindow();
    mapWindow->resize(800, 600);

    // Set up the background label
    backgroundLabel = new QLabel(this);
    QPixmap background("E:/Documents/treasurehunt/bg.jpg"); // Ensure the path is correct
    if (!background.isNull()) {
        background = background.scaled(this->size(), Qt::KeepAspectRatio);
        backgroundLabel->setPixmap(background);
    } else {
        qDebug() << "Failed to load background image.";
    }
    backgroundLabel->setScaledContents(true);

    // Set up the buttons
    startButton = new QPushButton("Start Game", this);
    moveButton = new QPushButton("Move", this);
    pickUpButton = new QPushButton("Pick Up Item", this);
    showInventoryButton = new QPushButton("Show Inventory", this);
    fightButton = new QPushButton("Fight", this);
    mapButton = new QPushButton("Map", this);
    closeMapButton = new QPushButton("Close Map", this); // Button to close the map
    closeMapButton->hide();

    // Set button style
    QString buttonStyle = "QPushButton { background-color: #555555; color: white; font: bold; }";
    startButton->setStyleSheet(buttonStyle);
    moveButton->setStyleSheet(buttonStyle);
    pickUpButton->setStyleSheet(buttonStyle);
    showInventoryButton->setStyleSheet(buttonStyle);
    fightButton->setStyleSheet(buttonStyle);
    mapButton->setStyleSheet(buttonStyle);
    closeMapButton->setStyleSheet(buttonStyle);

    moveButton->hide();
    pickUpButton->hide();
    showInventoryButton->hide();
    fightButton->hide();
    mapButton->hide();

    // Set up the output area
    outputArea = new QTextEdit(this);
    outputArea->setReadOnly(true);
    outputArea->setStyleSheet("QTextEdit { color: white; background-color: #333333; font-family: 'Monotype Corsiva'; font-size: 18pt; }");

    // Set up the input area
    inputArea = new QLineEdit(this);

    // Connect signals and slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(moveButton, &QPushButton::clicked, this, &MainWindow::movePlayer);
    connect(pickUpButton, &QPushButton::clicked, this, &MainWindow::pickUpItem);
    connect(showInventoryButton, &QPushButton::clicked, this, &MainWindow::showInventory);
    connect(fightButton, &QPushButton::clicked, this, &MainWindow::fightEnemy);
    connect(mapButton, &QPushButton::clicked, this, &MainWindow::displayMap);
    connect(closeMapButton, &QPushButton::clicked, this, &MainWindow::closeMap); // Connect closeMapButton to closeMap

    // Layout management
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(backgroundLabel);
    layout->addWidget(startButton);
    layout->addWidget(moveButton);
    layout->addWidget(pickUpButton);
    layout->addWidget(showInventoryButton);
    layout->addWidget(fightButton);
    layout->addWidget(mapButton);
    layout->addWidget(closeMapButton); // Add closeMapButton to the layout
    layout->addWidget(outputArea);
    layout->addWidget(inputArea);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    // Initialize game elements
    initializeGame();
}

MainWindow::~MainWindow() {
    delete player;
}
void MainWindow::initializeGame() {
    srand(time(0));
    player = new Player("Hero", 100);

    Room* entrance = new Room("Entrance Hall");
    Room* livingRoom = new Room("Living Room");
    Room* kitchen = new Room("Kitchen");
    Room* diningRoom = new Room("Dining Room");
    Room* basement = new Room("Basement");
    Room* attic = new Room("Attic");
    Room* secretRoom = new Room("Secret Room");
    Room* garden = new Room("Garden");
    Room* library = new Room("Library");
    Room* cellar = new Room("Cellar");

    hauntedHouse.addRoom(entrance);
    hauntedHouse.addRoom(livingRoom);
    hauntedHouse.addRoom(kitchen);
    hauntedHouse.addRoom(diningRoom);
    hauntedHouse.addRoom(basement);
    hauntedHouse.addRoom(attic);
    hauntedHouse.addRoom(secretRoom);
    hauntedHouse.addRoom(garden);
    hauntedHouse.addRoom(library);
    hauntedHouse.addRoom(cellar);

    hauntedHouse.connectRooms(entrance, livingRoom);
    hauntedHouse.connectRooms(livingRoom, kitchen);
    hauntedHouse.connectRooms(livingRoom, diningRoom);
    hauntedHouse.connectRooms(kitchen, basement);
    hauntedHouse.connectRooms(diningRoom, attic);
    hauntedHouse.connectRooms(attic, secretRoom);
    hauntedHouse.connectRooms(entrance, garden);
    hauntedHouse.connectRooms(garden, library);
    hauntedHouse.connectRooms(library, cellar);

    Item treasure("Treasure", 100);
    Item key("Key", 0);
    Item potion("Potion", 50);
    Item sword("Sword", 20);
    Item shield("Shield", 15);
    Item armor("Armor", 30);
    Enemy ghost("Ghost", 30, 5);
    Enemy zombie("Zombie", 40, 10);
    Enemy vampire("Vampire", 50, 15);

    entrance->items.push_back(potion);
    kitchen->items.push_back(key);
    kitchen->items.push_back(sword);
    basement->enemies.push_back(ghost);
    basement->enemies.push_back(zombie);
    attic->items.push_back(shield);
    attic->enemies.push_back(vampire);
    garden->items.push_back(armor);

    std::vector<Room*> potentialTreasureRooms = { livingRoom, kitchen, diningRoom, basement, attic, secretRoom, garden, library, cellar };
    treasureRoom = potentialTreasureRooms[rand() % potentialTreasureRooms.size()];
    treasureRoom->items.push_back(treasure);

    currentRoom = entrance;
}

void MainWindow::startGame() {
    outputArea->clear();
    outputArea->append("You are in the " + QString::fromStdString(currentRoom->description));

    startButton->hide();
    moveButton->show();
    pickUpButton->show();
    showInventoryButton->show();
    fightButton->show();
    mapButton->show();
}

void MainWindow::movePlayer() {
    outputArea->clear();
    outputArea->append("Where would you like to go?");
    for (size_t i = 0; i < currentRoom->adjacentRooms.size(); ++i) {
        outputArea->append(QString::number(i + 1) + ". " + QString::fromStdString(currentRoom->adjacentRooms[i]->description));
    }

    bool ok;
    int choice = QInputDialog::getInt(this, tr("Move"), tr("Choose a room:"), 1, 1, currentRoom->adjacentRooms.size(), 1, &ok);
    if (ok) {
        moveToRoom(currentRoom->adjacentRooms[choice - 1]);
    } else {
        outputArea->append("Invalid choice!");
    }
}

void MainWindow::moveToRoom(Room* targetRoom) {
    currentRoom = targetRoom;
    outputArea->clear();
    outputArea->append("You are in the " + QString::fromStdString(currentRoom->description));
}

void MainWindow::pickUpItem() {
    outputArea->clear();
    if (!currentRoom->items.empty()) {
        Item item = currentRoom->items.front();
        currentRoom->items.pop_front();
        player->pickUpItem(item);
        outputArea->append("You picked up: " + QString::fromStdString(item.name));
    } else {
        outputArea->append("No items to pick up!");
    }
}

void MainWindow::showInventory() {
    outputArea->clear();
    player->showInventory(outputArea);
}

void MainWindow::fightEnemy() {
    outputArea->clear();
    if (!currentRoom->enemies.empty()) {
        Enemy& enemy = currentRoom->enemies.front();
        outputArea->append("You are fighting: " + QString::fromStdString(enemy.name));
        while (enemy.health > 0 && player->isAlive()) {
            enemy.health -= 10;
            outputArea->append("You hit the " + QString::fromStdString(enemy.name) + " for 10 damage. Enemy health: " + QString::number(enemy.health));
            if (enemy.health > 0) {
                player->takeDamage(enemy.attackPower, outputArea);
                if (!player->isAlive()) {
                    outputArea->append("You have been defeated by the " + QString::fromStdString(enemy.name) + ". Game over.");
                    break;
                }
            }
        }
        if (enemy.health <= 0) {
            outputArea->append("You defeated the " + QString::fromStdString(enemy.name));
            currentRoom->enemies.pop_front();
        }
    } else {
        outputArea->append("No enemies to fight!");
    }
}


void MainWindow::closeMap() {
    if (mapWindow != nullptr) {
        mapWindow->close();
        delete mapWindow;
        mapWindow = nullptr;
    }
}


void MainWindow::displayMap() {
    if (mapWindow == nullptr) {
        mapWindow = new QMainWindow();
        mapWindow->setWindowTitle("Map");

        // Create a label to display the map image
        QLabel *mapLabel = new QLabel(mapWindow);
        QPixmap map("E:/Documents\treasurehunt/map.png"); // Ensure the path is correct
        if (!map.isNull()) {
            qDebug() << "Map image loaded successfully.";
            // Scale the map image to fit within the label
            map = map.scaled(400, 400, Qt::KeepAspectRatio);
            mapLabel->setPixmap(map);
        } else {
            qDebug() << "Failed to load map image.";
        }

        // Set the map label as the central widget of the map window
        mapWindow->setCentralWidget(mapLabel);
    }

    // Show the map window
    mapWindow->show();
}


