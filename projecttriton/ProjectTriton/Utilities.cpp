//-------------------------------------------------------------------------------------------------
#include "Utilities.h"

using namespace Triton;


Game* Triton::parseGameFile(const char* filepath)
{
	list<string> tokens = parseOutComments(filepath);

	// pops out the game tokens(<game> and </game> from the list)
	tokens.pop_back();
	tokens.pop_front();

	// creates a new game object
	Game* game = new Game;

	for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
	{
		if (*it == "directory")
			game->directory = *(++it);
		else if (*it == "window")
			while (*(++it) != "/window")
			{
				if (*it == "fullscreen")
				{
					game->fullscreen = (stoi(*(++it)) != 0);
					if (game->fullscreen)
						game->desktop_resolution = (stoi(*(++it)) != 0);
					else
						game->desktop_resolution = false;
				}
				else if (*it == "resolution")
				{
					game->windowWidth = stoi(*(++it));
					game->windowHeight = stoi(*(++it));
				}
				else if (*it == "title")
					game->title = *(++it);
			}
		else if (*it == "scenes")
		{
			string sceneDirectory = game->directory;
			while (*(++it) != "/scenes")
			{
				if (*it == "directory")
					sceneDirectory.append(*(++it));
				else 
				{
					const string sceneID = *it;
					const string sceneFile = *(++it);
					game->sceneDirectories.emplace(sceneID, sceneFile);
					if (!(game->currentScene))
					{
						try{
							parseSceneFile(sceneDirectory.append(sceneFile).c_str(), game);
						}
						catch (exception e)
						{
							string errorMessage;
							throw errorMessage.append(sceneFile.c_str()).append(" ").append(e.what()).c_str();
						}
					} // end of if (!(game->currentScene))
				} // end of else
			} // end of while
		} // end of else if (*it == "scenes")
	} // end of for
	
	tokens.clear();

	return game;
}

// loads in assets and object specified by scene file for the currentScene
void Triton::parseSceneFile(const char* filepath, Game * game)
{
	list<string> tokens = parseOutComments(filepath);

	// pops out border tokens
	tokens.pop_back();
	tokens.pop_front();

	game->currentScene = new SceneLevel();

	for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
	{
		if (*it == "id")
			game->currentSceneID = *(++it);
		else if (*it == "assets")
		{
			string assetDirectory = game->directory;
			
			while (*(++it) != "/assets")
			{
				if (*it == "directory")
					assetDirectory.append((*(++it)).c_str());
				else
				{
					//*** // KM's parser will be used here, 
					// assetDirectory.append((*it).c_str()).c_str() should be the filepath to bend.xml
				}
			}
		}
		// assets should have been already filled by this point
		else if (*it == "objects")
		{
			string objectDirectory = game->directory;
			
			while (*(++it) != "/objects")
			{
				if (*it == "directory")
					objectDirectory.append((*(++it)).c_str());
				else if (*it == "object")
				{
					// iterator in the objects map to this object currently being parsed
					unordered_map<string, ObjectEntity *>::iterator objIterator;

					while (*(++it) != "/object")
					{
						if (*it == "file")
							objIterator = parseObjectFile(objectDirectory.append(
							(*(++it)).c_str()).c_str(), game->currentScene);
						else if (*it == "position")
						{
							float x, y, z;
							x = stof(*(++it));
							y = stof(*(++it));
							z = stof(*(++it));

							objIterator->second->position = vec3(x, y, z);
						}
					} // end while
				} // end else if
			} // end while
		} // end else if
	} // end for

	tokens.clear();
}

// KM's parser's split function with reworded nomenclature
list<string> Triton::split(const string &data)
{
	string token;

	list<string> output;

	for (string::const_iterator it = data.cbegin(); it != data.cend(); ++it)
	{
		switch (*it)
		{
		case '"':
			while (*(++it) != '"')
				token.push_back(*it);
			// token needs to be pushed back even if it's empty in this case
			output.push_back(token); 
			token.clear();
			break;
		case '<': case '>': case '=': case '/': case ' ': case ',': case ';': case '\'':
			if (token != "")
			{
				if (token == "true")
					output.push_back("1");
				else if (token == "false")
					output.push_back("0");
				else
					output.push_back(token);
				token.clear();
			}
			if (*it == '<')
				if (*(++it) == '/')
					token.push_back(*it);
				else
					--it;
			break;
		default:
			token.push_back(*it);
		}
	}
		
	return output;
}

