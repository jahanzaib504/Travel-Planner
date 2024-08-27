/* Welcome to travel planner
This app helps user organize their trips
Finding best route for the trip using modfied dijkstra algorithm
It uses various data structures which are as follows:
unordered_set :  //Giving O(1) searching capability
Graphs using unordered_map:   //For making the adjacency list of the cities
lists:   //Insertion and deletion is easy as compared to vectors

It has following features:
1. Maintaining trip history and profile using filing
2. Choosing optimal path for journey
3. Map and navigation of trip: 	User current location
4. Sorting trips based on name and date and searching of trips
5. Trip deletion from history
6. Trip interuption: Allows the user to change path while in the middle of journey
*/
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <regex>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <string>
#include "Profiles.h"
#include "Graph.h"
#include "Trip.h"
using namespace std;
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// comparision function for sorting list on the basis of date and name
bool compare_date(Trip &trip1, Trip &trip2)
{
	return trip1.getDeparture() < trip2.getDeparture();
}
bool compare_name(Trip &trip1, Trip &trip2)
{
	return trip1.getTripName() < trip2.getTripName();
}

class MapAndNavigation : public Graph
{
public:
	void menu(Trip &trip)
	{
		system("cls");
		// Loading current system date
		auto currentTimePoint = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);
		std::tm *currentDate = std::localtime(&currentTime);

		string departure = to_string(currentDate->tm_year + 1900) + "-" + to_string(currentDate->tm_mon + 1) + "-" + to_string(currentDate->tm_mday);
		trip.setDeparture(departure); // Setting departure
		trip.setCurrentCity(trip.getPath().begin());
		trip.setTripStarted(1);

		int sec = 0, avg = 100, distanceBetweenTwoCities = get_dis(*trip.getPath().begin(), *next(trip.getPath().begin(), 1)); // Time of journey
		int timeRequiredFor = (distanceBetweenTwoCities / avg) * 60 * 60, tSecs = 0,    // Time requred for each traveling between two nodes
		inc = 5000;			//	Incrementer 						
		float totalDistance = 0;
		bool pause = 0;
		trip.setCurrentCity(next(trip.getPath().begin(), 1));
		while (trip.getCurrentCity() != trip.getPath().end())
		{
			system("cls");
			cout << "--Menu--- p : pause --- c : change plan --- e : end trip---" << endl;
			cout << endl
				 << "Current City---" << getKey(*trip.getCurrentCity()) << "---Time of journey---" << (sec / 60) / 60 << "h " << ((sec / 60) % 60) << "m " << sec % 60 << "s" << endl;
			cout << endl
				 << "Average Speed: " << avg << endl;
			cout << "Distance between: " << getKey(*prev(trip.getCurrentCity(), 1)) << " " << getKey(*(trip.getCurrentCity())) << ": "
				 << distanceBetweenTwoCities << "Km" << endl;
			cout << "Estimated Time Required in hours: " << (timeRequiredFor / 60) / 60 << endl;
			cout << ((pause) ? "Journey Paused" : "") << endl;
			if (tSecs >= timeRequiredFor)
			{
				tSecs = 0;
				totalDistance += distanceBetweenTwoCities;
				distanceBetweenTwoCities = get_dis(*trip.getCurrentCity(), *next(trip.getCurrentCity(), 1));
				timeRequiredFor = (distanceBetweenTwoCities / 100) * 60 * 60;
				trip.setCurrentCity(next(trip.getCurrentCity(), 1));
			}
			Sleep(1000);
			sec += inc; // If you want to complete the journey in lesser time
			if (!pause)
				tSecs += inc;
			if (kbhit())
			{
				char c = getch();
				if (c == 'p')
					pause = !pause;
				else if (c == 'c')
					trip.changeTripPlan();
				else if (c == 'e')
					break;
			}
		}
		auto arrivalTimePoint = std::chrono::system_clock::now();
		std::time_t arrivalTime = std::chrono::system_clock::to_time_t(arrivalTimePoint);
		std::tm *arrivalDate = std::localtime(&arrivalTime);

