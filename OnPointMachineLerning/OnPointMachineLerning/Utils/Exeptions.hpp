#pragma once
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>

namespace opml
{
	static std::string message(const std::string &msg, size_t line, const std::string &function, const std::string &path)
	{
		return ("opml_message: " + msg + "\nat line: " + std::to_string(line) + " in function: " + function + "\nfrom file: " + path).c_str();
	}

	struct exeption : public std::exception
	{
		exeption(const std::string &msg, size_t line, const std::string &function, const std::string &path)
			: std::exception(("opml_exeption: " + msg + "\nat line: " + std::to_string(line) + " in function: " + function + "\nfrom file: " + path).c_str())
			, line(line)
			, function(function)
			, path(path)
		{
		}

		size_t line;
		std::string function, path;
	};

	static class dualOutputStream
	{
	public:
		dualOutputStream(const std::string &filename)
		{
			try { fout.open(filename, std::ios::trunc); }
			catch (std::exception const& e) {
				std::cerr << "There was an error in OpmlDualOutputStream(const std::string &filename): " << e.what() << std::endl;
			}
		}

		~dualOutputStream() { fout.close(); }

		template<class T>
		dualOutputStream& operator<<(const T& x)
		{
			try {
				std::cout << x;
				fout << x;
			}
			catch (std::exception const& e) {
				std::cerr << "There was an error in dual_stream& operator<<(const T& x): " << e.what() << std::endl;
			}
			return *this;
		}
	private:
		std::ofstream fout;
	} out_opml = {"opml_log.txt"};
}

#define OPML_INTERNAL_CATCH catch(std::exception &e) { OPML_THROW(e.what()); }
#define OPML_MESSAGE(msg) opml::message(msg,  __LINE__, __FUNCTION__, __FILE__)
#define OPML_THROW(x) throw opml::exeption(x, __LINE__, __FUNCTION__, __FILE__)