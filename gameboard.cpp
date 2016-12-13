#include "gameboard.h"

//for implementation of monstersDead with vector
//Monster* GameBoard::left = nullptr;

GameBoard::GameBoard()
{
    Environment *empty = new Environment(".", Environment::Empty);
    Environment *tree = new Environment("T", Environment::Tree);
    Position *pos;

    for (int i=0; i<sizeX; i++){
        std::vector<GameField *> row;
        for (int j=0; j<sizeY; j++){
            pos = new Position(i,j);
            row.push_back(new GameField(nullptr, empty, pos));
        }
        board.push_back(row);
    }

    for (int i=0; i<sizeX; i++){
        int j=0;
        pos = new Position(i,j);
        board.at(i).at(j)= new GameField(nullptr, tree, pos);

        j=sizeY-1;
        pos = new Position(i,j);
        board.at(i).at(j)= new GameField(nullptr, tree, pos);
    }

    for (int j=1; j<sizeY-1; j++){
        int i=0;
        pos = new Position(i,j);
        board.at(i).at(j)= new GameField(nullptr, tree, pos);

        i=sizeX-1;
        pos = new Position(i,j);
        board.at(i).at(j)= new GameField(nullptr, tree, pos);
    }

    Position *heroPos = new Position(1,1);
    hero = new Hero("The hero", "H", 100, 20, 15, true, heroPos);
    Monster *monster1 = new Monster("A not too scary monster", "M", 100, 15, 1.1);
    Monster *monster2 = new Monster("A scarier monster", "N", 100, 15, 1.5);
    /*monsters.push_back(monster1);
    monsters.push_back(monster2);*/ //for implementation of monstersDead with vector
    Potion *potion = new Potion("P", 50);



    setFieldEntityAt(heroPos, hero);
    setFieldEntityAt(new Position(5,5), potion);
    setFieldEntityAt(new Position(4,4), monster1);
    setFieldEntityAt(new Position(3,6), monster2);
}

GameBoard::~GameBoard(){/*
    for (int i=0; i<sizeX; i++){
        for (int j=0; j<sizeY; j++){
            pos = new Position(i,j);
            row.push_back(new GameField(nullptr, empty, pos));
        }
        board.push_back(row);
    }*/
    //delete the env etc
}

Hero *GameBoard::getHero(){
    return hero;
}

GameField *GameBoard::getFieldAt(Position *atPos){
    return board.at(atPos->x).at(atPos->y);
}

Environment::fieldType GameBoard::getEnvTypeAt(Position *atPos){
    return board.at(atPos->x).at(atPos->y)->getFieldEnvironment()->getType();
}

void GameBoard::setFieldEntityAt(Position *atPos, Entity *toEntity){
    board.at(atPos->x).at(atPos->y)->setFieldEntity(toEntity);
}

void GameBoard::moveHero(Position *toPos){
    board.at(hero->getPosition()->x).at(hero->getPosition()->y)->setFieldEntity(nullptr);
    board.at(toPos->x).at(toPos->y)->setFieldEntity(hero);
    hero->setPosition(toPos);
    hero->resetSurpriseHit();
}
                              //for (string &s : stringVec) cout << s << " ";
void GameBoard::printBoard(){ //vector<>::size_type; !=.size()
    std::cout << std::endl;
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            std::cout << std::left << std::setw(2) <<
                board.at(i).at(j)->getPrintSign();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameBoard::deleteEntityAt(Position *atPos){
    Entity *entityAtPos = board.at(atPos->x).at(atPos->y)->getFieldEntity();
    setFieldEntityAt(atPos, nullptr);

    Creature *creatureAtPos = dynamic_cast<Creature *>(entityAtPos);

    if (creatureAtPos != nullptr)
        getFieldAt(atPos)->setFieldEnvironment(new Environment("X", Environment::Corpse));

        //for implementation of monstersDead with vector
    /*left = dynamic_cast<Monster *>(entityAtPos);
    if (left != nullptr){
        std::vector<Monster *>::iterator toDeleteIterator =
            std::find_if(monsters.begin(), monsters.end(), monsterComparison);
        monsters.erase(toDeleteIterator);
    }*/

    delete entityAtPos;
}

bool GameBoard::monstersDead() const{
    if (Monster::getMonsterCount() == 0) return true;
    //if (monsters.empty()) return true;        //for implementation of monstersDead with vector
    return false;
}

    //for implementation of monstersDead with vector
/*bool GameBoard::monsterComparison(Monster* right){
    if (left->getName()==right->getName() && GameBoard::left->getHealth()==right->getHealth()) return true;
    return false;
}*/