		string arrival = to_string(arrivalDate->tm_year + 1900) + "-" + to_string(arrivalDate->tm_mon + 1) + "-" + to_string(arrivalDate->tm_mday);
		trip.setArrival(arrival);
		trip.setTimeOfJourney(sec);
	}
	int get_dis(int a, int b) //For getting distance between two cities in a graph
	{
		int dis = 0;
		for (auto it : Map[a])
		{
			if (it.first == b)
			{
				dis = it.second;
				break;
			}
		}
		return dis;
	}
};
class MainMenu : public Graph
{
	list<Trip> currentTrips, tripHistory; // The trips currently made by the user and hsitory
	MapAndNavigation navigate; //For current location
	Profile profile;  //User profile informtion

public:
	void menu();			  // Main menu
	void navBar();			  // Inner menu
	void printCurrentTrips(); // printing current trips
	void printHistory();	  // printing history of trips
	void loadTripHistory();	  // For loading trip history from file
	void logOut();			  // Will save changes when the user exit
	void startMapping();	  // Will load trip selected by the user to the map and navigation
	void createTrip();		  // For creating a new trip
};
void MainMenu::menu()
{
	while (1)
	{
		profile.menu(); // For logging in and signing up
		loadTripHistory();
		navBar();
		tripHistory.clear();
		currentTrips.clear();
	}
}
void MainMenu::navBar()
{
	while (1)
	{
		system("cls");
		cout << "Welcome " << profile.getName() << endl;
		cout << "1. Create Trip\n2. Trip History\n3. Map and navigation\n4. Current Trips\n5. Change Login information\n6. Graph of cities\n7. Log Out\n\nNote: Do not turn off this page log out to save your changes";
		char c = getch();
		switch (c)
		{
		case '1':
			createTrip();
			break;
		case '2':
			printHistory();
			break;
		case '3':
			startMapping();
			break;
		case '4':
			printCurrentTrips();
			break;
		case '5':
			profile.changeInfo();
			break;
		case '6':
			printGraph();
			break;
		case '7':
			logOut();
			return;
			break;
		}
	}
}
void MainMenu::printCurrentTrips()
{
	char choice;
	while (1)
	{
		system("cls");
		cout << "Press s to select a trip and any to go back: " << endl;
		int count = 0;
		for (auto itor : currentTrips)
		{
			count++;
			cout << "Trip " << count << endl;
			itor.tripDetails();
			cout << endl;
			for (int i = 0; i < 40; i++)
				cout << "*";
			cout << endl;
		}
		char c = getch();
		if (c != 's')
			break;

		cout << "\nEnter trip number: ";
		int i = 0;
		cin >> i;
		if (i < 1 || i > currentTrips.size())
			break;
		i--;
		auto it = next(currentTrips.begin(), i);
		it->menu();
	}
}
void MainMenu::loadTripHistory()
{ // compeleted trips by the user
	ifstream file("Profiles.txt");
	file.seekg(profile.position);
	string line, var;
	getline(file, line);
	istringstream ss(line);
	for (int i = 0; i < 3; i++)
		getline(ss, var, ',');
	// Loading all compelete trips
	while (getline(ss, var, ','))
	{
		Trip trip;
		trip.setDeparture(var);
		getline(ss, var, ',');
		trip.setArrival(var);
		getline(ss, var, ',');
		trip.setTripName(var);
		getline(ss, var, ',');
		trip.setTimeOfJourney(stoi(var));
		getline(ss, var, ',');
		trip.setOneWay(var[0] - '0');
		getline(ss, var, ',');
		trip.setSourceCity(stoi(var));
		getline(ss, var, ','); // List of cities in plan
		istringstream newSS(var);
		string inside;
		while (getline(newSS, inside, ' '))
			trip.getVisitedCities().insert(stoi(inside));

		getline(ss, var, ','); // List of cities in plan
		istringstream SS(var);
		while (getline(SS, inside, ' '))
			trip.getPath().push_back(stoi(inside));

		tripHistory.push_back(trip);
	}
}
void MainMenu::startMapping()
{
	system("cls");
	int num = 0, count = 1;
	for (auto it : currentTrips)
	{
		cout << count << ". " << it.getTripName() << endl;
	}
	cout << "Enter trip number: " << endl;
	cin >> num;
	if (num < 1 || num > currentTrips.size())
		return;
	num--;
	auto it = next(currentTrips.begin(), num);
	navigate.menu(*it); // After this the trip will be part of history
	tripHistory.push_back(*it);
	currentTrips.erase(it);
}
void MainMenu::printHistory()
{
	system("cls");
	if (tripHistory.empty())
	{
		cout << "You have no history";
		getch();
		return;
	}
	while (1)
	{
		system("cls");
		cout << "Menu --------- D | d: Sort by Date ------ N | n: Sort by Name ------ e | E: To delete trip ------ S | s: Search ---- Any key to go back" << endl;
		for (auto itor : tripHistory)
		{
			itor.tripDetails();
			cout << endl;
			for (int i = 0; i < 40; i++)
				cout << "*";
			cout << endl;
		}
		char c = getch();
		if (c == 'd' || c == 'D')
			tripHistory.sort(compare_date);
		else if (c == 'N' || c == 'n')
			tripHistory.sort(compare_name);
		else if (c == 'S' || c == 's')
		{
			string s;
			cout << "Enter attribute (name date): " << endl;
			cin >> s;
			system("cls");
			// Matching characters with tripHistory name or date
			for (auto it : tripHistory)
			{
				for (int i = 0; (i < it.getTripName().length() || i < it.getDeparture().length()) && i < s.length(); i++)
				{
					if (i < it.getTripName().length() && it.getTripName()[i] == s[i])
					{
						it.tripDetails();
						cout << endl;
						break;
					}
					if (i < it.getDeparture().length() && it.getDeparture()[i] == s[i])
					{
						it.tripDetails();
						cout << endl;
						break;
					}
				}
			}
			getch();
		}
		else if (c == 'e' || c == 'E')
		{
			system("cls");
			string name;
			cout << "Enter trip name: ";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, name);
			auto it = std::find_if(tripHistory.begin(), tripHistory.end(),
								   [name](Trip &trip)
								   { return trip.getTripName() == name; });

			tripHistory.erase(it);
		}
		else
			break;
	}
}

