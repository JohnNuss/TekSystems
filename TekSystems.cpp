#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

const int pictureIndex = 0;
const int speechIndex = 1;
const int maxBlankLines = 2;

class QuestionBlock
{
public:
	string QuestionName1;
	string QuestionName2;
	string QuestionName3;
	string Picture;
	string Speech;
	string Difficulty;
	string Flags;
	vector<string> Incompatibilities;
	string LegalReference;
	string ManualReference;
	string EffectiveDate;
	string IneffectiveDate;
	string QuestionText;
	string AnswerText1;
	string AnswerText2;
	string AnswerText3;
	string AnswerText4;
	string AnswerText5;
	string DescriptionText;
	int Correct;

	QuestionBlock()
	{
		QuestionName1 = "";
		QuestionName2 = "";
		QuestionName3 = "";
		Picture = "";
		Speech = "";
		Difficulty = "";
		Flags = "";
		LegalReference = "";
		ManualReference = "";
		EffectiveDate = "";
		IneffectiveDate = "";
		QuestionText = "";
		AnswerText1 = "";
		AnswerText2 = "";
		AnswerText3 = "";
		AnswerText4 = "";
		AnswerText5 = "";
		DescriptionText = "";
	}
};

vector<QuestionBlock> QuestionBlocks;

const string versionKey = "#ver";
const int whiteSpace = 32;
const char doubleQuote = '"';
const char perCent = '%';
const char correct = '*';
const string pictureKey = "pictures/";
const string speechesKey = "speeches/";

void getVersion(int *ptr, string *version, vector<string> lines)
{
	int vectorLength = lines.size();
	int keyLength = versionKey.length();
	bool versionFound = false;
	int idx = *ptr;
	while(idx < vectorLength && versionFound ==false)
	{
		string line = lines[idx++];
		int position = line.find(versionKey);
		if(position != string::npos)
		{
			*version = line.substr(position + keyLength);
			versionFound = true;
		}
	}
	*ptr = idx;
}

vector<string> readFile(string pathName)
{
	string lineIn;
	vector<string> linesRead;
	ifstream fileIn(pathName);
	if(fileIn.is_open())
	{
		while(getline(fileIn, lineIn))
			linesRead.push_back(lineIn);
		fileIn.close();
	}
	return linesRead;
}

void extractQuestions1_2_3(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();

	if(idx <lineLength)
	{
		char c = lineToBeParsed[idx++];
		while(idx < lineLength && c == whiteSpace)
		{
			c = lineToBeParsed[idx++];
		}

		while(idx < lineLength && c != whiteSpace)
		{
			questionBlock->QuestionName1 += c;
			c = lineToBeParsed[idx++];
		}

		while(idx < lineLength && c == whiteSpace)
		{
			c = lineToBeParsed[idx++];
		}

		while(idx < lineLength && c != whiteSpace)
		{
			questionBlock->QuestionName2 += c;
			c = lineToBeParsed[idx++];
		}

		while(idx < lineLength && c == whiteSpace)
		{
			c = lineToBeParsed[idx++];
		}

		while(idx <= lineLength && c != whiteSpace)
		{
			questionBlock->QuestionName3 += c;
			c = lineToBeParsed[idx++];
		}
	}
}

void extractPictureAndSpeech(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int position = lineToBeParsed.find(pictureKey);
	if(position != string::npos)
	{
		int keyLength = pictureKey.length();
		int lineLength = lineToBeParsed.length();
		int idx = position + keyLength;
		char c = lineToBeParsed[idx++];
		while(idx < lineLength && c != whiteSpace)
		{
			questionBlock->Picture += c;
			c = lineToBeParsed[idx++];
		}
	}

	position = lineToBeParsed.find(speechesKey);

	if(position != string::npos)
	{
		int keyLength = speechesKey.length();
		int lineLength = lineToBeParsed.length();
		int idx = position + keyLength;
		char c = lineToBeParsed[idx];
		while(idx < lineLength && c != whiteSpace)
		{
			questionBlock->Speech += c;
			c = lineToBeParsed[idx++];
		}
	}	
}

