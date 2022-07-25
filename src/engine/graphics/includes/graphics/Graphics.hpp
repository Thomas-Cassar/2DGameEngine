#pragma once
#include "GL/glew.h"
#include "core/core.hpp"
#include <iostream>

/**
 * @brief Clears previous errors, runs the function and logs and
 * breaks in the case of an error.
 */
#define glCheck(x)                                                                                                     \
    glClearErrors();                                                                                                   \
    x;                                                                                                                 \
    engineAssert(glLogErrors(#x, __FILE__, __LINE__))

/**
 * @brief Clears Previous OpenGL Errors
 *
 * @return ** void
 */
void glClearErrors();

/**
 * @brief Prints any errors to the cerr output
 *
 * @param function The function to log
 * @param file The file the function is in
 * @param line The line the function is called from
 * @return true The function ran successfully
 * @return false The function is unsuccessful and the error was logged
 */
bool glLogErrors(const char *function, const char *file, int line);