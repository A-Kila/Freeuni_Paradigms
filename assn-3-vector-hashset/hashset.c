#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	assert(elemSize > 0);
	assert(numBuckets > 0);
	assert(hashfn != NULL);
	assert(comparefn != NULL);
	h->elemSize = elemSize;
	h->numBuckets = numBuckets;
	h->hashFn = hashfn;
	h->compareFn = comparefn;
	h->logSize = 0;
	h->buckets = malloc(numBuckets * sizeof(vector));
	for (int i = 0; i < numBuckets; i++)
		VectorNew(&h->buckets[i], elemSize, freefn, 0);
}

void HashSetDispose(hashset *h)
{
	for (int i = 0; i < h->numBuckets; i++) 
		VectorDispose(&h->buckets[i]);
	free(h->buckets);
}

int HashSetCount(const hashset *h) { return h->logSize; }

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
	assert(mapfn != NULL);
	for (int i = 0; i < h->numBuckets; i++)
		VectorMap(&h->buckets[i], mapfn, auxData);
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
	assert(elemAddr != NULL);
	int bucketIn = h->hashFn(elemAddr,h->numBuckets);
	assert(bucketIn >= 0 && bucketIn < h->numBuckets);
	int prevElem = VectorSearch(&h->buckets[bucketIn], elemAddr, h->compareFn, 0, false);
	if (prevElem == -1) {
		VectorAppend(&h->buckets[bucketIn], elemAddr);
		h->logSize++;
		return;
	}
	VectorReplace(&h->buckets[bucketIn], elemAddr, prevElem);
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{ 
	assert(elemAddr != NULL);
	int bucketIn = h->hashFn(elemAddr,h->numBuckets);
	assert(bucketIn >= 0 && bucketIn < h->numBuckets);
	int elemIn = VectorSearch(&h->buckets[bucketIn], elemAddr, h->compareFn, 0, false);
	if (elemIn == -1) return NULL;
	return VectorNth(&h->buckets[bucketIn], elemIn); 
}
