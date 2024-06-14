#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QApplication>
#include "gamelogic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void closeMap();
private slots:
    void startGame();
    void movePlayer();
    void pickUpItem();
    void showInventory();
    void fightEnemy();
    void displayMap();
    void moveToRoom(Room* targetRoom);

private:
    void initializeGame();
    QMainWindow *mapWindow = nullptr;
    QPushButton *startButton;
    QPushButton *moveButton;
    QPushButton *pickUpButton;
    QPushButton *showInventoryButton;
    QPushButton *fightButton;
    QPushButton *mapButton;
    QLabel *backgroundLabel;
    QTextEdit *outputArea;
    QLineEdit *inputArea;
    QPushButton *closeMapButton;
    Player *player;
    DungeonGraph hauntedHouse;
    Room *currentRoom;
    Room *treasureRoom;

};

#endif // MAINWINDOW_H
