//-------------------------------------------------------------------------------------------------
//	File: [File Name]
//
//	Functions:
//	[Functions Used]
//
//-------------------------------------------------------------------------------------------------
#include "Utilities.h"

using namespace Triton;
//using namespace std;
using std::list;
using std::string;
using std::exception;

using glm::vec2;
using glm::vec3;

Parser parser;

void Parser::clear()
{
	currentGameDirectory.clear();
	meshMap.clear();
	objectMap.clear();
	materialMap.clear();
	textureMap.clear();
	shaderMap.clear();
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
list<string> Triton::getTokensFromFile(const char* filepath)
{
	try{
		std::ifstream file;
		file.open(filepath, std::ifstream::in);

		string line;
		list<string> tokens;
		while (file.good())
		{	
			getline(file, line);

			if (line.empty())
				continue;

			// removes indents from the file strings
			while ((line.front() == '\t') || (line.front() == ' '))
				line = line.substr(1);
			// for the first line in an xml file, the header, which is really superficial for this 
			// engine and is only there for xml editors, such as notepad++
			if (!strncmp(line.c_str(), "<?", 2))
			{
				// checks if the header ends before the first linefeed
				size_t pos = line.rfind('>');
				// if it doesn't, it looks for the end of the header ignoring all characters
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
				// program just throws away the line since the end token for this type of comment
				//  is a line feed, which is the default delimiter for getline
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

		if (tokens.empty())
		{
			const string error = "file failed to create tokens.";
			throw error;
		}
			
		// removes border tokens
		tokens.pop_back();
		tokens.pop_front();

		return tokens;
	}
	catch (const exception& e)
	{
		string errorMsg = e.what();
		throw (errorMsg + " occured while reading file " + filepath);
	}
	catch (const string& error)
	{
		string errorMsg = error;
		throw (errorMsg + " while reading file " + filepath);
	}
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
list<string> Triton::split(const string &data)
{
	try{
		string token;

		list<string> output;

		for (string::const_iterator it = data.cbegin(); it != data.cend(); ++it)
		{
			switch (*it)
			{
			case '"':
				while (*(++it) != '"')
				{
					if (it != --data.cend())
						token.push_back(*it);
					else
						throw std::out_of_range("no ending \" after a \"");
				}
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
	catch (const exception& e)
	{
		string errorMsg = e.what();
		throw (errorMsg + " occured while reading line " + data);
	}
	catch (const string& errorMessage)
	{
		throw errorMessage;
	}
}

#pragma region Parsers

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
void Triton::parse(const char* filepath, Game*& type)
{
	try{
		list<string> tokens = getTokensFromFile(filepath);

		for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
		{
			if (*it == "directory")
			{
				type->directory = *(++it);
				parser.currentGameDirectory = type->directory;
			}
			else if (*it == "window")
			{
				while (*(++it) != "/window")
				{
					if (*it == "fullscreen")
					{
						type->fullscreen = (stoi(*(++it)) != 0);
						if (type->fullscreen)
							type->desktop_resolution = (stoi(*(++it)) != 0);
						else
							type->desktop_resolution = false;
					}
					else if (*it == "resolution")
					{
						type->windowWidth = stoi(*(++it));
						type->windowHeight = stoi(*(++it));
					}
					else if (*it == "title")
						type->title = *(++it);
				}
			}
			else if (*it == "scenes")
			{
				string sceneDirectory = type->directory;
				bool directorySaved = false;
				while (*(++it) != "/scenes")
				{
					if (*it != "directory" || directorySaved)
					{
						const string sceneID = *it;
						const string sceneFile = *(++it);
						type->sceneDirectories.emplace(sceneID, sceneFile);
						if (!type->currentScene)
						{
							parse((sceneDirectory + sceneFile).c_str(), type->currentScene);
						} // end of if (!(game->currentScene))
					}
					else
					{
						sceneDirectory.append(*(++it));
						directorySaved = true;
					}
				} // end of while
			} // end of else if (*it == "scenes")
		} // end of for

		tokens.clear();

		parser.clear();
	}
#pragma region CatchBlocks
	catch (const exception& e)
	{
		parser.clear();
		string errorMsg = " occurred while reading file '";
		delete type;
		type = nullptr;
		throw (errorMsg + filepath + "' " + e.what());
	}
	catch (const string& errorMessage)
	{
		parser.clear();
		delete type;
		type = nullptr;
		throw errorMessage;
	}
#pragma endregion
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
void Triton::parse(const char* filepath, SceneLevel*& type)
{
	bool sdl_image_running = false;
	
	try{
		list<string> tokens = getTokensFromFile(filepath);

		type = new SceneLevel;

		parser.currentScene = type;

		for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
		{
#pragma region Meshes
			if (*it == "meshes")
			{
				string meshDirectory = parser.currentGameDirectory;

				while (*(++it) != "/meshes")
				{
					if (*it != "directory")
					{
						parser.currentScene->meshes.emplace_back(nullptr);
						
						parser.meshMap[parse(
							(meshDirectory + *it).c_str(), parser.currentScene->meshes.back())]
							= parser.currentScene->meshes.size() - 1;
					}
					else
						meshDirectory.append((*(++it)).c_str());
				}
			}
#pragma endregion

#pragma region Materials
			else if (*it == "materials")
			{
				string materialDirectory = parser.currentGameDirectory;
				
				while (*(++it) != "/materials")
				{
					if (*it != "directory")
					{
						parser.currentScene->materials.emplace_back(nullptr);

						parser.materialMap[parse(
							(materialDirectory + *it).c_str(), 
							parser.currentScene->materials.back())]
							= parser.currentScene->materials.size() - 1;
					}
					else
						materialDirectory.append((*(++it)).c_str());
				}
			}
#pragma endregion

#pragma region Textures
			else if (*it == "textures")
			{
				IMG_Init(IMG_INIT_PNG);
				sdl_image_running = true;

				string textureDirectory = parser.currentGameDirectory;

				while (*(++it) != "/textures")
				{
					if (*it != "directory")
					{
						parser.currentScene->textures.emplace_back(nullptr);

						parse((textureDirectory + *it).c_str(), 
							parser.currentScene->textures.back());
						parser.textureMap[*(++it)] = parser.currentScene->textures.size() - 1;
					}
					else
						textureDirectory.append((*(++it)).c_str());
				}

				IMG_Quit();
				sdl_image_running = false;
			}
#pragma endregion

#pragma region Shaders
			else if (*it == "shaders")
			{
				string shaderDirectory = parser.currentGameDirectory;

				while (*(++it) != "/shaders")
				{
					if (*it != "directory")
					{
						parser.currentScene->shaders.emplace_back(nullptr);

						parser.shaderMap[parse(
							(shaderDirectory + *it).c_str(), parser.currentScene->shaders.back())]
							= parser.currentScene->shaders.size() - 1;
					}
					else
						shaderDirectory.append((*(++it)).c_str());
				}
			}
#pragma endregion

#pragma region Objects
			// assets should have been already filled by this point
			else if (*it == "objects")
			{
				string objectDirectory = parser.currentGameDirectory;

				while (*(++it) != "/objects")
				{
					if (*it == "object")
					{
						while (*(++it) != "/object")
						{
							if (*it == "file")
							{
								parser.currentScene->objects.emplace_back(nullptr);
								 
								parser.objectMap[parse(
									(objectDirectory + *(++it)).c_str(), 
									parser.currentScene->objects.back())]
									= parser.currentScene->objects.size() - 1;
							}
						} // end while
					}
					else if (*it == "directory")
						objectDirectory.append((*(++it)).c_str());
				} // end while
			} // end else if
#pragma endregion
		}

		tokens.clear();

		parser.currentScene = nullptr;
	}
#pragma region CatchBlocks
	catch (const exception& e)
	{
		delete type;
		type = nullptr;
		parser.currentScene = nullptr;
		if (sdl_image_running)
			IMG_Quit();
		string errorMsg = e.what();
		throw (errorMsg + " occured while reading file " + filepath);
	}
	catch (const string& errorMessage)
	{
		delete type;
		type = nullptr;
		parser.currentScene = nullptr;
		if (sdl_image_running)
			IMG_Quit();
		throw errorMessage;
	}
#pragma endregion
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
string Triton::parse(const char* filepath, Mesh*& type)
{
	vector<vec3> vertices, normals, tangents;
	vector<vec2> UVs;
	vector<vector<short>> face_indices;
	vector<float> weights;
	vector<vector<unsigned short>> weight_indices;
	unsigned short vertexGroupCount;
	unsigned short indicesCount;
	unsigned short verticesCount;

	string id;
	
	try{
		list<string> tokens = getTokensFromFile(filepath);

		type = new Mesh;

		// parsing file
		for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
		{
			if (*it == "id")
			{
				id = *(++it);
			}
#pragma region Vertices
			if (*it == "vertices")
			{
				advance(it, 2); // skips over "count" token
				verticesCount = stoi(*it);
				for (unsigned short i = 0; i < verticesCount; ++i)
				{
					float x, y, z;
					x = stof(*(++it));
					y = stof(*(++it));
					z = stof(*(++it));

					vertices.push_back(vec3(x, y, z));
				}
			}

#pragma region Vertex_Weights
			else if (*it == "vertex_groups")
			{
				advance(it, 2); // skips over "count" token
				vertexGroupCount = stoi(*it);
				advance(it, vertexGroupCount);
			}
			else if (*it == "weights")
			{
				advance(it, 2); // skips over "count" token
				unsigned short count = stoi(*it);
				for (unsigned short i = 0; i < count; ++i)
				{
					weights.push_back(stof(*(++it)));
				}
			}
			else if (*it == "weights_indices")
			{
				advance(it, 2); // skips over "count" token
				for (unsigned short i = 0; i < verticesCount; ++i)
				{
					weight_indices.push_back(vector<unsigned short>());
					for (unsigned short j = 0; j < vertexGroupCount; ++j)
					{
						weight_indices[i].push_back(stoi(*(++it)));
					}
				}
			}
#pragma endregion

#pragma endregion

#pragma region UVs
			else if (*it == "UVs")
			{
				advance(it, 2); // skips over "count" token
				unsigned short count = stoi(*it);
				for (unsigned short i = 0; i < count; ++i)
				{
					float x, y;
					x = stof(*(++it));
					y = stof(*(++it));

					UVs.push_back(vec2(x, y));
				}
			}
#pragma endregion

#pragma region Normals
			else if (*it == "normals")
			{
				advance(it, 2); // skips over "count" token
				unsigned short count = stoi(*it);
				for (unsigned short i = 0; i < count; ++i)
				{
					float x, y, z;
					x = stof(*(++it));
					y = stof(*(++it));
					z = stof(*(++it));

					normals.push_back(vec3(x, y, z));
				}
			}
#pragma endregion

#pragma region Tangents
			else if (*it == "tangents")
			{
				advance(it, 2); // skips over "count" token
				unsigned short count = stoi(*it);
				for (unsigned short i = 0; i < count; ++i)
				{
					float x, y, z;
					x = stof(*(++it));
					y = stof(*(++it));
					z = stof(*(++it));

					tangents.push_back(vec3(x, y, z));
				}
			}
#pragma endregion

#pragma region Faces
			else if (*it == "faces")
			{
				advance(it, 2); // skips over "count" token
				indicesCount = stoi(*it) * 3;
				unsigned short copyIndex = 0;
				for (unsigned short i = 0; i < indicesCount; ++i)
				{
					// creates new empty short array
					face_indices.push_back(vector<short>());
					// fills short array with values from tokens
					for (unsigned short j = 0; j < 5; ++j)
					{
						if (*(++it) != "na")
							face_indices[i].push_back(stoi(*(it)));
						else
							face_indices[i].push_back(-2);
					}

					bool unique = true;

					// checks if array is a unique set of shorts
					for (unsigned short j = 0; j < i; ++j)
					{
						bool sameSet = true;

						for (unsigned short k = 0; k < 5; ++k)
						{
							if (face_indices[i][k] != face_indices[j][k])
							{
								sameSet = false; break;
							}
						}

						if (sameSet)
						{
							unique = false;
							copyIndex = j; break;
						}
					}

					if (!unique)
					{
						type->indices.push_back(copyIndex);
						face_indices.pop_back();
						--indicesCount; --i;
					}
					else
						type->indices.push_back(i);
				} // end of for
			} // end of else if
#pragma endregion
		} // end of for

		tokens.clear();

		// filling mesh
		for (unsigned short i = 0; i < face_indices.size(); ++i)
		{
			if (face_indices[i][0] != -2)
				type->vertices.push_back(vertices[face_indices[i][0]]);
			if (!weight_indices.empty())
			{
				type->groups.push_back(uvec4(0));
				type->weights.push_back(vec4(0.f));
				for (unsigned short j = 0; j < vertexGroupCount; ++j)
				{
					type->groups[i][j] = j;
					type->weights[i][j] = weights[weight_indices[face_indices[i][0]][j]];
				}
			}
			if (face_indices[i][1] != -2)
				type->UVs.push_back(UVs[face_indices[i][1]]);
			if (face_indices[i][2] != -2)
				type->normals.push_back(normals[face_indices[i][2]]);
			if (face_indices[i][3] != -2)
				type->tangents.emplace_back(tangents[face_indices[i][3]], (float)face_indices[i][4]);
		}

		type->init( // implement a form of vertex formatting later
			);

		vertices.clear();//** Also breaking here
		normals.clear();
		tangents.clear();
		face_indices.clear();
		weights.clear();
		weight_indices.clear();

		return id;
	}
#pragma region CatchBlocks
	catch (const exception& e)
	{
		delete type;
		type = nullptr;
		parser.currentScene->meshes.pop_back();
		vertices.clear();
		normals.clear();
		tangents.clear();
		face_indices.clear();
		weights.clear();
		weight_indices.clear();
		string errorMsg = e.what();
		throw (errorMsg + "occured while reading file " + filepath);
	}
	catch (const string& errorMessage)
	{
		delete type;
		type = nullptr;
		parser.currentScene->meshes.pop_back();
		vertices.clear();
		normals.clear();
		tangents.clear();
		face_indices.clear();
		weights.clear();
		weight_indices.clear();
		throw errorMessage;
	}
#pragma endregion
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
string Triton::parse(const char* filepath, ObjectEntity*& type)
{
	string id;
	
	try{
		list<string> tokens = getTokensFromFile(filepath);

		type = new ObjectEntity;

		for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
		{
			if (*it == "id")
			{
				id = *(++it);
			}
			else if (*it == "mesh")
			{
				// object->mesh = activeScene->meshes.find(*(++it));
				// safety: program crashes if you try to dereference the end iterator 
				if (parser.meshMap.find(*(++it)) != parser.meshMap.cend())
					type->mesh = parser.currentScene->meshes[parser.meshMap[*it]];
			}
			else if (*it == "material")
			{
				if (parser.materialMap.find(*(++it)) != parser.materialMap.cend())
					type->material = parser.currentScene->materials[parser.materialMap[*it]];
			}
		}
		
		tokens.clear();

		return id;
	}
#pragma region CatchBlocks
	catch (const exception& e)
	{
		delete type;
		type = nullptr;
		parser.currentScene->objects.pop_back();
		string errorMsg = e.what();
		throw (errorMsg + " occured while reading file " + filepath);
	}
	catch (const string& errorMessage)
	{
		delete type;
		type = nullptr;
		parser.currentScene->objects.pop_back();
		throw errorMessage;
	}
#pragma endregion
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
string Triton::parse(const char* filepath, Material*& type)
{
	string id;
	
	try{
		list<string> tokens = getTokensFromFile(filepath);

		type = new Material;

		for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
		{
			if (*it == "id")
			{
				id = *(++it);
			}
			else if (*it == "shader")
			{
				type->shader = parser.currentScene->shaders[parser.shaderMap[*(++it)]];
			}
			else if (*it == "texture")
			{
				advance(it, 2); // skips over type token
				if (*it == "diffuse")
				{
					type->diffuse = parser.currentScene->textures[parser.textureMap[*(++it)]];
				}
			}
		}

		tokens.clear();

		return id;
	}
#pragma region CatchBlocks
		catch (const exception& e)
		{
			delete type;
			type = nullptr;
			string errorMsg = " occurred while reading file '";
			throw (errorMsg + filepath + "' " + e.what());
		}
		catch (const string& errorMessage)
		{
			delete type;
			type = nullptr;
			throw errorMessage;
		}
#pragma endregion
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
void Triton::parse(const char* filepath, Texture*& type)
{
	try{
		type = new Texture;

		type->load(filepath);
	}
#pragma region CatchBlocks
	catch (const exception& e)
	{
		delete type;
		type = nullptr;
		string errorMsg = " occurred while reading file '";
		throw (errorMsg + filepath + "' " + e.what());
	}
	catch (const string& errorMessage)
	{
		delete type;
		type = nullptr;
		throw errorMessage;
	}
#pragma endregion
}

//-------------------------------------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//-------------------------------------------------------------------------------------------------
string Triton::parse(const char* filepath, Shader*& type)
{
	string id;
	
	try{
		list<string> tokens = getTokensFromFile(filepath);

		type = new Shader;

		string shaderDirectory = parser.currentGameDirectory;

		type->init();

		for (list<string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
		{
			if (*it == "id")
			{
				id = *(++it);
			}
			else if (*it == "directory")
			{
				shaderDirectory.append((*(++it)).c_str());
			}
			else if (*it == "component")
			{
				advance(it, 2); // skips type token
				GLenum shaderType;
				if (*it == "vertex")
					shaderType = GL_VERTEX_SHADER;
				else if (*it == "fragment")
					shaderType = GL_FRAGMENT_SHADER;

				string line;
				string GLSLstrings = "";

				std::ifstream file;
				file.open((shaderDirectory + *(++it)).c_str(), std::ifstream::in);

				while (file.good())
				{
					getline(file, line);
					GLSLstrings.append(line + "\n");
				}

				file.close();

				type->addComponent(GLSLstrings, shaderType);
			}
		}

		tokens.clear();

		type->compile();

		return id;
	}
#pragma region CatchBlocks
	catch (const exception& e)
	{
		delete type;
		type = nullptr;
		string errorMsg = " occurred while reading file '";
		throw (errorMsg + filepath + "' " + e.what());
	}
	catch (const string& errorMessage)
	{
		delete type;
		type = nullptr;
		throw errorMessage;
	}
#pragma endregion
}

#pragma endregion