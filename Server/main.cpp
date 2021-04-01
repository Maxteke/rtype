/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** main
*/

#include "Lobby.hpp"

int main(int argc, char **argv)
{
    try
    {
        Lobby lobby(argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
