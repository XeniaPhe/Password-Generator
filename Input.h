/* 
 * File:   Input.h
 * Author: xenia
 *
 * Created on March 23, 2022, 8:43 PM
 */

#ifndef INPUT_H
#define INPUT_H

namespace input
{
	int getInteger(std::string question);
	char getChar(std::string question);
	std::string getString(std::string question);
}

#endif /* INPUT_H */