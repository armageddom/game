#pragma once

#include <exception>
#include <string>

namespace Exception {
	class Exception : public std::exception {
	public:
		Exception() {};
		Exception(const std::string message) {
			this->setMessage(message);
		}
		explicit Exception(const std::string& message);
		virtual const char* what() const throw() {
			return _message.c_str();
		}
	protected:
		void setMessage(std::string message) {
			this->_message = message;
		}
		std::string _message;
	};
	class FileNotFound : public Exception {
	public:
		FileNotFound(std::string filepath) {
			std::string message = "Error: file not found " + filepath;
			this->setMessage(message);
		}
	};
};