#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Hash
{
protected:
	// Static variables are used so that memory is allocated only once
	static unordered_map<int, string> frontCityMapping;	  // Associating int value with string like 0 Karachi
	static unordered_map<string, int> reverseCityMapping; // Associating string to int like Karachi 0
	static int vertices;								  // Number of vertices or cities in a graph will depend on file
public:
	void loadHash()
	{
		vertices = 0;
		ifstream cityFile("Cities.csv", ios::in);
		string list;
		if (!cityFile.is_open())
		{
			std::cout << "Error opening file!\n";
			return;
		}
		cityFile.seekg(0, std::ios::beg);
		while (getline(cityFile, list))
		{
			std::istringstream iss(list);
			string city;
			getline(iss, city, ',');
			frontCityMapping[vertices] = city;
			reverseCityMapping[city] = vertices;
			vertices++;
		}
	}
	// Polymorphic function for getting keys
	string getKey(int a)
	{
		return frontCityMapping[a];
	}
	int getKey(string c)
	{
		return reverseCityMapping[c];
	}
	// for printing table
	void printHash()
	{
		for (auto it : reverseCityMapping)
		{
			cout << it.first << endl;
		}
	}
};
int Hash::vertices = 0;
unordered_map<int, string> Hash::frontCityMapping;
unordered_map<string, int> Hash::reverseCityMapping;

class Graph : public Hash
{ // Contains adjacency list of cities
protected:
	static unordered_map<int, list<pair<int, int>>> Map; // int denotes cities and their distances
public:
	void loadGraph()
	{
		ifstream cityFile("Cities.csv", ios::in);
		string list;
		if (!cityFile.is_open())
		{
			std::cout << "Error opening file!\n";
			return;
		}
		cityFile.seekg(0, std::ios::beg);
		while (getline(cityFile, list))
		{
			std::istringstream iss(list);
			string source, des, desDistance;
			getline(iss, source, ',');
			while (getline(iss, des, ',') && getline(iss, desDistance, ','))
			{
				pair<int, float> pairList;
				pairList.first = getKey(des);
				pairList.second = stoi(desDistance);
				Map[getKey(source)].push_back(pairList);
			}
		}
		cityFile.close();
	} //A function to print graph of cities
	void printGraph()
	{
		system("cls");
		for (auto it : Map)
		{
			cout << getKey(it.first) << " ";
			for (auto itor : it.second)
			{
				cout << getKey(itor.first) << " " << itor.second;
			}
			cout << endl;
		}
		getch();
	}
};
unordered_map<int, list<pair<int, int>>> Graph::Map;