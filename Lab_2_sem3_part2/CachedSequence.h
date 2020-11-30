#pragma once

#include <utility>

#include "dependencies/Sequence.h"
#include "dependencies/ArraySequence.h"

#include "IDictionary.h"
#include "HashMap.h"

using namespace sequences;
using namespace dictionary;

template<class K, class V>
class CachedSequence
{
public:
	typedef std::pair<K, V> KeyValuePair;
private:
	Sequence<KeyValuePair>* data;
	IDictionary<K, V>* cache;

	int cacheSize;
public:
	CachedSequence(std::function<int(K, int)> hashFunc, int cacheSize) :
		data(new ArraySequence<KeyValuePair>()),
		cache(new HashMap<K, V>(hashFunc, cacheSize)),
		cacheSize(cacheSize)
	{}
};