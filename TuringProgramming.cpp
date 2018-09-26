//Version 0.2

#include <iterator>
#include <map>
#include "TuringProgramming.h"


//Converts input file to .wb1
int LexWB1(std::string FilePath)
{
	std::ifstream FIn;
	FIn.exceptions(std::ios::badbit);
	FIn.open(FilePath);

	if (FIn)
	{
		std::ofstream FOut;
		FOut.open("output.wb1");
		if (FOut)
		{
			//Write WB1 header
			std::string Line;
			FOut << "WB1\n";
			getline(FIn, Line);

			//Read in alphabet
			std::vector<char> Alphabet;
			for (int I = 0; I < Line.length(); I += 2)
			{
				Alphabet.push_back(Line[I]);
			}

			//Write alphabet
			FOut << Line + '\n';

			while (std::getline(FIn, Line))
			{
				std::istringstream StringStream(Line);
				std::vector<std::string> Args(std::istream_iterator<std::string>{StringStream}, std::istream_iterator<std::string>());
				if (Args[0] == "accept") // accept
				{
					FOut << "0\n";
				}
				else if (Args[0] == "reject") //reject
				{
					FOut << "1\n";
				}
				else if (Args[0] == "if") //if-goto
				{
					FOut << "2 " << std::to_string(std::find(Alphabet.begin(), Alphabet.end(), Args[1][0]) - Alphabet.begin()) 
						<< " " << Args[3] << "\n";
				}
				else if (Args[0] == "write") //write
				{
					FOut << "3 " 
						<< std::to_string(std::find(Alphabet.begin(), Alphabet.end(), Args[1][0]) - Alphabet.begin())
						<< "\n";
				}
				else if (Args[0] == "goto") //goto
				{
					FOut << "4 " << Args[1] << "\n";
				}
				else if (Args[0] == "move") //move
				{
					FOut << "5 " << Args[1] << "\n";
				}
			}
			FOut.close();
		}

		FIn.close();
	}
	else
	{
		printf("Error: Failed to open file!");
		return -1;
	}

	return 0;
}

//Converts .wb1 to .tm
int WB1toTM(std::string FilePath)
{
	std::ifstream FIn;
	FIn.exceptions(std::ios::badbit);
	FIn.open(FilePath);

	if (FIn)
	{
		std::ofstream FOut;
		FOut.open("output.tm");
		if (FOut)
		{
			//Check for valid header
			std::string Line;
			getline(FIn, Line);

			if (Line == "WB1")
			{
				//Write header to file
				FOut << "TM\n";

				//Read in alphabet
				getline(FIn, Line);
				std::vector<char> Alphabet;
				for (int I = 0; I < Line.length(); I += 2)
				{
					Alphabet.push_back(Line[I]);
				}

				//Write alphabet
				FOut << Line + '\n';

				//Maps .wb1 lines to .tm states
				unsigned int LineNum = 0;
				std::map<unsigned int, unsigned int> LineMap;

				//TODO: Add functionality for string labels
				//Queue for labels to be process
				std::vector<unsigned int> LabelQueue;
				unsigned int LabelIter = 0;

				//template of turing machine table to be filled out
				unsigned int NumStates = 0;
				std::vector<char> Write;
				std::vector<char> Move;
				std::vector<std::string> NextState;

				while (getline(FIn, Line))
				{					
					std::istringstream StringStream(Line);
					std::vector<std::string> Args(std::istream_iterator<std::string>{StringStream}, 
						std::istream_iterator<std::string>());
					if (Args[0] == "0") // Accept
					{
						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('r');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back("a");
						}
						LineMap[LineNum++] = NumStates;
						++NumStates;
					}
					else if (Args[0] == "1") //Reject
					{
						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('r');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back("r");
						}
						LineMap[LineNum++] = NumStates;
						++NumStates;
					}
					else if (Args[0] == "2") // If-goto
					{
						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('r');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('l');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('l');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							if (C == stoi(Args[1]))
								NextState.push_back(std::to_string(NumStates + 1));
							else
								NextState.push_back(std::to_string(NumStates + 2));
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back("temp");
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back(std::to_string(NumStates+3));
						}
						LabelQueue.push_back(stoi(Args[2]));
						LineMap[LineNum++] = NumStates;
						NumStates += 3;
					}
					else if (Args[0] == "3") // write
					{
						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[stoi(Args[1])]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('r');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('l');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back(std::to_string(NumStates + 1));
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back(std::to_string(NumStates + 2));
						}
						LineMap[LineNum++] = NumStates;
						NumStates += 2;
					}
					else if (Args[0] == "4") //goto
					{
						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('r');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back('l');
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back(std::to_string(NumStates + 1));
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back("temp");
						}
						LabelQueue.push_back(stoi(Args[1]));
						LineMap[LineNum++] = NumStates;
						NumStates += 2;
					}
					else if (Args[0] == "5") // Move
					{
						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Write.push_back(Alphabet[C]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							Move.push_back(Args[1][0]);
						}

						for (int C = 0; C < Alphabet.size(); ++C)
						{
							NextState.push_back(std::to_string(NumStates+1));
						}
						LineMap[LineNum++] = NumStates;
						++NumStates;
					}
				}

				//Replace labels with mapped states
				while (!LabelQueue.empty())
				{
					if (NextState[LabelIter] == "temp")
					{
						NextState[LabelIter] = std::to_string(LineMap[LabelQueue[0]]);
						NextState[LabelIter+1] = std::to_string(LineMap[LabelQueue[0]]);
						NextState[LabelIter+2] = std::to_string(LineMap[LabelQueue[0]]);
						LabelQueue.erase(LabelQueue.begin());
					}
					++LabelIter;
				}
				//Write table template to .tm
				FOut << NumStates << "\n";
				for (int C = 0; C < Write.size(); ++C)
				{
					FOut << Write[C] << " ";
				}
				FOut << "\n";
				for (int C = 0; C < Move.size(); ++C)
				{
					FOut << Move[C] << " ";
				}
				FOut << "\n";
				for (int C = 0; C < NextState.size(); ++C)
				{
					FOut << NextState[C] << " ";
				}
				FOut << "\n";
			}
			else
			{
				printf("Error: Invalid Header Filed.\nExpected: \"WB1\".\nRead: %s\n", Line.c_str());
				FOut.close();
				FIn.close();
				return -1;
			}

			FOut.close();
		}

		FIn.close();
	}
	else
	{
		printf("Error: Failed to open file!");
		return -1;
	}

	return 0;
}

void main() 
{
	turing_machine Machine;
	Machine.Tape = { '1','1','0','1','1','0','1','1' };
	if (LexWB1("input.txt") == 0)
	{
		if (WB1toTM("output.wb1"))
		{
			if (Machine.Load("output.tm"))
			{
				while (Machine.Head.State != "r" && Machine.Head.State != "a")
				{
					Machine.Step();
				}
				printf("%s\n", Machine.Head.State == "r" ? "Rejected" : "Approved");
			}
		}
	}
}