void extractDifficultyFlags(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int lineLength = lineToBeParsed.length();
	int idx = 0;

	string difficulty = "";
	string flags = "";

	char c = lineToBeParsed[idx++];

	while(idx < lineLength && c != whiteSpace)
	{
		if(c >= '0' && c <= '9')
			difficulty += c;
		else
			flags+=c;
		c = lineToBeParsed[idx++];
	}
	while(idx < lineLength && c == whiteSpace)
	{
		c = lineToBeParsed[idx++];
	}

	questionBlock->Difficulty = difficulty;
	questionBlock->Flags = flags;

	vector<string> incompatibilities;
	string incompatibility = "";
	bool incompatibilitiesComplete = false;
	bool legalRefWriting = false;
	bool manualRefWriting = false;
	bool legalRefComplete = false;
	bool manualRefComplete = false;
	int numberQuotes = 0;
	bool insideQuotes = false;

	while(!incompatibilitiesComplete && idx < lineLength)
	{
		char cNext = c;
		if(idx + 1 < lineLength)
			cNext = lineToBeParsed[idx + 1];
		if(idx < lineLength)
		{
			if(c != whiteSpace && c!= doubleQuote)
			{
				incompatibility += c;
				c = lineToBeParsed[idx++];
			}
			else if(c == whiteSpace && cNext != perCent)
			{
				c = lineToBeParsed[idx++];
				questionBlock->Incompatibilities.push_back(incompatibility);
				incompatibility = "";
			}
			else if(c == whiteSpace && cNext == perCent)
			{
				c = lineToBeParsed[idx++];
				questionBlock->Incompatibilities.push_back(incompatibility);
				incompatibility = "";
				incompatibilitiesComplete = true;
			}
		
			else if(c == doubleQuote && cNext != perCent)
			{
				incompatibility = "";
				incompatibilitiesComplete = true;
			}
			else if(c == doubleQuote && cNext == perCent)
			{
				c = lineToBeParsed[idx++];
				incompatibility = "";
				incompatibilitiesComplete = true;
			}
		}
	}
	
	while(idx < lineLength && legalRefComplete == false && !legalRefWriting)
		{
			if(c == doubleQuote && insideQuotes == false)
			{
				insideQuotes = true;
				c = lineToBeParsed[idx++];
			}
			else if(c == doubleQuote && insideQuotes == true)
			{
				insideQuotes = false;
				c = lineToBeParsed[idx++];
			}
			else if(c == perCent)
			{
				legalRefWriting = true;
				c = lineToBeParsed[idx++];
			}
			else
			{
				c = lineToBeParsed[idx++];
			}
		}
	while(idx < lineLength && legalRefWriting && !legalRefComplete)
			{
				if(c == doubleQuote && insideQuotes == true)
				{
					insideQuotes = false;
					legalRefComplete = true;
					legalRefWriting = false;
					c = lineToBeParsed[idx++];
				}
				else if(c != doubleQuote && insideQuotes == true)
				{
					questionBlock->LegalReference += c;
					c = lineToBeParsed[idx++];
				}
				else if(c != whiteSpace && !insideQuotes)
				{
					questionBlock->LegalReference += c;
					c = lineToBeParsed[idx++];
				}
				else if(c == whiteSpace && !insideQuotes)
				{
					legalRefComplete = true;
					legalRefWriting = false;
					c = lineToBeParsed[idx++];
				}

			}
	while(idx < lineLength && manualRefComplete == false && !manualRefWriting)
		{
			if(c == doubleQuote && insideQuotes == false)
			{
				insideQuotes = true;
				c = lineToBeParsed[idx++];
			}
			else if(c == doubleQuote && insideQuotes	== true)
			{
				insideQuotes = false;
				c = lineToBeParsed[idx++];
			}
			else if(c == perCent)
			{
				manualRefWriting = true;
				c = lineToBeParsed[idx++];
			}
			else
			{
				c = lineToBeParsed[idx++];
			}
		}
	while(idx <= lineLength && manualRefWriting && !manualRefComplete)
			{
				if(c == doubleQuote && insideQuotes == true)
				{
					insideQuotes = false;
					manualRefComplete = true;
					manualRefWriting = false;
					c = lineToBeParsed[idx++];
				}
				else if(c != doubleQuote && insideQuotes == true)
				{
					questionBlock->ManualReference += c;
					c = lineToBeParsed[idx++];
				}
				else if(c != whiteSpace && !insideQuotes)
				{
					questionBlock->ManualReference += c;
					c = lineToBeParsed[idx++];
				}
				else if(c == whiteSpace && !insideQuotes)
				{
					manualRefComplete = true;
					manualRefWriting = false;
					insideQuotes = false;
					c = lineToBeParsed[idx++];
				}

			}
}

