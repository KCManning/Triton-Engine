//-------------------------------------------------------------------------------------------------

// Engine will currently only support the loading of png files, see texture loading during scene
// parse

#include <vld.h>
#include "Engine.h"

using namespace std;
using namespace Triton;

int main(int argc, char** argv)
{
	try	
	{
		Engine engine;
		
		// int i = 0;

		engine.loadGame("../Test/testgame.xml");
		
		while (!engine.Quit)
			engine.update();
	}
	catch (const exception& error)
	{
		cerr << error.what() << endl;

		return EXIT_FAILURE;
	}
	catch (const string& ExceptionMessage)
	{
		cerr << ExceptionMessage << endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}