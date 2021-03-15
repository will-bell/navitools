#include <exception>

/* Generic exception for when we have disagreeing state sizes */

struct BadStateSizeException : public std::exception
{
    const char* what() const throw()
    {
        return "Given state does not have the same size as data structure's state.";
    }
};