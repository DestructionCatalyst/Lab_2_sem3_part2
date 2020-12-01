#pragma once

#include <utility>
#include <stdexcept>

#include "dependencies/Sequence.h"
#include "dependencies/ArraySequence.h"
#include "dependencies/LinkedList.h"

#include "IDictionary.h"
#include "HashMap.h"
#include "DoubleLinkedList.h"

using namespace sequences;
using namespace dictionary;

template<class K, class V>
class CachedSequence
{
public:
	typedef std::pair<K, V> KeyValuePair;
	typedef DoubleNode<KeyValuePair> CachePointer;
private:
	Sequence<KeyValuePair>* data;
	IDictionary<K, CachePointer*>* cacheTable;
	DoubleLinkedList<KeyValuePair>* cacheList = new DoubleLinkedList<KeyValuePair>();

	int cacheSize;
public:
	CachedSequence(std::function<int(K, int)> hashFunc, int cacheSize) :
		data(new ArraySequence<KeyValuePair>()),
		cacheTable(new HashMap<K, CachePointer*>(hashFunc, cacheSize)),
		cacheSize(cacheSize)
	{}

	void Add(K key, V value)
	{
		KeyValuePair pair = std::make_pair(key, value);
		data->Append(pair);
		PutInCache(pair);
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

		//std::cout << ((Node<KeyValuePair>*)cacheList->begin())->GetContent().first
			//<< *dynamic_cast<HashMap<K, PrevCacheItemIter*>*>(cacheTable) << std::endl;

		return res;
	}
private:
	KeyValuePair GetFromCache(K key)
	{
		return cacheTable->Get(key)->GetContent();
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
		if (cacheList->GetLength() >= cacheSize)
		{
			//Remove last item from cache
			CachePointer* last = cacheList->Tail();

			cacheTable->Remove(last->GetContent().first);
			cacheList->Remove(last);
		}


		//Add to the start
		cacheList->Prepend(pair);
		cacheTable->Add(pair.first, cacheList->Head());
	}
	void RefreshCache(K key)
	{
		CachePointer* toStart = cacheTable->Get(key);

		cacheList->MoveToStart(toStart);
	}

};
/*
template<class K, class V>
class CachedSequence
{
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
*/