void extractEffectiveDate(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();
	char c = lineToBeParsed[idx++];
	if(idx <lineLength)
	{
		while(idx < lineLength && c == whiteSpace)
		{
			c = lineToBeParsed[idx++];
		}

		while(idx < lineLength && c != whiteSpace)
		{
			questionBlock->EffectiveDate += c;
			c = lineToBeParsed[idx++];
		}

		while(idx < lineLength && c == whiteSpace)
		{
			c = lineToBeParsed[idx++];
		}

		while(idx <= lineLength && c != whiteSpace)
		{
			questionBlock->IneffectiveDate += c;
			c = lineToBeParsed[idx++];
		}
		}
}

void extractQuestionText(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();

	if(idx <lineLength)
	{
		char c = lineToBeParsed[idx++];
		while(idx <= lineLength)
		{
			questionBlock->QuestionText += c;
			c = lineToBeParsed[idx++];
		}

	}
}

void extractAnswerText1(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();
	char c = lineToBeParsed[idx];
	c = lineToBeParsed[idx++];
	while(idx <= lineLength)
	{
		if(c == correct && idx == 1)
		{
		questionBlock->Correct = 1;
		c = lineToBeParsed[idx++];
		}
		else
		{
		questionBlock->AnswerText1 += c;
		c = lineToBeParsed[idx++];
		}
	}
}

void extractAnswerText2(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();
	char c = lineToBeParsed[idx];
	c = lineToBeParsed[idx++];
	while(idx <= lineLength)
	{
		if(c == correct && idx == 1)
		{
		questionBlock->Correct = 2;
		c = lineToBeParsed[idx++];
		}
		else
		{
		questionBlock->AnswerText2 += c;
		c = lineToBeParsed[idx++];
		}
	}
}

void extractAnswerText3(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();
	char c = lineToBeParsed[idx];
	c = lineToBeParsed[idx++];
	while(idx <= lineLength)
	{
		if(c == correct && idx == 1)
		{
		questionBlock->Correct = 3;
		c = lineToBeParsed[idx++];
		}
		else
		{
		questionBlock->AnswerText3 += c;
		c = lineToBeParsed[idx++];
		}
	}
}

void extractAnswerText4(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();
	char c = lineToBeParsed[idx];
	c = lineToBeParsed[idx++];
	while(idx <= lineLength)
	{
		if(c == correct && idx == 1)
		{
		questionBlock->Correct = 4;
		c = lineToBeParsed[idx++];
		}
		else
		{
		questionBlock->AnswerText4 += c;
		c = lineToBeParsed[idx++];
		}
	}
}

void extractAnswerText5(string lineToBeParsed, QuestionBlock *questionBlock)
{
	int idx = 0;
	int lineLength = lineToBeParsed.length();
	char c = lineToBeParsed[idx];
	c = lineToBeParsed[idx++];
	while(idx <= lineLength)
	{
		if(c == correct && idx == 1)
		{
		questionBlock->Correct = 5;
		c = lineToBeParsed[idx++];
		}
		else
		{
		questionBlock->AnswerText5 += c;
		c = lineToBeParsed[idx++];
		}
	}
}

