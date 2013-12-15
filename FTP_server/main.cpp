#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <windows.h>
#include "main.h"
#include <conio.h>
#include <filesystem>

using namespace std;
using namespace std::tr2::sys;

string publicip; //will hold the url's contents

std::string editannouncement();
std::string editcolor();
void getFoldersize(string rootFolder, float & f_size);

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        publicip.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}


int main()
{
	HWND hWnd = GetConsoleWindow();
	//ShowWindow(hWnd, SW_HIDE);

	while(1)
	{
	publicip = "";
	SetConsoleTitle( TEXT("lel im hacking your computer XD"));

    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://wtfismyip.com/text");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);

	cout << endl << "Public IP : ";
	Sleep(10);
    cout << publicip << endl;
	Sleep(10);

    curl_easy_cleanup(curl);
    curl_global_cleanup();

	if (publicip == "Service Unavailable")
	{
		publicip = "";

		CURL* backupcurl; //our curl object

		curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
		backupcurl = curl_easy_init();

		curl_easy_setopt(backupcurl, CURLOPT_URL, "http://www.telize.com/ip");
		curl_easy_setopt(backupcurl, CURLOPT_WRITEFUNCTION, &writeCallback);
		curl_easy_setopt(backupcurl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

		curl_easy_perform(backupcurl);

		cout << endl << "Public IP : ";
		Sleep(10);
		cout << publicip << endl;
		Sleep(10);

		curl_easy_cleanup(backupcurl);
		curl_global_cleanup();
	}

	SetConsoleTitle( TEXT("FuckThePolice Server"));
	
		ClearScreen();
		cout << "Loading Data...";
		float  f_size=0;
		getFoldersize("C://Downloads",f_size);
		ClearScreen();
		f_size = f_size / (1073741824);
		std::string f_size_str = std::to_string(f_size);

		std::ifstream ServerData("C://Dropbox//Public//FTP-Server//data.txt");

		//	Read colorcode
		std::string colorcode_str;
		std::getline(ServerData, colorcode_str);
		int colorcode = atoi(colorcode_str.c_str());

		//	Read actual announcement
		std::string announcement_str;
		std::getline(ServerData, announcement_str);
		ServerData.close();

		cout << "Current Announcement :";

		//	Color change
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, colorcode);
		cout << " " << announcement_str << endl;

		//	Change to Normal
		SetConsoleTextAttribute(hConsole, 7);
		cout << "Size of all data : " << f_size_str << " GB";

		std::ofstream outputData("C://Dropbox//Public//FTP-Server//data.txt");
		outputData << colorcode_str << endl << announcement_str << endl << f_size;
		outputData.close();
		std::ofstream outputip("C://Dropbox//Public//FTP-Server//ip.txt");
		outputip << publicip;
		outputip.close();

		cout << endl << endl << "1 : Edit Color" << endl;
		cout << "2 : Edit Announcement" << endl;
		cout << "0 : Quit" << endl;

		char nig = 'A';
		nig = _getch();

		switch (nig)
		{
		case '1':
			colorcode_str = editcolor();

			outputData.open("C://Dropbox//Public//FTP-Server//data.txt");
			outputData.clear();
			outputData.seekp(0);
			outputData << colorcode_str << endl << announcement_str << endl << f_size;
			outputData.close();
			break;
		case '2':
			announcement_str = editannouncement();

			outputData.open("C://Dropbox//Public//FTP-Server//data.txt");
			outputData.clear();
			outputData.seekp(0);
			outputData << colorcode_str << endl << announcement_str << endl << f_size;
			outputData.close();
			break;
		case '0':
			return 0;
		default:
			break;
		}
	}
    return 0;
}

std::string editcolor()
{
	cout << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 255; i++)
	{
		SetConsoleTextAttribute(hConsole, i);
		cout << " " << i << " ";
	}
	cout << endl;
	SetConsoleTextAttribute(hConsole, 7);
	std::string newcolorcode;
	std::cin >> newcolorcode;
	return newcolorcode;
}

std::string editannouncement()
{
	cout << endl << "New Announcement : ";
	std::string new_announcement;
	std::getline(cin, new_announcement);
	return new_announcement;
}

void getFoldersize(string rootFolder, float & f_size)
{
	path folderPath(rootFolder);                      
	if (exists(folderPath))
	{
		directory_iterator end_itr;
		for (directory_iterator dirIte(rootFolder); dirIte != end_itr; ++dirIte )
		{
			path filePath(complete (dirIte->path(), folderPath));
			try{
					if (!is_directory(dirIte->status()) )
					{
						f_size = f_size + file_size(filePath);                      
					}else
					{
						getFoldersize(filePath,f_size);
					}
				}catch(exception& e){  /*cout << e.what() << endl;*/ }
			}
	}
 }