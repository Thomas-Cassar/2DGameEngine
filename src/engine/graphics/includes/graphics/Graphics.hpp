#pragma once 
#include "GL/glew.h"
#include <iostream>

/**
 * @brief If myAssert returns false the program breaks
 */
#ifdef __linux__
#define myAssert(x)\
    if(!(x))\
        __builtin_trap();
#elif _WIN32
#define myAssert(x)\
    if(!(x))\
    __debugbreak();
#else
    #define myAssert(x)\
    if(!(x))\
        exit(1);
#endif
        

/**
 * @brief Clears previous errors, runs the function and logs and
 * breaks in the case of an error.
 */
#define glCheck(x)\
    glClearErrors();\
    x;\
    myAssert(glLogErrors(#x,__FILE__,__LINE__))

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
bool glLogErrors(const char* function, const char* file, int line);