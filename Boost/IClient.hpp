/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** IClient
*/

#ifndef ICLIENT_HPP_
#define ICLIENT_HPP_

class IClient {
    public:
        virtual ~IClient(){};
        virtual void write(void *) = 0;
        virtual void run() = 0;
        virtual void stop() = 0;
};

#endif /* !ICLIENT_HPP_ */
