#include "Parser.h"

void Parser::run(Mesh type)
{
	string wholeLine;
	ifstream dataFile;

	dataFile.open(m_FileName);

	while (getline(dataFile, wholeLine))
	{
		list<string> parts = split(wholeLine);

		string sType = parts.front();
		if (!strcmp(sType.c_str(), "matrix"))
		{
			if (parts.size() > 2)
			{
				parts.pop_front();
				parts.pop_front();
			}

			sType = parts.front();

			if (!strcmp(sType.c_str(), "Space"))
			{
				getline(dataFile, wholeLine);

				for (int i = 0; i < 4; i++)
				{
					getline(dataFile, wholeLine);
					list<string> matrices = split(wholeLine);

					for (int j = 0; j < 4; j++)
					{
						float x = stof(matrices.front());
						matrices.pop_front();

						m_Matrix[i][j] = x;
					}
				}

			}
		}
		else if (!strcmp(sType.c_str(), "vertices"))
		{
			parts.pop_front();
			parts.pop_front();

			int count = stoi(parts.front());

			for (int i = 0; i < count; i++)
			{
				getline(dataFile, wholeLine);
				list<string> vertices = split(wholeLine);

				float x = stof(vertices.front());
				vertices.pop_front();
				float y = stof(vertices.front());
				vertices.pop_front();
				float z = stof(vertices.front());
				vertices.pop_front();

				vec3 vert(x, y, z);

				m_positions.push_back(vert); //Vertices

			}//end for

			getline(dataFile, wholeLine);
		}
		else if (!strcmp(sType.c_str(), "UVs"))
		{
			parts.pop_front();
			parts.pop_front();

			int count = stoi(parts.front());

			for (int i = 0; i < count; i++)
			{
				getline(dataFile, wholeLine);
				list<string> vertices = split(wholeLine);

				float x = stof(vertices.front());
				vertices.pop_front();
				float y = stof(vertices.front());
				vertices.pop_front();

				vec2 uvs(x, y);

				m_texCoords.push_back(uvs); // UVs

			}//end for

			getline(dataFile, wholeLine);

		}
		else if (!strcmp(sType.c_str(), "normals"))
		{
			parts.pop_front();
			parts.pop_front();

			int count = stoi(parts.front());

			for (int i = 0; i < count; i++)
			{
				getline(dataFile, wholeLine);
				list<string> normals = split(wholeLine);

				float x = stof(normals.front());
				normals.pop_front();
				float y = stof(normals.front());
				normals.pop_front();
				float z = stof(normals.front());

				vec3 norms(x, y, z);

				m_normals.push_back(norms); // normals

			}//end for

			getline(dataFile, wholeLine);

		}
		else if (!strcmp(sType.c_str(), "faces"))
		{
			parts.pop_front();
			parts.pop_front();

			const int count = stoi(parts.front()) * 3;
			m_Faces = new unsigned int*[count];

			getline(dataFile, wholeLine);

			for (int i = 0; i < count; i++)
			{

				getline(dataFile, wholeLine);

				m_Faces[i] = new unsigned int[5];

				list<string> face = split(wholeLine);
				string sNum;
				unsigned int v = 0;
				unsigned int w = 0;
				unsigned int x = 0;
				unsigned int y = 0;
				unsigned int z = 0;

				sNum = face.front();

				if (strcmp(sNum.c_str(), "na"))
					v = stoi(sNum);

				face.pop_front();
				sNum = face.front();

				if (strcmp(sNum.c_str(), "na"))
					w = stoi(sNum);

				face.pop_front();
				sNum = face.front();

				if (strcmp(sNum.c_str(), "na"))
					x = stoi(sNum);

				face.pop_front();
				sNum = face.front();

				if (strcmp(sNum.c_str(), "na"))
					y = stoi(sNum);

				face.pop_front();
				sNum = face.front();

				if (strcmp(sNum.c_str(), "na"))
					z = stoi(sNum);

				face.pop_front();

				m_Faces[i][0] = v;
				m_Faces[i][1] = w;
				m_Faces[i][2] = x;
				m_Faces[i][3] = y;
				m_Faces[i][4] = z;

			}//end i

			m_positions;
			m_texCoords;
			m_normals;
			m_Faces;
			m_Matrix;


			getline(dataFile, wholeLine);

		}//end if

	}// end loop

	for (int i = 0; i < 60; i++)
	{
		cout << m_Faces[i][0] << ", " << m_Faces[i][1] << ", " << m_Faces[i][2] << ", " << m_Faces[i][3] << ", "
			<< m_Faces[i][4] << endl;
	}
	dataFile.close();
}

//type.indices.push_back(faces); // Faces
list<string> Parser::split(const string &data)
{
	string hold;

	list<string> datum;


	for (string::const_iterator i = data.begin(); i != data.end(); i++)
	{
		if (*i == '<' || *i == '>' || *i == '/' || *i == '?' || *i == '=' || *i == '"'
			|| *i == '\t' || *i == ' ' || *i == ',' || *i == ';')
		{
			datum.push_back(hold);
			if (hold == "")
				datum.pop_back();
			hold = "";
		}
		else
		{
			hold.push_back(*i);
		}
	}

	return datum;

}
