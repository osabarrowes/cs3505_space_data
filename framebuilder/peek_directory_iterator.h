/*
 * class declaration header for peek_directory_iterator.cpp
 * a peek_directory_iterator is essentially the same as a directory iterator but with a peek function
 */

#include <string>
#include <fstream>
#include <filesystem>

class peek_directory_iterator
{
    private:
        std::string current;
        std::string nextcurrent;
        std::filesystem::directory_iterator iterator;

    public:
        peek_directory_iterator();
        peek_directory_iterator(std::string);
        ~peek_directory_iterator();

        std::string get();
        std::string peek();
        void next();
};