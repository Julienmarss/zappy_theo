/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** IObject.hpp
*/

#pragma once

#include "raylib.h"
#include "IObjet.hpp"
#include <string>
#include "raylib_wrapper.hpp"
#include <iostream>
#include <fstream>
#include <array>
#include <exception>

class IObject {
public:
    virtual ~IObject() = default;

    virtual int getId() const = 0;
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual void setPosition(int x, int y) = 0;
    virtual void Draw() = 0;
    class ObjectException : public std::exception {
    public:
        ObjectException(const std::string &message) : _message(message) {}
        const char *what() const noexcept override { return _message.c_str();
        }
    private:
        std::string _message;
    };
};
