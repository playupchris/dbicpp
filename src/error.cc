#include "dbic++.h"
#include <cstdio>

namespace dbi {

    using namespace std;

    Error::Error(const char* msg = "UNKNOWN") : exception() {
        message = string(msg);
    }

    const char* Error::what() const throw() {
        snprintf((char*)messagebuffer, 8192, "%s", message.c_str());
        return messagebuffer;
    }

    Error::~Error() throw() {}

    ConnectionError::ConnectionError(const char* msg = "UNKNOWN") : Error(msg) {}
    ConnectionError::ConnectionError(string msg) : Error(msg.c_str()) {}

    RuntimeError::RuntimeError(const char* msg = "UNKNOWN") : Error(msg) {}
    RuntimeError::RuntimeError(string msg) : Error(msg.c_str()) {}

    InvalidDriverError::InvalidDriverError(const char* msg = "UNKNOWN") : Error(msg) {}
    InvalidDriverError::InvalidDriverError(string msg) : Error(msg.c_str()) {}
}
