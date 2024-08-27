#pragma once
#include <iostream>
#include <unordered_set>
#include <list>
#include <windows.h>
#include <conio.h>
#include <string>
#include "Graph.h"
using namespace std;
class Trip : public Graph
{
	list<int> path;						 // Contains the whole path of journey
	list<int>::iterator currentCity;	 // A marker which will point to the current city where the user is headed towards
	string departure, arrival, tripName; // Name of trip
	int timeOfJourney = 0;				 // in seconds
	unordered_set<int> visitedCities, unvisitedCites;
	int userSourceCity;					  // Will remain same throughout the journey
	bool isTripStarted = 0, isOneWay = 0; // If the trip has started
public:
	void menu();								 // Menu for each trip
	void tripDetails();							 // Details of trip created
	void inputPlan();							 // Input the plan of trip
	void applyDijkstra(int, unordered_set<int>); // Apply dijkstra between source and cities
	void appendCitiesToPlan();					 // This will insert cities to unvisitedCites
	void changeTripPlan();						 // If the user wants change the plan of trip in the middle of journey or during creation of trip
	void printList(list<int> &, bool);			 // A function to print list

	// Getters And Setters
	string& getTripName()
	{
		return tripName;
	}
	void setTripName(string name)
	{
		tripName = name;
	}
	string& getDeparture()
	{
		return departure;
	}
	void setDeparture(string &s)
	{
		departure = s;
	}
	string& getArrival()
	{
		return arrival;
	}
	void setArrival(string &s)
	{
		arrival = s;
	}
	void setTimeOfJourney(int a)
	{
		timeOfJourney = a;
	}
	unordered_set<int>& getVisitedCities()
	{
		return visitedCities;
	}
	unordered_set<int>& getUnvisitedCities()
	{
		return unvisitedCites;
	}
	list<int>::iterator& getCurrentCity()
	{
		return currentCity;
	}
	void setCurrentCity(list<int>::iterator it)
	{
		currentCity = it;
	}
	list<int>& getPath()
	{
		return path;
	}
	void setTripStarted(bool s)
	{
		isTripStarted = s;
	}
	void setOneWay(int c)
	{
		isOneWay = c;
	}
	void setSourceCity(int s)
	{
		userSourceCity = s;
	}
	int& getSourceCity()
	{
		return userSourceCity;
	}
	int& getTimeOfJourney()
	{
		return timeOfJourney;
	}
	bool& getIsOneWay()
	{
		return isOneWay;
	}
};

void Trip::menu()
{
	char c;
	while (1)
	{
		system("cls");
		cout << "1. Trip Details\n"
			 << "2. Change Trip Data\n"
			 << "\nPress any key to go back";
		c = getch();
		switch (c)
		{
		case '1':
			system("cls");
			tripDetails();
			getch();
			break;
		case '2':
			changeTripPlan();
			break;
		default:
			return;
		}
	}
}
void Trip::inputPlan()
{
	system("cls");
	while (1)
	{
		string s;
		cout << "Source: ";
		cin >> s;
		if (reverseCityMapping.find(s) == reverseCityMapping.end())
		{
			cout << "City data not present" << endl;
			continue;
		}
		userSourceCity = getKey(s);
		break;
	}

	cout << "Trip Name: ";
	cin.ignore();
	getline(cin >> ws, tripName);

	cout << "One way (1 for oneWay and 0 for return): ";
	cin >> isOneWay;

	appendCitiesToPlan();
	path.push_back(userSourceCity);
	applyDijkstra(userSourceCity, unvisitedCites);
	if (isOneWay)
		return;
	applyDijkstra(path.back(), {userSourceCity});
}
void Trip::changeTripPlan()
{
	system("cls");
	if (!isTripStarted)
	{
		inputPlan();
		return;
	}
	path.erase(next(currentCity, 1), path.end()); // Erasing all cities after current cities
	unvisitedCites.clear();						  // Since the user wants to start new trip from current city
	appendCitiesToPlan();
	applyDijkstra(*currentCity, unvisitedCites);
}
void Trip::applyDijkstra(int source, unordered_set<int> cities)
{ /* This algorithm will keep normalizing nodes until it finds a city in set. If it does then that
city will become next source and process repeates
*/

	while (!cities.empty())
	{
		vector<int> dis(vertices, INT_MAX), prev(vertices, -1); // Distance of a node from source node previous vertex of a node
		vector<bool> vis(vertices, 0);							// Nodes which have been visited
		dis[source] = 0;
		list<int> path;
		int normalized = source;

		while (1)
		{
			vis[normalized] = 1;
			for (auto it : Map[normalized])
			{ // Updating distances
				int i = dis[normalized] + it.second;
				if (i < dis[it.first] && !vis[it.first])
				{
					dis[it.first] = i;
					prev[it.first] = normalized;
				}
			}
			int minIndex = 0, minDis = INT_MAX;
			for (int i = 0; i < vis.size(); i++)  //For finding the normalized node
			{ // Finding minimum index
				if (!vis[i] && dis[i] < minDis)
				{
					minDis = dis[i];
					minIndex = i;
				}
			}
			normalized = minIndex;
			if (cities.find(minIndex) != cities.end())
			{
				cities.erase(minIndex);
				int i = minIndex;
				while (i != source)
				{
					path.push_back(i);
					i = prev[i];
				}
				reverse(path.begin(), path.end());
				this->path.splice(this->path.end(), path);
				source = minIndex;
				break;
			}
		}
	}
}
void Trip::appendCitiesToPlan() //Will push cities to unvisited set
{
	system("cls");
	int num = 0;
	do
	{
		cout << "Enter the number of cities you want to visit: ";
		cin >> num;
	} while (num <= 0);
	unordered_set<int> li; // Removing dublicates
	for (int i = 0; i < num; i++)
	{
		string s;
		cin >> s;
		if (reverseCityMapping.find(s) == reverseCityMapping.end())
		{
			cout << "City data is not present " << endl;
			i--;
			continue;
		}
		if (getKey(s) == userSourceCity)
		{
			i--;
			cout << "Cannot take source city" << endl;
			continue;
		}
		li.insert(getKey(s));
	}
	for (auto it : li)
		unvisitedCites.insert(it);
}
void Trip::tripDetails() //Printing trip details
{
	cout << "Name: " << tripName << "\nSource City: " << getKey(userSourceCity) << "\nDeparture: " << departure << "\nArrival: " << arrival << "\nTime of Journey in secs: " << this->timeOfJourney << "\nTrip Type: " << ((isOneWay) ? "OneWay" : "Return") << endl
		 << "Cities included in plan: " << endl;
	for (auto it : visitedCities)
		cout << getKey(it) << endl;
	for (auto it : unvisitedCites)
		cout << getKey(it) << endl;
	cout << "\nGenerated Path: ";
	printList(path, 1);
}
void Trip::printList(list<int> &cities, bool isNewline = 0) //Utility function to print list
{
	for (auto index : cities)
	{
		cout << getKey(index);
		if (isNewline)
			cout << endl;
	}
}
