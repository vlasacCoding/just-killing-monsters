#ifndef INVALID_INPUT_H
#define INVALID_INPUT_H

#include <stdexcept>
#include <exception>

class invalid_input : public std::invalid_argument
{
public:
    using std::invalid_argument::invalid_argument; //constructor inherited
};

class file_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

#endif // INVALID_INPUT_H