// converts file in filepath specified and returns the strings of said file, removing linefeeds,
// comments and other unnecessary lines from the file, either xml or script
list<string> Triton::parseOutComments(const char* filepath)
{
	ifstream file;
	file.open(filepath, ifstream::in);

	string line;
	list<string> tokens;
	while (file.good())
	{
		getline(file, line);
		// removes indents from the file strings
		while (!strncmp(line.c_str(), "\t", 1) || !strncmp(line.c_str(), " ", 1))
			line = line.substr(1);
		// for the first line in an xml file, the header, which is really superficial for this 
		// engine and is only there for xml editors, such as notepad++
		if (!strncmp(line.c_str(), "<?", 2))
		{
			// checks if the header ends before the first linefeed
			size_t pos = line.rfind('>');
			// if it doesn't, it looks for the end of the header ignoring all character before it
			if (pos == line.npos)
				file.ignore(USHRT_MAX, '>');
			// if it does end before the first linefeed, checks if there's any other content on 
			// the first line before the first linefeed other than the header
			else if (pos + 1 < line.size())
				// moves the ifstream to the character right after the header, if there's 
				// more than just the header in the first line
				file.seekg(pos + 1 - line.size(), file.cur);
		}
		// checks for xml comments
		else if (!strncmp(line.c_str(), "<!--", 4))
		{
			// checks if the end of the comment is in this line of the file or if it's a block
			size_t pos = line.rfind("-->");
			// if it's a block comment the program searches for the end of the commment block
			while (pos == line.npos && file.good())
			{
				// searches for the next occurrence of the '>' character
				getline(file, line, '>');
				line.push_back('>');
				// checks if it's the end comment token
				pos = line.rfind("-->", line.size() - 3);
			}
			// checks if the comment is the only content in this line of the file
			if (pos != line.npos && pos + 3 < line.size())
				// sets the ifstream position right after the end comment token
				file.seekg(pos + 3 - line.size(), file.cur);
		}
		// checks for c++ style comments
		else if (!strncmp(line.c_str(), "//", 2))
			// program just throws away the line since the end token for this type of comment is a
			// line feed, which is the default delimiter for getline
			;
		// checks for c++ style block comments
		else if (!strncmp(line.c_str(), "/*", 2))
		{
			size_t pos = line.rfind("*/");
			while (pos == line.npos && file.good())
			{
				// searches for the next occurrence of the '/' character
				getline(file, line, '/');
				line.push_back('/');
				// checks if it's the end block comment token
				pos = line.rfind("*/", line.size() - 2);
			}
			// checks if the comment is the only content in this line of the file
			if (pos != line.npos && pos + 2 < line.size())
				// sets the ifstream position right after the end comment token
				file.seekg(pos + 2 - line.size(), file.cur);
		}
		else
			// appends each line that's not a comment or header to the token list
			tokens.splice(tokens.cend(), split(line));
	}

	file.close();

	return tokens;
}

// returns a map iterator so that position can be set later by scene file parser for the object
unordered_map<string, ObjectEntity *>::iterator Triton::parseObjectFile(
	const char* filepath, SceneLevel * scene)
{
	list<string> tokens = parseOutComments(filepath);

	// pops out border tokens
	tokens.pop_back();
	tokens.pop_front();

	string id;
	ObjectEntity * object = new ObjectEntity();

	for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
	{
		if (*it == "id")
			id = *(++it);
		else if (*it == "mesh")
			object->mesh = scene->meshes.find(*(++it));
		else if (*it == "armature")
		{
			object->armatureID = scene->armatures.find(*(++it));
			object->armature = *(object->armatureID->second);
		}
		else if (*it == "idle_anim")
			object->armature.setCurrentAnimation(*(++it));
		else if (*it == "material")
			object->material = scene->materials.find(*(++it));
	}

	tokens.clear();

	return scene->objects.emplace(id, object).first;
}