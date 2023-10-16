//
// Created by Ilya on 16.10.2023.
//

#ifndef SUNGEARENGINE_UUID_H
#define SUNGEARENGINE_UUID_H

#include <string>
#include <random>
#include <ios>
#include <sstream>
#include <iostream>

namespace Core::Patterns
{
    // UUID STRUCT: 8-4-4-4-12
    // xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
    struct UUID
    {
    private:
        std::string m_uuid;

    public:
        UUID()
        {
            static std::random_device rd;
            static std::mt19937_64 gen(rd());
            static std::uniform_int_distribution<> dis(0, 15);
            /*
             *  1 0 0 0 = 8
             *  1 0 0 1 = 9
             *  1 0 1 0 = A
             *  1 0 1 1 = B
             */
            static std::uniform_int_distribution<> dis2(8, 11);

            std::stringstream ss;
            int i;
            ss << std::hex;
            for (i = 0; i < 8; i++) {
                ss << dis(gen);
            }
            ss << "-";
            for (i = 0; i < 4; i++) {
                ss << dis(gen);
            }
            // 4 IS RANDOMLY OR PSEUDO-RANDOMLY GENERATED UUID
            ss << "-4";
            for (i = 0; i < 3; i++) {
                ss << dis(gen);
            }
            ss << "-";
            ss << dis2(gen);
            for (i = 0; i < 3; i++) {
                ss << dis(gen);
            }
            ss << "-";
            for (i = 0; i < 12; i++) {
                ss << dis(gen);
            };

            m_uuid = ss.str();

            std::cout << "uuid: " << m_uuid << std::endl;
        }

        bool operator==(const UUID& other) const
        {
            return m_uuid == other.m_uuid;
        }
    };
}

#endif //SUNGEARENGINE_UUID_H
