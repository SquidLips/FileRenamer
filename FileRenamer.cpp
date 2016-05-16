// FileRenamer.cpp : Defines the entry point for the console application.
//
/* list contents of a directory */
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include "dirent.h"
#include <iostream>
#include <time.h>

using namespace std;

string** GetFormat(int aParts);//Automated process for building the format of the new file naming
string BuildName(string** aFormat, int aSeqNum, int aParts, string aOldName);//Uses collected information to build the new file name

int main(int argc, char *argv[]) 
{
	DIR *dir;
	struct dirent *ent;

	/* open directory stream */
	dir = opendir(""); // opens the folder the executable is placed in
	if (dir != NULL) {
		int i = 0;
		string newName = "";
		string oldName = "";

		ent = readdir(dir);
		ent = readdir(dir);// skips the first two entries which were always garbage in tests

		string** format = 0;
		int parts = 0;
		cout << "Enter number of parts" << endl; //used to setup GetFormat
		cin >> parts;
		format = GetFormat(parts);

		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			oldName = ent->d_name;
			if (oldName.find('.') != UINT_MAX && oldName != "FileRenamer.exe")//Detects if the current name is a folder or the file renamer itself. NOTE: doesn't work if filenames include any period aside of for the extension
			{
				newName = BuildName(format, i, parts, oldName);
				cout << oldName << " changed to " << newName << endl;//Output changes for debugging and confirmation
				rename(oldName.c_str(), newName.c_str());
			}
			else if (oldName == "FileRenamer.exe")
				cout << oldName << " (Skipping self)" << endl;
			else
				cout << oldName << " (folder skipped)" << endl;
			++i;
		}
		for (int i = 0; i < parts; ++i)
		{
			delete format[i];
		}
		delete[] format;
		system("pause");
		closedir(dir);
	}
	return 0;
}

string** GetFormat(int aParts) //Automated process for building the format of the new file naming
{
	int choice = 0;
	string** tFormat = new string*[aParts];
	for (int i = 0; i < aParts; ++i)
	{
		tFormat[i] = new string;
	}
	for (int i = 0; i < aParts; ++i)//loop for building the new file format
	{
		cout << "Enter number for type for element number " << i << endl << "1.Custom Text" << endl << "2.Sequential Number" << endl << "3.Old File Name" << endl;
		cin >> choice;
		switch (choice)
		{
			case 1:
				cout << "Enter Text" << endl;
				cin >> *tFormat[i];
				break;
			case 2:
				cout << "Added sequential number" << endl;
				*tFormat[i] = "<num>";
				break;
			case 3:
				cout << "added old file name" << endl;
				*tFormat[i] = "<oName>";
				break;
			default:
				cout << "invalid selection" << endl;
				--i;
				break;
		}
	}
	return tFormat;
}

string BuildName(string** aFormat, int aSeqNum, int aParts, string aOldName)//Uses collected information to build the new file name
{
	string result = "";
	for (int i = 0; i < aParts; ++i)//Detect the type/content of each part and combine them into a string
	{
		if (*aFormat[i] == "<num>")
			result += to_string(aSeqNum);
		else if (*aFormat[i] == "<oName>")
			result += aOldName.substr(0, aOldName.find('.'));
		else
			result += *aFormat[i];
	}
	result += aOldName.substr(aOldName.find('.'));
	return result;
}
