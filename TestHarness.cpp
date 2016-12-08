// HashMap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include "HashMap.h"

using namespace std;


int main()
{
	{
		//Create a hashmap of ints with a hash size of 10
		HashMap<char, int> myMap(10);

		//Add eleven values/keys to the map
		myMap.Add('a', 10);
		myMap.Add('b', 9);
		myMap.Add('c', 8);
		myMap.Add('d', 7);
		myMap.Add('e', 6);
		myMap.Add('f', 5);
		myMap.Add('g', 4);
		myMap.Add('h', 3);
		myMap.Add('i', 2);
		myMap.Add('j', 1);
		myMap.Add('k', 0);

		//Look for the values/keys in the map
		int* myPtr = myMap.GetValueAt('c');
		cout << "Looking for 'c', should be 8. myPtr:" << myPtr << " returns the value: " << (*myPtr) << endl;
		cout << "There are: " << myMap.NumberOfStoredValues() << " values stored in the map." << endl;
		//Overwrite a value
		myMap.Add('c', 55);
		cout << "Looking for 'c', overwritten to 55. myPtr:" << myPtr << " returns the value: " << (*myPtr) << endl;

		myPtr = myMap.GetValueAt('k');
		cout << "Looking for 'k', collision should be 0. myPtr:" << myPtr << " returns the value: " << (*myPtr) << endl;

		//Delete all values from the map
		myMap.Delete('k');
		cout << "Looking for 'k', should be deleted. myPtr:" << myPtr << " returns random value: " << (*myPtr) << endl;
		cout << "There are: " << myMap.NumberOfStoredValues() << " values stored in the map." << endl;
	}

	//Let the map go out of scope and be destructed
	cout << "Destroying myMap." << endl;



	system("pause");
    return 0;
}

