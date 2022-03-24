
/* 
 * File:   main.cpp
 * Author: xenia
 *
 * Created on March 23, 2022, 1:46 PM
 */

#include <iostream>
#include <random>
#include <chrono>
#include "Input.h"

static int getLength(std::string question);
static uint8_t getCharTypes(std::string question);
static std::string generatePassword(int passwordLength, int maxCharTypeRepetitionLength, int maxCharRepetitionLength, uint8_t allowedChars);
static char generateRandomChar(uint8_t* validTypes, int validTypeCount, uint8_t* charType);

namespace //For the flags to be only accessible inside main.cpp
{
	constexpr uint8_t n = 1;
	constexpr uint8_t u = 1 << 1;
	constexpr uint8_t l = 1 << 2;
	constexpr uint8_t s = 1 << 3;
}

int main(int argc, char** argv)
{
	std::string passwordLengthString = "Enter the length of the password you want to create : ";
	std::string maxTypeRepetitionString = "Enter the length of the longest sequence of same type of characters allowed in the password : ";
	std::string maxCharRepetitionString = "Enter the length of the longest sequence of same characters allowed in the password : ";
	std::string charTypesString = "Enter which characters(lower case:l, upper case:u, number:n, sign:s) should be allowed : ";
	
	int counter=0;
	
	int passwordlength;
	int maxTypeRepetition;
	int maxCharRepetition;
	char userResponse = NULL;
	uint8_t charTypesFlag;
	
	
	while(true)
	{
		if(userResponse != 'r' && userResponse != 'R')
		{
			passwordlength = getLength(passwordLengthString);
			maxTypeRepetition = getLength(maxTypeRepetitionString);
			maxCharRepetition = getLength(maxCharRepetitionString);
			charTypesFlag = getCharTypes(charTypesString);
		}

		std::string password = generatePassword(passwordlength, maxTypeRepetition, maxCharRepetition, charTypesFlag);

		std::cout << "\nPassword : " << password<<"\n\n";
		
		std::cout << "Press Q and hit enter to exit\n";
		std::cout << "Press R and hit enter to generate a new password with the same settings\n";
		std::cout << "Press S and hit enter to generate a new password with different settings\n";
		
		while(true)
		{
			userResponse = input::getChar("");
			
			switch(userResponse)
			{
				case 'q':
				case 'Q':
				case 'r':
				case 'R':
				case 's':
				case 'S':
					break;
				default :
					std::cout<<"Invalid input, please try again : ";
					continue;
					break;
			}
			break;
		}
		
		if(userResponse == 'q' || userResponse == 'Q')
			break;
	}
		
	return 0;
}

static int getLength(std::string question)
{
	int length = 0;
	
	while((length = input::getInteger(question))<1)
	{
		std::cerr << "\nZero(0) is not a valid length, please try again : ";
	}

	return length;
}


static uint8_t getCharTypes(std::string question)
{
	uint8_t flag = 0;
	std::string allowedChars = "";
	
	while(true)
	{
		allowedChars = input::getString(question);
		
		for (int i = 0; i < allowedChars.length(); i++)
		{
			switch (allowedChars.at(i))
			{
				case 'n':
				case 'N':
					flag |= n;
					break;
				case 'u':
				case 'U':
					flag |= u;
					break;
				case 'l':
				case 'L':
					flag |= l;
					break;
				case 's':
				case 'S':
					flag |= s;
					break;
			}
		}
		
		if (flag == 0) //If user provided no valid input
			std::cerr << "\nInvalid input, please try again : \n";
		else break;
	}

	return flag;
}

static std::string generatePassword(int passwordLength, int maxTypeRepetition, int maxCharRepetition, uint8_t allowedChars)
{
	uint8_t validTypes[4];
	uint8_t temp;
	int validTypeCount = 0;

	for (int i = 0; i < 4; i++)
	{
		if (allowedChars & (temp = (1 << i)))
			validTypes[validTypeCount++] = temp;
	}

	uint8_t lastChartType = 0,chType=0;
	char lastChar = NULL,ch;
	bool again=false;
	int currentTypeRepetition = 0;
	int currentCharRepetition = 0;
	
	std::string password="";
	
	for (int i = 0; i < passwordLength; i++)
	{
		while(true)
		{
			ch = generateRandomChar(validTypes, validTypeCount,&chType);
			
			if(lastChartType==chType)
			{
				++currentTypeRepetition;
				
				if (ch == lastChar)
					++currentCharRepetition;
				else
					currentCharRepetition = 0;
			}
			else
			{
				currentTypeRepetition = 0;
				currentCharRepetition=0;
			}
			
			again=false;
			
			if((validTypeCount>1 && (currentTypeRepetition > maxTypeRepetition)))
			{
				again=true;
				--currentTypeRepetition;
			}
			if(currentCharRepetition>maxCharRepetition)
			{
				again=true;
				--currentCharRepetition;
			}
			
			if(again)
				continue;
			else 
				break;
		}
		
		lastChartType=chType;
		lastChar=ch;
		password+=ch;
	}

	return password;
}

static char generateRandomChar(uint8_t* validTypes, int validTypeCount,uint8_t* charType)	//Third parameter is a return values
{
	static std::mt19937 mersenneTwister{std::random_device{}()};

	std::uniform_int_distribution<> charTypeDistributor{0, validTypeCount - 1};
	int bitIndex = charTypeDistributor(mersenneTwister);

	char ch = NULL;
	
	*charType = validTypes[bitIndex];
	switch (*charType)
	{
		case n:
			static std::uniform_int_distribution<> numberDistributor{48,57};	//from '0' to '9'
			ch = static_cast<char> (numberDistributor(mersenneTwister));
			break;
		case u:
			static std::uniform_int_distribution<> uppercaseDistributor{65,90};		//from 'A' to 'Z'
			ch = static_cast<char> (uppercaseDistributor(mersenneTwister));
			break;
		case l:
			static std::uniform_int_distribution<> lowercaseDistributor{97,122};	//from 'a' to 'z'
			ch = static_cast<char> (lowercaseDistributor(mersenneTwister));
			break;
		case s:

			static std::uniform_int_distribution<> signDistributorSelector{1,4};	//chooses which distributor to use

			switch (signDistributorSelector(mersenneTwister))
			{
				case 1:
					static std::uniform_int_distribution<> signDistributor1{33, 47};	//from '!' to '/'
					ch = static_cast<char> (signDistributor1(mersenneTwister));
					break;
				case 2:
					static std::uniform_int_distribution<> signDistributor2{58, 64};	//from ':' to '@'
					ch = static_cast<char> (signDistributor2(mersenneTwister));
					break;
				case 3:
					static std::uniform_int_distribution<> signDistributor3{91, 96};	//from '[' to '`'
					ch = static_cast<char> (signDistributor3(mersenneTwister));
					break;
				case 4:
					static std::uniform_int_distribution<> signDistributor4{123, 126};	//from '{' to '~'
					ch = static_cast<char> (signDistributor4(mersenneTwister));
					break;
			}

			break;
	}
	
	return ch;
}