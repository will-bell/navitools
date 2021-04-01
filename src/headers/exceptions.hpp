#pragma once

#include <exception>

/* Generic exception for when we have disagreeing state sizes */

struct BadStateSizeException : public std::exception
{
    const char* _msg = nullptr;

    BadStateSizeException() {};

    BadStateSizeException(const char* msg) {
        _msg = msg;
    }

    const char* what() const throw()
    {
        if (!_msg)
            return "Given state does not have the same size as data structure's state.";
        
        return _msg;
    }
};