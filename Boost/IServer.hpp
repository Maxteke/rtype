/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** IServer
*/

#ifndef ISERVER_HPP_
#define ISERVER_HPP_

#include <iostream>

class IServer {
    public:
        virtual ~IServer(){};
        virtual void run() = 0;
        virtual void writeTo(std::string addr, void *packet) = 0;
        virtual void writeToAll(void *packet) = 0;
        virtual void remove_user(std::string addr) = 0;
        virtual void stop() = 0;
};

#endif /* !ISERVER_HPP_ */
