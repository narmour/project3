/*******************************************************************************
* Assignment: Project 3 - Code generation for Scheme to C++, an addition to    *
* 			  CS 460 Project 1 and Project2 Lexical and Syntactical*
* 			  Analyzers.					       * 
* Author: Dr. Watts                                                            *
* Date: Fall 2018                                                              *
* File: Project3.cpp                                                           *
*                                                                              *
* Description: This file contains the main function that will create a         *
* 	       syntactical analyzer which contains its own lexical analyzer     *
* 	       and code generation function to convert Scheme to C++.          *
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: %s <filename>\n", argv[0]);
		exit (1);
	}
	
	SetLimits ();
	SyntacticalAnalyzer parse (argv[1]);

	return 0;
}
