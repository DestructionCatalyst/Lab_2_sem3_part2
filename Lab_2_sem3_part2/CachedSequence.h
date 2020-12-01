#pragma once

#include <utility>
#include <stdexcept>

#include "dependencies/Sequence.h"
#include "dependencies/ArraySequence.h"
#include "dependencies/LinkedList.h"

#include "IDictionary.h"
#include "HashMap.h"

using namespace sequences;
using namespace dictionary;

template<class K, class V>
class CachedSequence
{
public:
	typedef std::pair<K, V> KeyValuePair;
	typedef sequences::iterators::MutableListIterator<std::pair<K, V>> PrevCacheItemIter;
private:
	Sequence<KeyValuePair>* data;
	IDictionary<K, PrevCacheItemIter*>* cacheTable;
	LinkedList<KeyValuePair>* cacheList = new LinkedList<KeyValuePair>();

	int cacheSize;
public:
	CachedSequence(std::function<int(K, int)> hashFunc, int cacheSize) :
		data(new ArraySequence<KeyValuePair>()),
		cacheTable(new HashMap<K, PrevCacheItemIter*>(hashFunc, cacheSize)),
		cacheSize(cacheSize)
	{}

	void Add(K key, V value)
	{
		data->Append(std::make_pair(key, value));
	}
	V Get(K key)
	{
		V res;
		try
		{
			res = GetFromCache(key).second;
			RefreshCache(key);
		}
		catch (key_not_found e)
		{
			KeyValuePair fromArray = FindInArray(key);
			res = fromArray.second;
			PutInCache(fromArray);
		}

		std::cout << ((Node<KeyValuePair>*)cacheList->begin())->GetContent().first
			<< *dynamic_cast<HashMap<K, PrevCacheItemIter*>*>(cacheTable) << std::endl;

		return res;
	}
private:
	KeyValuePair GetFromCache(K key)
	{
		if ((!cacheList->IsEmpty()) && (cacheList->GetFirst().first == key))
			return cacheList->GetFirst();

		PrevCacheItemIter* prev = new PrevCacheItemIter(*cacheTable->Get(key));
		
		return *(++(*prev));
	}
	KeyValuePair FindInArray(K key)
	{
		KeyValuePair tmp;

		for (int i = 0; i < data->GetLength(); i++)
		{
			tmp = data->Get(i);
			if (tmp.first == key)
				return tmp;
		}

		throw key_not_found("");
	}
	void PutInCache(KeyValuePair pair)
	{
		if (cacheList->GetLength() == cacheSize)
		{
			//Remove last item from cache

			K lastKey = cacheList->GetLast().first;

			PrevCacheItemIter* prevToLastItem = cacheTable->Get(lastKey);

			cacheList->Remove(*prevToLastItem);
			cacheTable->Remove(lastKey);

		}
			

		//Add to the start
		if (!cacheList->IsEmpty()) {
			K secondItemKey = cacheList->GetFirst().first;

			cacheList->Prepend(pair);

			AddSecondItemToTable(secondItemKey);
		}
		else
			cacheList->Prepend(pair);
	}
	void RefreshCache(K key)
	{
		try
		{
			K secondItemKey = cacheList->GetFirst().first;

			PrevCacheItemIter* prevToMoved = cacheTable->Get(key); // ~ key
			Node<KeyValuePair>* nextToMovedPtr = (*prevToMoved).GetNextPointer()->Next();

			PrevCacheItemIter* moved = nullptr;
			K nextToMovedKey{};

			if (nextToMovedPtr != nullptr)
			{
				moved = cacheTable->Get(nextToMovedPtr->GetContent().first); // ~ nextToMovedKey
				nextToMovedKey = (**moved).first; // ~ moved
			}

			cacheList->MoveToStart(*prevToMoved);
			cacheTable->Remove(key);

			if (moved != nullptr) 
				cacheTable->Add(nextToMovedKey, moved);
			
			if (secondItemKey != key)
				AddSecondItemToTable(secondItemKey);
		}
		catch (key_not_found e)
		{
		}
	}
	void AddSecondItemToTable(K secondItemKey)
	{
		PrevCacheItemIter* item = new PrevCacheItemIter(cacheList->begin());
		cacheTable->Add(secondItemKey, item);
	}
};