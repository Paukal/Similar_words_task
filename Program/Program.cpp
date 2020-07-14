/*
NOTES:
	*Optimization lines might interfere with results (but shouldn't)
	*All sample tests were successful
	*sample_3.txt might take longer to execute than expected
	*Used OPTIMIZATION: data sorting, data formatting, duplicate data deletion, extra letter checking
*/

#include <iostream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector> 
#include <algorithm>

using namespace std;

struct Line //data stucture for each line
{
	int letterCount;
	string letters;
	int chainNumber=-1;

	bool formated = false;
	string formLetter = "";
	string formLocation="";
	string formLength="";
	int formLengthSum = 0;
};

bool sortFunction(Line i, Line j) //sorting by letter count
{ 
	std::string sLength1 = i.formLength;
	std::string sLength2 = j.formLength;
	std::string delimiter = ";";

	int count1=0; int count2 = 0;

	size_t pos = 0;
	std::string tokenLength1;
	std::string tokenLength2;

	while ((pos = sLength1.find(delimiter)) != std::string::npos) {
		tokenLength1 = sLength1.substr(0, pos);
		sLength1.erase(0, pos + delimiter.length());

		count1 += stoi(tokenLength1);
	}

	while ((pos = sLength2.find(delimiter)) != std::string::npos) {
		tokenLength2 = sLength2.substr(0, pos);
		sLength2.erase(0, pos + delimiter.length());

		count2 += stoi(tokenLength2);
	}

	return (i.letterCount+count1 > j.letterCount+count2);
}

