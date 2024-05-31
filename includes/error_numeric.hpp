#ifndef ERROR_NUMERIC_HPP
# define ERROR_NUMERIC_HPP

#include <exception>

class ChanOPrivsNeededException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("ChanOPrivsNeededException");}
};

class NoSuchChannelException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("NoSuchChannelException");}
};

class NotOnChannelException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("NotOnChannelException");}
};


class NeedMoreParamsException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("NeedMoreParamsException");}
};

#endif