void MainMenu::createTrip() // Creates a trip and pushes it to currentTrips
{
	Trip trip;
	trip.inputPlan();
	currentTrips.push_back(trip);
}
void MainMenu::logOut()
{ // After logging out users changes will be saved
	ifstream inputFile("Profiles.txt");
	inputFile.seekg(0, ios::beg);
	list<string> outputFileString;
	string line;
	getline(inputFile, line);
	outputFileString.push_back(line);
	while (getline(inputFile, line))
	{
		istringstream ss(line);
		string fileEmail;
		for (int i = 0; i < 2; i++)
			getline(ss, fileEmail, ','); // Getting user email since it it unique
		if (fileEmail == profile.email)
		{
			line.clear();
			line += profile.name + ',' + profile.email + ',' + profile.password + ',';
			for (auto it : tripHistory)
			{
				line += it.getDeparture() + ',' + it.getArrival() + ',' + it.getTripName() + ',' + to_string(it.getTimeOfJourney()) + ',' + to_string(it.getIsOneWay()) + ',' + to_string(it.getSourceCity()) + ',';
				for (auto itor : it.getVisitedCities())
					line += to_string(itor) + ' ';
				line += ',';
				for (auto itor : it.getPath())
					line += to_string(itor) + ' ';
				line += ',';
			}
		}
		outputFileString.push_back(line);
	}
	inputFile.close();
	ofstream outputFile("Profiles.txt", ios::out);
	for (auto it = outputFileString.begin(); it != prev(outputFileString.end(), 1); it++)
		outputFile << *it << endl;
	outputFile << outputFileString.back();
	outputFile.close();
}
int main()
{
	Graph gh; // Graph will only be initialized once in program
	gh.loadHash();
	gh.loadGraph();

	MainMenu m;
	m.menu();
}