int main()
{
	//string pathName= "C:/Users/Paulius/Documents/Study n code/Oracle/task_3_b_similar_words/samples/"; 
	string pathName = "../task_3_b_similar_words/samples/";
	string fileName;
	ifstream file;
	string line; int lineCount = 0; vector<Line> lines;

	int result = 0;

	cout << "Enter data file from SAMPLES folder with extension (sample_1.txt): ";
	cin >> fileName;
	pathName += fileName;

	file.open(pathName);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			string num=""; int j=0;
			while (!isspace(line[j]))
			{
				num += line[j];
				j++;
				if (num == "" || num == " " || num[0] == '\0')
					break;
				else if (stoi(num) < 1)
					break;
			}
			if (num == "" || num == " " || num[0] == '\0')
				continue;
			else if (stoi(num) < 1)
				continue;
			Line ln;
			ln.letterCount=stoi(num);
			ln.letters = line.substr(j+1);

			int i = 0;
			while(ln.letters[i] != '\0') //data formatting (for long strings with same letter sequences, ex: AAAA)
			{
				if (ln.letters[i] == ln.letters[i + 1] &&
					ln.letters[i + 1] == ln.letters[i + 2] &&
					ln.letters[i + 2] == ln.letters[i + 3])
				{
					int z = i + 1;
					int count = 0;
					while (ln.letters[z] == ln.letters[i])
					{
						ln.letters.erase(z, 1);
						++count;
					}
					ln.letters.insert(i+1, to_string(count));
					ln.formated = true;

					ln.formLocation += to_string(i + 1);
					ln.formLocation += ";";
					ln.formLength += to_string(count);
					ln.formLength += ";";
					ln.formLetter += ln.letters[i];
					ln.formLetter += ";";
					ln.formLengthSum += count;

					//ln.letterCount += to_string(count).length();
					ln.letterCount -= count;
				}
				++i;
			}

			lines.push_back(ln);
			lineCount++;
		}
	}
	file.close();

	sort(lines.begin(), lines.end(), sortFunction); //data sorting OPTIMIZATION

	/*for (int i = 0; i < lineCount ; i++) //for debugging
	{
		cout << lines.at(i).letters << endl;
	}*/

	for (int i = 0; i < lineCount-1; i++) //extra data and duplicate removal OPTIMIZATION
	{
		if (i == 0)
		{
			if ((lines.at(i).formLengthSum + lines.at(i).letterCount) - (lines.at(i+1).formLengthSum + lines.at(i+1).letterCount) > 1)
			{
				lines.erase(lines.begin());
				--lineCount;
			}
		}
		else if(i>0)
		{
			if ((lines.at(i).formLengthSum + lines.at(i).letterCount) - (lines.at(i+1).formLengthSum + lines.at(i+1).letterCount) > 1
				&& (lines.at(i-1).formLengthSum + lines.at(i-1).letterCount) - (lines.at(i).formLengthSum + lines.at(i).letterCount) > 1)
			{
				lines.erase(lines.begin() + i);
				--lineCount;
			}
		}
		if (lines.at(i).letters == lines.at(i + 1).letters)
		{
			lines.erase(lines.begin() + i);
			--lineCount;
		}
	}


	for (int i = 0; i < (lineCount - 1); i++) //going through lines
	{
		if (lines.at(i).chainNumber == -1)
		{
			lines.at(i).chainNumber = 0;
		}

		int nextLine = i + 1;

		//skipping lines with same length as current line
		while (((lines.at(i).formLengthSum + lines.at(i).letterCount) - (lines.at(nextLine).formLengthSum + lines.at(nextLine).letterCount)) == 0)
		{
			if (nextLine + 1 < lineCount)
				++nextLine;
			else
				break;
		}

		//going through lines wich are shorter than current line by 1
		while (((lines.at(i).formLengthSum + lines.at(i).letterCount) - (lines.at(nextLine).formLengthSum + lines.at(nextLine).letterCount)) == 1)
		{
			//jei sutampa ir lokacijos ir lengthai, tai tada tikrini su paprastu
			//jei sutampa lengthai, bet skirias lokacijos,tai tikrini su paprastu
			//jei sutampa lokacijos, bet skirias lengthai, tai tada atformatuoji abudu ir sulygini normaliu formatu
			//jei nesutampa nei lokacijos nei lengthai, tai tada nepraeis ir paprasto
			//jei nesutampa raides, tai nepraeina paprasto
			//jei tikrinamas formatuotas su neformatuotu visada bus skipinami

			if ((lines.at(i).formLengthSum - lines.at(nextLine).formLengthSum) == 1 &&
				lines.at(nextLine).formLetter == lines.at(i).formLetter) //if line is formatted (for ex: AAAAAA -> A5)
			{
				bool lengthCorrect = true;

				string temp2 = lines.at(i).letters;
				string temp3 = lines.at(nextLine).letters;

				std::string sLocation = lines.at(i).formLocation;
				std::string sLength1 = lines.at(i).formLength;
				std::string sLength2 = lines.at(nextLine).formLength;
				std::string sLetter = lines.at(nextLine).formLetter;
				std::string delimiter = ";";

				size_t pos = 0;
				std::string tokenLocation;
				std::string tokenLength1;
				std::string tokenLength2;
				std::string tokenLetter;


				int locationChange1 = 0;
				int locationChange2 = 0;

				//deformatting line:
				while ((pos = sLocation.find(delimiter)) != std::string::npos) {
					tokenLocation = sLocation.substr(0, pos);
					sLocation.erase(0, pos + delimiter.length());

					pos = sLetter.find(delimiter);
					tokenLetter = sLetter.substr(0, pos);
					sLetter.erase(0, pos + delimiter.length());

					pos = sLength1.find(delimiter);
					tokenLength1 = sLength1.substr(0, pos);
					sLength1.erase(0, pos + delimiter.length());

					pos = sLength2.find(delimiter);
					tokenLength2 = sLength2.substr(0, pos);
					sLength2.erase(0, pos + delimiter.length());

					int lngth = stoi(tokenLength1) - stoi(tokenLength2);

					temp2.erase(stoi(tokenLocation)+locationChange1, tokenLength1.length());
					temp3.erase(stoi(tokenLocation) + locationChange2, tokenLength2.length());

					//line deformatting logic:
					locationChange2 -= tokenLength2.length();
					int prevLocationChange = locationChange1;
					for (int i = 0; i < lngth; i++)
					{
						temp2.insert(stoi(tokenLocation) +prevLocationChange + i, tokenLetter);

						if (i > tokenLength1.length())
							++locationChange1;
					}

					if (lngth < tokenLength1.length())
						locationChange1= lngth - tokenLength1.length();

					if (lngth != 1 && lngth != 0)
					{
						lengthCorrect = false;
						break;
					}
				}
				if (lengthCorrect) //if line length difference meets the requirements (-1)
				{
					if ((temp2[0] == temp3[0] || temp2[1] == temp3[0]) ||/*&&*/
						(temp2[temp2.length()] == temp3[temp3.length()] || temp2[temp2.length() - 1] == temp3[temp3.length()])) //OPTIMIZATION for faster performance
					{
						string etalon = temp2;
						for (int j = 0; j < etalon.length(); j++) //going through letters
						{
							temp2 = etalon;
							temp2.erase(j, 1);

							if (temp2 == temp3)
							{
								if ((lines.at(i).chainNumber + 1) > (lines.at(nextLine).chainNumber))
								{
									lines.at(nextLine).chainNumber = lines.at(i).chainNumber + 1;
								}

								if (lines.at(nextLine).chainNumber > result)
								{
									result = lines.at(nextLine).chainNumber;
								}
								break;
							}
						}
					}
					else
						break;
				}
			}

			else if ((lines.at(i).formLengthSum - lines.at(nextLine).formLengthSum) == 0 &&
				lines.at(nextLine).formLetter == lines.at(i).formLetter) 	//if line is not formatted or formatting is irrelevant
				{
					string temp= lines.at(i).letters;
					int strLength = temp.length();
					string temp2 = lines.at(nextLine).letters;
					if ((temp[0] == temp2[0] || temp[1] == temp2[0]) ||/*&&*/ 
						(temp[temp.length()] == temp2[temp2.length()] || temp[temp.length()-1] == temp2[temp2.length()])) //OPTIMIZATION for faster performance		
					{
						for (int j = 0; j < strLength; j++) //going through letters
						{
							temp = lines.at(i).letters;
							temp.erase(j, 1);
							if (temp == temp2)
							{
								if ((lines.at(i).chainNumber + 1) > (lines.at(nextLine).chainNumber))
								{
									lines.at(nextLine).chainNumber = lines.at(i).chainNumber + 1;
								}

								if (lines.at(nextLine).chainNumber > result)
								{
									result = lines.at(nextLine).chainNumber;
								}
								break;
							}
						}
					}
					else
						break;
				}

			if (nextLine + 1 < lineCount) //if there are lines unchecked against the current
				++nextLine;
			else
				break;
		}
		
	}

	cout << "Result : " << result << endl;
}