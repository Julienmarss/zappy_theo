#ifndef MOUV_HPP
#define MOUV_HPP

#include <sys/socket.h>

// Forward declaration
class IA;
class MyIA;

void handle_movement(int s, MyIA& ia);

#endif // MOUV_HPP