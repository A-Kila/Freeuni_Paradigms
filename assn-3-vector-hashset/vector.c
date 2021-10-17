#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    assert(initialAllocation >= 0);
    assert(elemSize > 0);
    int allocSize = 4;
    if (initialAllocation != 0) allocSize = initialAllocation;
    v->elems = malloc(allocSize * elemSize);
    v->elemSize = elemSize;
    v->allocSize = allocSize;
    v->initAllocSize = allocSize;
    v->logSize = 0;
    v->freeFn = freeFn;
}

void VectorDispose(vector *v)
{
    if (v->freeFn != NULL) {
        for (int i = 0; i < v->logSize; i++) {
            void *elem = (char *)v->elems + i * v->elemSize;
            v->freeFn(elem);
        }
    }
    free(v->elems);
}

int VectorLength(const vector *v) { return v->logSize; }

void *VectorNth(const vector *v, int position)
{ 
    assert(position >= 0 && position < v->logSize);
    return (void *)((char *)v->elems + position * v->elemSize); 
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position < v->logSize);
    void *elemPtr = (char *)v->elems + position * v->elemSize;
    if (v->freeFn != NULL) v->freeFn(elemPtr);
    memcpy(elemPtr, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position <= v->logSize);
    if (v->logSize == v->allocSize) {
        v->allocSize += v->initAllocSize;
        v->elems = realloc(v->elems, v->allocSize * v->elemSize);
    }
    memmove((char*)v->elems + (position + 1) * v->elemSize, 
            (char*)v->elems + position * v->elemSize, (v->logSize - position) * v->elemSize);
    memcpy((char*)v->elems + position * v->elemSize, elemAddr, v->elemSize);
    v->logSize++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    if (v->logSize == v->allocSize) {
        v->allocSize += v->initAllocSize;
        v->elems = realloc(v->elems, v->allocSize * v->elemSize);
    }
    memcpy((char*)v->elems + v->logSize++ * v->elemSize, elemAddr, v->elemSize);
}

void VectorDelete(vector *v, int position)
{
    assert(position >= 0 && position < v->logSize);
    void *elemPtr = (char *)v->elems + position * v->elemSize;
    if (v->freeFn != NULL) v->freeFn(elemPtr);
    memcpy(elemPtr, (char *)elemPtr + v->elemSize, (v->logSize-- - position - 1) * v->elemSize);
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    assert(compare != NULL);
    qsort(v->elems, v->logSize, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    assert(mapFn != NULL);
    for (int i = 0; i < v->logSize; i++) {
        void *elemPtr = (char *)v->elems + i * v->elemSize;
        mapFn(elemPtr, auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    assert(startIndex >= 0 && startIndex <= v->logSize);
    assert(searchFn != NULL && key !=  NULL);
    int result = kNotFound;
    if (isSorted) {
        void *elem = bsearch(key, (char *)v->elems + startIndex * v->elemSize, v->logSize - startIndex, 
                            v->elemSize, searchFn);
        if (elem != NULL)
            result = (elem - v->elems) / v->elemSize;
    } else {
        for (int i = startIndex; i < v->logSize; i++)
            if (!searchFn(key, (char *)v->elems + i * v->elemSize)) {
                result = i; 
                break;
            }
    }
    return result; 
}