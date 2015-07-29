//-------------------------------------------------------------------------------------------------
#include "Engine.h"
#include <iostream>

using namespace std;
using namespace Triton;

int main(int argc, char** argv)
{
	try	
	{
		Engine engine;


		int i = 0;
		
		while (!engine.Quit)
		{
			engine.update();
		
			for (; i < argc; ++i)
				cout << "argv[" << i << "] = " << argv[i] << endl;
			i = argc;
		}

		return EXIT_SUCCESS;
	}
	catch (const char* ExceptionMessage)	
	{
		cerr << ExceptionMessage << endl;

		return EXIT_FAILURE;
	}
	catch (exception error)
	{
		cerr << error.what() << endl;

		return EXIT_FAILURE;
	}
}