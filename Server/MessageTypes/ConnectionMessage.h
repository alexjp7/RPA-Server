#pragma once

#include "Message.h"

namespace RPA
{
    class ConnectionMessage: public Message
    {
        private:
            std::string name;

        private:
            void deserialize(const std::string&); //Override from message
            void serialize(const std::vector<std::unique_ptr<RPA::Player> >&);
            
        public: 
            ConnectionMessage();
            ConnectionMessage(const std::string&);
            ConnectionMessage(const int& _stateId, const int& _gameId, const int& _clientId, 
                              const std::string& _name, const std::vector<std::unique_ptr<RPA::Player> >& players);

            std::string getClientName();
            std::string getMessage();
    };

    struct person 
    {
        std::string name;
        std::string address;
        int age;
    };
    void test();
    void to_json(json& j, const person& p);
}