#include <iostream>
#include <string>
#include <limits>

namespace input
{
	static void ignoreBuffer()
	{
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	int getInteger(std::string question)
	{
		int number = 0;

		while (true)
		{
			std::cout << question;
			std::cin>>number;

			ignoreBuffer();
			
			if (std::cin.fail())
			{
				std::cin.clear();
				ignoreBuffer();
				std::cerr << "\nInvalid input, please try again : \n";
			}
			else
				break;
		}

		return number;
	}

	char getChar(std::string question)
	{
		char ch = NULL;

		std::cout << question;
		std::cin>>ch;
		ignoreBuffer();

		return ch;
	}

	std::string getString(std::string question)
	{
		std::string str = "";

		std::cout << question;
		std::cin>>str;
		ignoreBuffer();

		return str;
	}
}