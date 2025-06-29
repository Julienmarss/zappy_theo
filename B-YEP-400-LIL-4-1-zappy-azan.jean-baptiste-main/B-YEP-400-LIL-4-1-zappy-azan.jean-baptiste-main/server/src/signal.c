/*
** EPITECH PROJECT, 2024
** main.cpp
** File description:
** main.cpp
*/

#include "socket.h"

void signal_handler(int signal_number)
{
    if (signal_number == SIGINT) {
        exit(0);
    }
}

void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}
