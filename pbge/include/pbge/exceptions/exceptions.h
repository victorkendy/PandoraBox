


#ifndef pbge_exceptions_h_
#define pbge_exceptions_h_

#include <string>

namespace pbge {
    class Exception {
    public:
        virtual const std::string getMessage() = 0;
    };

    class BuilderValidationException : public Exception {
    public:
        BuilderValidationException(const std::string & _message) {
            this->message = message;
        }

        virtual const std::string getMessage() {
            return message;
        }
    private:
        std::string message;
    };
}


#endif
