#include "gameboard.h"

GameBoard::GameBoard()
{
    ef = new EntityFactory();
    board.setSizes(SIZE_X, SIZE_Y);

    initializeBoard();  //use initializeGame()/initializeGameRnd()
                        //for (non)random board initialization
}

GameBoard::~GameBoard()
{
    while(!board.empty()) delete *(board.end()-1), board.pop_back();
    delete ef;
    //hero is deleted in board
}

void GameBoard::initializeBoard()
{
    initializeEnvironment();
    initializeEntities();
}

void GameBoard::initializeBoardRnd()
{
    StaticOutputStream::getStream() << "Randomizing positions based on time... \n"
                 "This takes 20 seconds... \n"
                 "\n";
    initializeEnvironmentRnd();
    initializeEntitiesRnd();
}

void GameBoard::initializeEnvironment(){
    initializeBoardBase();

    board.at(2, 2) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(3, 2) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(8, 4) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(1, 5) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(2, 5) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(3, 5) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(4, 6) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(5, 6) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(6, 5) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(5, 3) = new GameField(ef->getNewEnvironment(Environment::Tree));
    board.at(4, 3) = new GameField(ef->getNewEnvironment(Environment::Tree));
}

void GameBoard::initializeEnvironmentRnd()
{
    initializeBoardBase();

    Environment *tempTree;

    for (unsigned int i=0; i<10; i++){
        Position* pos = getNewRandomFreeBoardPosition();
        tempTree = ef->getNewEnvironment(Environment::Tree);
        board.at(pos->x, pos->y) = new GameField(tempTree);
    }
}

void GameBoard::initializeEntities(){
    Position* heroPos = new Position(1,1);
    hero = ef->getNewHero(heroPos);

    setFieldActorAt(hero, heroPos);
    setFieldActorAt(ef->getNewMonster(0), new Position(4,4));
    setFieldActorAt(ef->getNewMonster(1), new Position(3,6));
    setFieldActorAt(ef->getNewMonster(0), new Position(5,7));
    setFieldActorAt(ef->getNewPotion(), new Position(5,5));
    setFieldActorAt(ef->getNewPotion(), new Position(6,8));
}

void GameBoard::initializeEntitiesRnd()
{
    Position* heroPos = getNewRandomFreeBoardPosition();
    hero = ef->getNewHero(heroPos);

    setFieldActorAt(hero, heroPos);
    setFieldActorAt(ef->getNewMonster(0), getNewRandomFreeBoardPosition());
    setFieldActorAt(ef->getNewMonster(1), getNewRandomFreeBoardPosition());
    setFieldActorAt(ef->getNewMonster(0), getNewRandomFreeBoardPosition());
    setFieldActorAt(ef->getNewPotion(), getNewRandomFreeBoardPosition());
    setFieldActorAt(ef->getNewPotion(), getNewRandomFreeBoardPosition());
}

void GameBoard::initializeBoardBase()
{
    Environment *tempEmpty;
    Environment *tempTree;

    for (unsigned int i=0; i<SIZE_X; i++){
        for (unsigned int j=0; j<SIZE_Y; j++){
            tempEmpty = ef->getNewEnvironment(Environment::Empty);
            board.push_back(new GameField(tempEmpty));
        }
    }

    for (unsigned int i=0; i<SIZE_X; i++){
        unsigned int j=0;
        tempTree = ef->getNewEnvironment(Environment::Tree);
        board.at(i, j) = new GameField(tempTree);

        j = SIZE_Y-1;
        tempTree = ef->getNewEnvironment(Environment::Tree);
        board.at(i, j) = new GameField(tempTree);
    }

    for (unsigned int j=1; j<SIZE_Y-1; j++){
        unsigned int i=0;
        tempTree = ef->getNewEnvironment(Environment::Tree);
        board.at(i, j) = new GameField(tempTree);

        i = SIZE_X-1;
        tempTree = ef->getNewEnvironment(Environment::Tree);
        board.at(i, j) = new GameField(tempTree);
    }
}

