/* 
 * peek_directory_iterator.cpp
 * this class performs iterator-like functions on the files in a directory, with a peek function added
 */

#include "peek_directory_iterator.h"
#include <string>
#include <fstream>
#include <filesystem>

// default constructor, initializes the iterator to the current directory
peek_directory_iterator::peek_directory_iterator()
{
    std::filesystem::directory_iterator d(".");
    this->iterator = d;
    this->current = "";
    this->nextcurrent = "";
    // we need to call next twice to populate current and nextcurrent 
    next();
    next();
}

// constructor, initializes the iterator to the given directory
peek_directory_iterator::peek_directory_iterator(std::string filename)
{
    std::filesystem::directory_iterator d(filename);
    this->iterator = d;
    this->current = "";
    this->nextcurrent = "";
    // we need to call next tiwce to populate current and nextcurrent
    next();
    next();
}

// destructor.
peek_directory_iterator::~peek_directory_iterator()
{

}

// returns the current item in the directory
std::string peek_directory_iterator::get()
{
    return current;
}

// returns the next item in the directory
std::string peek_directory_iterator::peek()
{
    return nextcurrent;
}

// advances the iterator to the next item in the directory
// if there are no more items in the directory, it will start returning ""
void peek_directory_iterator::next()
{
    current = nextcurrent;
    
    // this checks if the iterator is at the end of the directory, if it is then 
    // we should return without advancing the internal directory_iterator
    if(iterator == std::filesystem::end(iterator))
    {
        nextcurrent = "";
        return;
    }

    // this * operator gets the current directory item from the directory_iterator
    // it's dumb, I know
    auto p = *iterator;
    nextcurrent = p.path();

    // this advances the directory_iterator
    iterator++;
    
    

}