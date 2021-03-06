#ifndef POSITION_H
#define POSITION_H

#include <limits>
#include <cstdlib>
#include <ctime>

/*!
 * \brief The Position struct
 *      This struct is a container for handling positioning on the GameBoard.
 *
 * It is extended with additional functionality over simple containers.
 * It can return instances of itself based on direction or randomly.
 */
struct Position
{
    unsigned int x;
    unsigned int y;
    //!
    //! \brief The direction enum
    //!     A type used to generate new Positions with direction parameter.
    //!
    enum direction{Up, Down, Left, Right, None};

    Position() : x(std::numeric_limits<int>::max()), y(std::numeric_limits<int>::max()){}

    Position(unsigned int xVal, unsigned int yVal) : x(xVal), y(yVal) {}

    static Position* getNewPosition(unsigned int x, unsigned int y){
        return new Position(x, y);
    }

    static Position getNewPositionInDirection(Position* currPos, char direct){
        Position targetPos;
        switch (direct) {
        case 'W':
            targetPos = Position(currPos->x-1, currPos->y);
            break;
        case 'S':
            targetPos = Position(currPos->x+1, currPos->y);
            break;
        case 'A':
            targetPos = Position(currPos->x, currPos->y-1);
            break;
        case 'D':
            targetPos = Position(currPos->x, currPos->y+1);
            break;
        default:
            targetPos = *currPos;
            sos::iout << "Invalid direction, returning currPos." << std::endl;
            break;
        }
        return targetPos;
    }

    static Position getNewPositionInDirection(Position* currPos, direction direct){
        Position targetPos;
        switch (direct) {
        case Up:
            targetPos = Position(currPos->x-1, currPos->y);
            break;
        case Down:
            targetPos = Position(currPos->x+1, currPos->y);
            break;
        case Left:
            targetPos = Position(currPos->x, currPos->y-1);
            break;
        case Right:
            targetPos = Position(currPos->x, currPos->y+1);
            break;
        default:
            targetPos = *currPos;
            sos::iout << "Invalid direction, returning currPos." << std::endl;
            break;
        }
        return targetPos;
    }

    /*!
     * \brief getNewRandomPosition
     *      This method returns an instance of Position of which the coordinates are randomized by time.
     * \param minX range parameter
     * \param maxX range parameter
     * \param minY range parameter
     * \param maxY range parameter
     * \return the instance
     */
    static Position getNewRandomPosition(unsigned int minX, unsigned int maxX, unsigned int minY, unsigned int maxY){
        srand(time(NULL));
        unsigned int finalX = std::rand() % maxX + minX;
        unsigned int finalY = std::rand() % maxY + minY;
        return Position(finalX, finalY);
    }

    bool operator==(const Position& right){
        return right.x == x && right.y == y;
    }
};

#endif // POSITION_H
