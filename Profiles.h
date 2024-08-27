#pragma once
#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;
class Profile
{
	string name, email, password; // Gmail must be unique 
	std::streampos position;	  // Position of the line at which user data is stored
	friend class MainMenu;

public:
	void menu(); //Menu presented to the user
	bool signUp(); //Returns 1 if the signUp was successful
	bool logIn(); //Returns 1 if the logIn was successful
	bool isEmailValid();  //Returns 1 if the password pattern was valid
	bool isNameValid();   //Returns 1 if the name pattern was valid 
	bool isPasswordValid();  //Returns 1 the password pattern was valid
	void changeInfo();
	string getName(){
		return name;
	}
};
bool Profile::signUp()
	{ // If signUp is successful
		system("cls");
		cout << "Sign In\nEnter your name: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		getline(cin, name);
		cout << "Enter your gmail(something@gmail.com | something@hotmail.com): ";
		cin >> email;
		cout << "Enter your password(must contain special character, character and number and atleast length of six): ";
		cin >> password;
		if (!isEmailValid() || !isPasswordValid() || !isNameValid()) //Check if the credentials meet the requirements
		{
			cout << "Invalid credentials";
			getch();
			return 0;
		}
		fstream file("Profiles.txt", ios::in);
		file.seekg(0, std::ios::beg);
		string line;
		getline(file, line); // To remove the first line since it contains headers
		while (getline(file, line))
		{
			string emailInFile;
			std::istringstream ss(line);
			getline(ss, emailInFile, ','); // Skip name
			getline(ss, emailInFile, ','); 
			if (emailInFile == this->email)
			{
				cout << "User alredy exists" << endl;
				Sleep(1000);
				file.close();
				return 0; //Sign up was unsuccessful
			}
		}
		file.close();
		file.open("Profiles.txt", ios::app);
		file<<endl;
		position = file.tellg();
		file << name << "," << email << "," << password << ",";
		file.close();
		return 1;
	}
	bool Profile::logIn()
	{ // If login is successful
		system("cls");
		cout << "Login\nEnter your gmail: ";
		cin >> email;
		cout << "Enter your password: ";
		cin >> password;
		ifstream file("Profiles.txt");
		file.seekg(0, std::ios::beg);
		string line;
		getline(file, line); // To remove the first row
		bool userFoundInFile = 0; //If the user is found in the file
		streampos tempPos;
		while (1)
		{
			tempPos = file.tellg();
			if (!getline(file, line))
				break;
			std::istringstream ss(line);
			string nameInFile, emailInFile, passwordInFile;
			getline(ss, nameInFile, ',');
			getline(ss, emailInFile, ',');
			getline(ss, passwordInFile, ',');
			if (emailInFile == email && passwordInFile == password) //If email and password matches 
			{
				position = tempPos;
				name = nameInFile;
				userFoundInFile = 1;
				break;
			}
		}
		if (!userFoundInFile)
		{
			cout << "User does not exist";
			Sleep(1000);
		}
		file.close();
		return userFoundInFile;
	}
	void Profile::menu()
	{
		while (1)
		{
			system("cls");
			cout << "1. Signup (Create a account)\n2. LogIn\n3. exit" << endl;
			char c = getch();
			if (c == '1' && signUp()) // After signing up user will asked again to login
				return;
			else if (c == '2' && logIn())
				return;
			else if (c == '3')
				exit(1);
		}
	}
	void Profile::changeInfo(){
		system("cls");
		string password, name;
		cout<<"Enter current password: ";
		cin>>password;
		if(password != this->password){
			cout<<"Invalid ";
			Sleep(1000);
			return;
		}
		cout<<"Enter name: ";
	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		getline(cin, this->name);
		cout<<"Enter new password: ";
		cin>>password;
		string temp = this->password;
		this->password = password;
		if(!isPasswordValid()){
			this->password = temp;
			cout<<"Invalid password";
			Sleep(1000);
		}
	}
	bool Profile::isEmailValid()
	{
		std::regex pattern(R"([a-zA-Z].*@(hotmail|gmail\.com))");
		return std::regex_match(email, pattern);
	}
	bool Profile::isPasswordValid()
	{
		std::regex pattern("^(?=.*[a-zA-Z])(?=.*\\d)(?=.*[\\W_]).{6,}$");
		return std::regex_match(password, pattern);
	}
	bool Profile::isNameValid()
	{
		std::regex pattern(R"([a-zA-Z][a-zA-Z ]*)");
		return std::regex_match(name, pattern);
	}
