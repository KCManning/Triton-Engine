//-------------------------------------------------------------------------------------------------

// Engine will currently only support the loading of png files, see texture loading during scene
// parse

//#include <vld.h>
#include "Engine.h"

using namespace Triton;

int main(int argc, char** argv)
{
	try	
	{
		Engine engine;
		
		// int i = 0;

		engine.loadGame("../Test/testgame.xml");
		
		while (!engine.Quit)//*** Unhandled exception
			engine.update();
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;

		return EXIT_FAILURE;
	}
	catch (const std::string& ExceptionMessage)
	{
		std::cerr << ExceptionMessage << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}