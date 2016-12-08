#pragma once

#include<vector>

template <typename KEY, typename VAL>
class Bucket
{
	KEY* m_key;
	VAL* m_val;

	Bucket* m_pNext;		//Next bucket with same hash - for collision resolution


public:
	//Accessor methods
	VAL* GetValPtr() const { return m_val;	}
	void SetVal(const VAL& newVal) 
	{ 
		//If val already exists, set value
		if (m_val != NULL)
		{
			*m_val = newVal;
		}
		//If val doesn't exist, create it on the heap
		else
		{
			m_val = new VAL(newVal);
		} 
	}

	KEY* GetKeyPtr() const { return m_key; }
	void SetKey(const KEY& newKey)
	{
		//If key already exists, set value
		if (m_key != NULL)
		{
			*m_key = newKey;
		}
		//If key doesn't exist, create it on the heap
		else
		{
			m_key = new KEY(newKey);
		}
	}

	Bucket* GetNext() const { return m_pNext; }
	void SetNext(Bucket* newNext) { m_pNext = newNext; }

	//Constructors
	Bucket(const KEY& newKey, const VAL& newVal)
	{
		SetKey(newKey);
		SetVal(newVal);
		m_pNext = NULL;
	}
	Bucket()	//Default bucket is totally empty
	{
		m_key = NULL;
		m_val = NULL;
		m_pNext = NULL;
	}

	//Destructor
	~Bucket()
	{
		if (m_key != NULL)
		{
			delete m_key;	//Delete any key pointed to
			m_key = NULL;
		}
		if (m_val != NULL)
		{
			delete m_val;	//Delete any value pointed to
			m_val = NULL;
		}

		//Call delete on all items in linked list
		while (m_pNext != NULL)
		{
			Bucket<KEY, VAL>* prev = m_pNext;

			//Find the next value if it exists
			m_pNext = m_pNext->GetNext();

			//Unlink the previous item and delete it
			prev->SetNext(NULL);
			delete prev;
		}
	}
};

template <typename KEY, typename VAL>
class HashMap
{
	//Holds the current size of the hash map
	unsigned long m_hashSize;
	unsigned long m_numHashValues;

	//A vector of bucket pointers makes up the map
	std::vector < Bucket<KEY, VAL>* > m_map;

	//This hash function divides the key by the map size and takes the remainder to assign a value
	unsigned long hashFunction(const KEY& key) const
	{
		return reinterpret_cast<unsigned long&> (const_cast<KEY&>(key)) % m_hashSize;
	}

	//Finds the bucket in linked list immediately before the bucket associated with a particular key
	Bucket<KEY, VAL>* findPrevBucket(const KEY& key)
	{
		Bucket<KEY, VAL>* prevBucket = NULL;

		//Get a hashValue for the key
		unsigned long hashVal = hashFunction(key);

		//Find the first bucket at that point
		Bucket<KEY, VAL>* curBucket = m_map[hashVal];

		//Loop through any entries created by collisions at this location and find the matching key
		while (curBucket != NULL)
		{
			KEY* curKey = curBucket->GetKeyPtr();

			//We've found the value we're searching for
			if (NULL != curKey && *curKey == key)
			{
				return prevBucket;
			}

			//Not this one, check the next bucket
			prevBucket = curBucket;
			curBucket = curBucket->GetNext();
		}

		//Didn't find the bucket
		return prevBucket;
	}

	//Finds a particular bucket given a key
	Bucket<KEY, VAL>* findBucket(const KEY& key)
	{
		//Get a hashValue for the key
		unsigned long hashVal = hashFunction(key);

		//Find the first bucket at that point
		Bucket<KEY, VAL>* curBucket = m_map[hashVal];

		//Loop through any entries created by collisions at this location and find the matching key
		while (curBucket != NULL)
		{
			KEY* curKey = curBucket->GetKeyPtr();

			//We've found the value we're searching for
			if (NULL != curKey && *curKey == key)
			{
				return curBucket;
			}

			//Not this one, check the next bucket
			curBucket = curBucket->GetNext();
		}

		//Didn't find the bucket
		return NULL;
	}

public:
	HashMap()
	{
		//Default size of map starts at 1000
		m_hashSize = 1000;

		initMap();
	}
	HashMap(const unsigned long& size)
	{
		m_hashSize = size;

		initMap();
	}

	//Creates empty table for hashing
	void initMap()
	{
		//create all the buckets for the hash map on the heap
		for (unsigned long i = 0; i < m_hashSize; i++)
		{
			m_map.push_back(NULL);
		}
	}

	~HashMap()
	{
		//delete all buckets from the map vector
		for (unsigned long i = 0; i < m_hashSize; i++)
		{
			if (m_map[i] != NULL)
			{
				delete m_map[i];
			}
		}
	}

	//a pointer to the value found in the hash map (value is NULL if it does not exist yet)
	VAL* GetValueAt(const KEY& key)
	{
		//Get a hashValue for the key
		unsigned long hashVal = hashFunction(key);

		Bucket<KEY, VAL>* curBucket = findBucket(key);

		//If we found a bucket, return the value pointer for it
		if (NULL != curBucket)
		{
			return curBucket->GetValPtr();
		}

		//Nothing found
		return NULL;
	}

	//Add a new value to the map for a particular key
	void Add(const KEY &key, const VAL &val)
	{
		unsigned long hashValue = hashFunction(key);
		Bucket<KEY, VAL> *prev = NULL;
		Bucket<KEY, VAL> *entry = m_map[hashValue];

		//Loop through list of collisions.  We want to get to the end or find the key
		while (entry != NULL && entry->GetKeyPtr() != NULL && *(entry->GetKeyPtr()) != key)
		{
			prev = entry;
			entry = entry->GetNext();
		}

		//If we can't find the entry, we need to add a new one
		if (entry == NULL)
		{
			m_numHashValues++;
			entry = new Bucket<KEY, VAL>(key, val);

			//If this is the first entry add the first bucket
			if (prev == NULL)
			{
				m_map[hashValue] = entry;
			}
			//Otherwise link this entry in to the existing ones
			else
			{
				prev->SetNext(entry);
			}
		}
		//if we've found the entry, update the value in the existing bucket
		else
		{
			entry->SetVal(val);
		}
	}

	//Removes a value from the map, deletes the bucket if it was part of a collision list
	void Delete(const KEY& key)
	{
		Bucket<KEY, VAL>* delBucket = findBucket(key);
		Bucket<KEY, VAL>* prev = findPrevBucket(key);

		//Check that this value exists in the map before trying to delete it
		if (delBucket != NULL)
		{
			Bucket<KEY, VAL>* next = delBucket->GetNext();

			//We are examining the head node, can't delete it
			if (prev == NULL)
			{
				//If this bucket is the only node remove all values
				if (next == NULL)
				{
					delBucket->SetKey(NULL);
					delBucket->SetVal(NULL);
					delBucket->SetNext(NULL);
				}
				else //There are more nodes after the head node
				{
					//Copy next values into current node
					delBucket->SetKey(*next->GetKeyPtr());
					delBucket->SetVal(*next->GetValPtr());
					delBucket->SetNext(next->GetNext());

					//unlink next bucket
					next->SetNext(NULL);

					//Now that the next bucket is unlinked, we can delete it
					delete next;
				}
			}
			//Any node that isn't the head node is safe to delete
			else
			{
				//Unlink the node
				prev->SetNext(next);
				delBucket->SetNext(NULL);

				delete delBucket;
			}
			m_numHashValues--;
		}
	}

	unsigned long NumberOfStoredValues() const { return m_numHashValues; }

};