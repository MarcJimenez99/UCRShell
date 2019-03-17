#ifndef SEMICOLON_H
#define SEMICOLON_H

#include "connector.hpp"

class Semicolon : public Connector {
    private:
        // bool (Base* );
    public:
        Semicolon() { };
        Semicolon(Base* , Base* );
        bool execute(int, int);
        bool checkFlag();
        void setFlagTrue() { };
};

#endif