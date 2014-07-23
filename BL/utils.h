#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <algorithm>

class utils
{
public:
    static std::string getExtension(std::string const& fileName);
    static std::string appendTextBeforeExtension(std::string const& inputStr, std::string const& strToAppend);
	static std::vector<std::string> splitString(std::string const& str, char const splitChar, bool const shouldIgnoreQuotes = false);
	static std::string toLower(std::string str);
	static std::string toUpper(std::string str);
private:	
	static bool isBetweenQuotes(int const position, std::string const& str);

public:
	template <typename T>
	static std::string convertToString(const T& number)
	{
		std::stringstream ss;//create a stringstream
		ss << number;//add number to the stream
		return ss.str();//return a string with the contents of the stream
	}

	template <>
	static std::string convertToString<double>(const double& number)
	{
		std::ostringstream ss;//create a stringstream
		ss << number;//add number to the stream
		return ss.str();//return a string with the contents of the stream
	}


	template <typename T>
	static T convertStringToNumber(std::string const& str)
	{
		T Result;//number which will contain the result

		stringstream convert(str); // stringstream used for the conversion initialized with the contents of Text

		if (!(convert >> Result))//give the value to Result using the characters in the string
			Result = 0;
		return Result;
	}
};

#endif // UTILS_H
