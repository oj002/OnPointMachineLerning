#pragma once
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace opml
{
	static std::string message(const std::string& msg, size_t line, const std::string& function, const std::string& path)
	{
		return "opml_message: " + msg + "\nat line: " + std::to_string(line) + " in function: " + function + "\nfrom file: " + path;
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

	class dualOutputStream
	{
	public:
		explicit dualOutputStream(const std::string &filename = "opml_log.txt") noexcept
			: path(filename)
		{
			try { fout.open(filename, std::ios::trunc); }
			catch (std::exception const& e) {
				std::cerr << "There was an error in OpmlDualOutputStream(const std::string &filename): " << e.what() << std::endl;
			}
		}
		dualOutputStream(const dualOutputStream &copy) = delete;
		dualOutputStream(const dualOutputStream &&moved) = delete;
		dualOutputStream &operator=(dualOutputStream other) = delete;
		dualOutputStream &operator=(dualOutputStream &&other) = delete;
		~dualOutputStream() noexcept { fout.close(); }

		template<class T>
		dualOutputStream& operator<<(const T& x) noexcept
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
		const std::string path;
		std::ofstream fout;
	};
	static dualOutputStream out_opml;
}  // namespace opml

#define OPML_MESSAGE(msg) opml::message(msg,  __LINE__, __FUNCTION__, __FILE__)
#define OPML_THROW(x) throw opml::exeption(x, __LINE__, __FUNCTION__, __FILE__)
#define OPML_INTERNAL_CATCH catch(std::exception &e) { OPML_THROW(e.what()); }