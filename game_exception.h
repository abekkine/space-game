#ifndef GAME_EXCEPTION_H_
#define GAME_EXCEPTION_H_

#include <exception>
#include <string>

class GameException : public std::exception {
public:
    enum ExceptionCode {
        eParameterNotFound = 10001,
        eGLFWError,
    };
public:
    GameException(ExceptionCode code, std::string & message)
    : message_(message)
    , code_(code)
    {}
    virtual ~GameException() {}
    virtual const char* what() const throw() {
        return message_.c_str();
    }
    ExceptionCode code() {
        return code_;
    }
private:
    const std::string message_;
    const ExceptionCode code_;
};

#endif  // GAME_EXCEPTION_H_
