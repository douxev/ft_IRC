#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <iostream>

class SendErrorException: public std::exception {
	public:
		virtual const char* what() const throw() {return ("SendErrorException");}
};

#endif
