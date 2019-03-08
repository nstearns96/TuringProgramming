#include "TuringMachine.h"

bool isNum(std::string s)
{
	for (int c = 0; c < s.length(); ++c)
	{
		if (s[c] != '0' && s[c] != '1' && s[c] != '2' && s[c] != '3' && s[c] != '4' &&
			s[c] != '5' &&  s[c] != '6' && s[c] != '7' && s[c] != '8' && s[c] != '9')
		{
			return false;
		}
	}
	return true;
}

void TuringMachine::step()
{

	//Extend tape on side if out of bounds
	if (head.position == -1)
	{
		tape.insert(tape.begin(), blankSymbol);
		++head.position;
	}
	else if (head.position == (int)tape.size()) tape.push_back(blankSymbol);

	int TableOffset = (alphabet.size() + 1) * stoi(head.state) +
		((tape[head.position]) == blankSymbol ? 0 :
		(1 + std::find(alphabet.begin(), alphabet.end(), tape[head.position]) - alphabet.begin()));
	tape[head.position] = table.write[TableOffset];
	head.position += (table.move[TableOffset] == 'r') ? 1 : -1;
	head.state = table.nextState[TableOffset];
}

int TuringMachine::load(std::string filePath)
{
	std::ifstream file;
	file.exceptions(std::ios::badbit);
	file.open(filePath);
	std::string line;
	if (file)
	{
		//Check for valid header
		getline(file, line);
		if (line == "TM")
		{
			//Read in alphabet
			getline(file, line);
			blankSymbol = line[0];
			for (int i = 2; i < line.length(); i += 2)
			{
				alphabet.push_back(line[i]);
			}

			//Get number of states
			getline(file, line);
			table.numStates = stoi(line);

			//Get Table
			getline(file, line);
			for (int i = 0; i < 2 * table.numStates * (alphabet.size() + 1); i += 2)
			{
				if (line[i] == blankSymbol || std::find(alphabet.begin(), alphabet.end(), line[i]) != alphabet.end())
				{
					table.write.push_back(line[i]);
				}
				else
				{
					printf("Error: Character not in alphabet.\nRead: %s\n", line[i]);
					file.close();
					return -1;
				}
			}

			getline(file, line);
			for (int i = 0; i < 2 * table.numStates * (alphabet.size() + 1); i += 2)
			{
				if (line[i] == 'r' || line[i] == 'l')
				{
					table.move.push_back(line[i]);
				}
				else
				{
					printf("Error: Invalid move direction.\nRead: %s\n", line[i]);
					file.close();
					return -1;
				}
			}

			for (int i = 0; i < table.numStates * (alphabet.size() + 1); ++i)
			{
				getline(file, line, ' ');
				if (line == "a" || line == "r")
				{
					table.nextState.push_back(line);
				}
				else
				{
					if (isNum(line.c_str()) && 0 < stoi(line) <= table.numStates)
					{
						table.nextState.push_back(line);
					}
					else
					{
						printf("Error: Invalid State Transition.\n Read %s\n", line);
						file.close();
						return -1;
					}
				}
			}
		}
		else
		{
			printf("Error: Invalid Header Filed.\nExpected: \"TM\".\nRead: %s\n", line.c_str());
			file.close();
			return -1;
		}

		file.close();
	}
	else
	{
		printf("Error: Failed to open file!");
		return -1;
	}

	return 0;
}