bool GameBoard::monstersDead() const{
    if (Monster::getMonsterCount() == 0) return true;
    return false;
}

void GameBoard::printBoard() const{
    StaticOutputStream::getStream() << std::endl;
    for (unsigned int i=0; i<SIZE_X; i++){
        for (unsigned int j=0; j<SIZE_Y; j++){
            StaticOutputStream::getStream() << std::left << std::setw(2)
                      << board.at(i, j)->getPrintSign();
        }
        StaticOutputStream::getStream() << std::endl;
    }
    StaticOutputStream::getStream() << std::endl;
}

bool GameBoard::saveBoard() throw(file_error){
    std::ofstream out("map.txt");
    if(out.is_open()){

        //std::for_each(board.begin(), board.end(), [out](GameField const& item){ out << item.getPrintSign(); });
        auto it = board.begin();
        int i = 0;
        while(it!=board.end()){
            out << (*it)->getPrintSign();    //double dereference
            ++it;
            if (i%10 == 9) out << std::endl;
            i++;
        }
        out.close();
        return true;
    } else {
        throw file_error("File is not open");
        return false;
    }
}

void GameBoard::loadBoard() const throw(file_error){
    std::ifstream in("map.txt");
    if(in.is_open()){
        StaticOutputStream::getStream() << std::endl;
        std::string line = "";

        while (std::getline(in, line)) {
            StaticOutputStream::getStream() << std::endl << line;
        }
        StaticOutputStream::getStream() << std::endl;
    } else {
        throw file_error("File is not open");
    }
    in.close();
}

void GameBoard::moveHero(Position *toPos){
    if (freeFieldAt(toPos)){
        setFieldActorAt(nullptr, hero->getPosition());
        hero->setPosition(toPos);
        setFieldActorAt(hero, toPos);
        hero->resetSurpriseHit();
    }
}

Position *GameBoard::getNewRandomFreeBoardPosition() const
{
    Position *pos = new Position;
    do {
        *pos = Position::getNewRandomPosition(1, SIZE_X-1, 1, SIZE_Y-1);
    } while (!freeFieldAt(pos));
    return pos;
}

bool GameBoard::freeFieldAt(Position *atPos) const
{
    bool a = actorEmptyPosition(atPos) && passableEnvironmentAt(atPos);
    return a;
}

bool GameBoard::actorEmptyPosition(Position *pos) const{
    bool a = getActorAt(pos) == nullptr;
    return a;
}

bool GameBoard::passableEnvironmentAt(Position *pos) const
{
    bool a = board.at(pos)->getFieldEnvironment()->passableEnvironment();
    return a;
}

Hero *GameBoard::getHero() const{
    return hero;
}

GameField *GameBoard::getFieldAt(Position *atPos) const{
    return board.at(atPos);
}

FieldActor *GameBoard::getActorAt(Position *atPos) const
{
    return board.at(atPos)->getFieldActor();
}

Environment::fieldType GameBoard::getEnvTypeAt(Position *atPos) const{
    return board.at(atPos)->getFieldEnvironment()->getType();
}

void GameBoard::setFieldActorAt(FieldActor *toActor, Position* toPos){
    board.at(toPos)->setFieldActor(toActor);
}

void GameBoard::deleteActorAt(Position *atPos){
    FieldActor *actorAtPos = getFieldAt(atPos)->getFieldActor();
    setFieldActorAt(nullptr, atPos);

    Creature *creatureAtPos = dynamic_cast<Creature *>(actorAtPos);
    if (creatureAtPos != nullptr)
        getFieldAt(atPos)->setFieldEnvironment(ef->getNewEnvironment(Environment::Corpse));

    delete actorAtPos;
}

void GameBoard::killActorAt(Position *at)
{
    board.at(at)->getFieldActor()->die();
    deleteActorAt(at);
}