void extractDescriptor(string lineToBeParsed, QuestionBlock *questionBlock)
{
	questionBlock->DescriptionText = lineToBeParsed;
}

int main()
{

	string fileName = "Tek Systems.txt";
	vector<string> airBrakeSumText = readFile(fileName);
	int airBrakeIndex = 0;
	string versionValue = "";
	getVersion(&airBrakeIndex, &versionValue, airBrakeSumText);

	int textSize = airBrakeSumText.size();
	//airBrakeIndex = 212;
	while(airBrakeIndex < textSize)
	{
		QuestionBlock questionBlock;
		int blankLines = 0;
		string line = airBrakeSumText[airBrakeIndex];
		if(line.length() > 0)
			extractQuestions1_2_3(line, &questionBlock);
		else
			blankLines++;

		if(airBrakeIndex < textSize)
			airBrakeIndex++;
		line = airBrakeSumText[airBrakeIndex];
		if(line.length() > 0)
			extractPictureAndSpeech(line, &questionBlock);
		else
			blankLines++;

		if(airBrakeIndex < textSize)
			airBrakeIndex++;
		line = airBrakeSumText[airBrakeIndex];
		if(line.length() > 0)
			extractDifficultyFlags(line, &questionBlock);
		else
			blankLines++;

		if(airBrakeIndex < textSize)
			airBrakeIndex++;
		line = airBrakeSumText[airBrakeIndex];
		if(line.length() > 0)
			extractEffectiveDate(line, &questionBlock);
		else 
			blankLines++;

		if(airBrakeIndex < textSize)
			airBrakeIndex++;
		line = airBrakeSumText[airBrakeIndex];
		if(line.length() > 0)
			extractQuestionText(line, &questionBlock);
		else 
			blankLines++;

		if(blankLines < maxBlankLines)
		{
			if(airBrakeIndex < textSize)
				airBrakeIndex++;
			line = airBrakeSumText[airBrakeIndex];
			if(line.length() > 0)
				extractAnswerText1(line, &questionBlock);
		}

		if(blankLines < maxBlankLines)
		{
			if(airBrakeIndex < textSize)
				airBrakeIndex++;
			line = airBrakeSumText[airBrakeIndex];
			if(line.length() > 0)
				extractAnswerText2(line, &questionBlock);
		}

		if(blankLines < maxBlankLines)
		{
			if(airBrakeIndex < textSize)
				airBrakeIndex++;
			line = airBrakeSumText[airBrakeIndex];
			if(line.length() > 0)
				extractAnswerText3(line, &questionBlock);
		}

		if(blankLines < maxBlankLines)
		{
			if(airBrakeIndex < textSize)
				airBrakeIndex++;
			line = airBrakeSumText[airBrakeIndex];
			if(line.length() > 0)
				extractAnswerText4(line, &questionBlock);
		}

		if(blankLines < maxBlankLines)
		{
			if(airBrakeIndex < textSize)
				airBrakeIndex++;
			line=airBrakeSumText[airBrakeIndex];
			if(line.length() > 0)
				extractAnswerText5(line, &questionBlock);
		}

		while(line.length() == 0 && airBrakeIndex < textSize)
		{
			airBrakeIndex++;
			line = airBrakeSumText[airBrakeIndex++];

		}
		if(line.length() > 0)
		{
			extractDescriptor(line, &questionBlock);
		}

		QuestionBlocks.push_back(questionBlock);

		if(airBrakeIndex < textSize)
		{
			airBrakeIndex++;
			line = airBrakeSumText[airBrakeIndex];

			while(line.length() == 0 && airBrakeIndex < textSize)
			{
				airBrakeIndex++;
				line=airBrakeSumText[airBrakeIndex];
			}
		}

	}

	unsigned int milliseconds = 500;
	for(int idx = 0; idx < QuestionBlocks.size(); idx++)
	{
		QuestionBlock questionBlock = QuestionBlocks[idx];
		if(questionBlock.QuestionName1.length() != 0)
			std::cout << "Question Name: " + questionBlock.QuestionName1 + " " + questionBlock.QuestionName2  + " " + questionBlock.QuestionName3 << std::endl;
		if(questionBlock.Picture.length() != 0)
			std::cout << "Picture: " + questionBlock.Picture << std::ends;
		if(questionBlock.Speech.length() != 0)
			std::cout << " Speech: " + questionBlock.Speech << std::endl;
		else
		if(questionBlock.Difficulty.length() != 0)
			std::cout << "Difficulty: " + questionBlock.Difficulty << std::endl;
		else
		{
			std::cout << "Difficulty: None" + questionBlock.Flags << std::endl;
		}
		if(questionBlock.Flags.length() != 0)
		{
		std::cout << "Flags: " + questionBlock.Flags << std::endl;
		}
		else
		{
			std::cout << "Flags: None" << std::endl;
		}
		if(questionBlock.Incompatibilities.size() > 0)
		{
			std::cout << "Incompatibilities:" << std::endl;
			for(int id = 0; id < questionBlock.Incompatibilities.size(); id++)
			{
				std::cout << "	" + questionBlock.Incompatibilities[id] << std::endl;
			}
		}
		else
		{
			std::cout << "Incompatibilities: None" << std::endl;
		}
		if(questionBlock.LegalReference.size() > 0)
			std::cout << "Legal Ref: " + questionBlock.LegalReference << std::ends;
		if(questionBlock.ManualReference.size() > 0)
			std::cout << " Manual Ref: " + questionBlock.ManualReference << std::endl;
		else
			std::cout << " " << std::endl;
		
		if(questionBlock.EffectiveDate.size() > 0)
			std::cout << "Effective Date: " + questionBlock.EffectiveDate << std::ends;
		if(questionBlock.IneffectiveDate.size() > 0)
			std::cout << " Ineffective Date: " + questionBlock.IneffectiveDate << std::endl;
		else
			std::cout << " " << std::endl;
		std::cout << "	" << std::endl;
		if(questionBlock.QuestionText.size() > 0)
		std::cout << "Question Text: " + questionBlock.QuestionText << std::endl;
		if(questionBlock.AnswerText1.size() > 0)
		{
			std::cout << "Answers: " << std::endl;
			std::cout << "	" + questionBlock.AnswerText1 << std::endl;
		}
		if(questionBlock.AnswerText2.size() > 0)
			std::cout << "	" + questionBlock.AnswerText2 << std::endl;
		if(questionBlock.AnswerText3.size() > 0)
			std::cout << "	" + questionBlock.AnswerText3 << std::endl;
		if(questionBlock.AnswerText4.size() > 0)
			std::cout << "	" + questionBlock.AnswerText4 << std::endl;
		if(questionBlock.AnswerText5.size() > 0)
			std::cout << "	" + questionBlock.AnswerText5 << std::endl;
		std::cout << "	" << std::endl;
		std::cout << "__________________________________________________" << std::endl;
		std::cout << "	" << std::endl;
		switch (questionBlock.Correct)
		{
		case 1:
			std::cout << "Correct Answer: " + questionBlock.AnswerText1 << std::endl;
			break;
		case 2:
			std::cout << "Correct Answer: " + questionBlock.AnswerText2 << std::endl;
			break;
		case 3:
			std::cout << "Correct Answer: " + questionBlock.AnswerText3 << std::endl;
			break;
		case 4:
			std::cout << "Correct Answer: " + questionBlock.AnswerText4 << std::endl;
			break;
		case 5:
			std::cout << "Correct Answer: " + questionBlock.AnswerText5 << std::endl;
			break;
		}
		std::cout << "	" << std::endl;
		std::cout << "__________________________________________________" << std::endl;
		std::cout << "	" << std::endl;
		Sleep(milliseconds);
	}
	milliseconds = 200;
	Sleep(milliseconds);

	getchar();

}