#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <iostream>
# include "error_numeric.hpp"

class SendErrorException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("SendErrorException");}
};


class NickAlreadyTakenException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("NickAlreadyTakenException");}
};

class UserNotFoundException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("UserNotFoundException");}
};

class ChannelNotFoundException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("ChannelNotFoundException");}
};


#endif
