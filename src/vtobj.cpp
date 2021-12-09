/* vtobj.cpp */

/* $Id: vtobj.cpp,v 1.4 2013/02/06 17:00:22 kpettit1 Exp $ */

/*
 * Copyright 2004 Ken Pettit
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include	<memory.h>
#include	<stdlib.h>
#include    <string.h>
#include	"VirtualT.h"
#include	"vtobj.h"
#include	<stdio.h>

const struct VTClass VTObject::classVTObject =
	{ "VTObject", sizeof(VTObject), 0, 0, 0};

VTClass* VTObject::GetClass() const
{
	return VT_CLASS(VTObject);
}

VTObject* VTClass::CreateObject()
{
	if (m_pCreateObject == 0)
		return 0;

	VTObject* pObject = (*m_pCreateObject)();

	return pObject;
}

VTObject::VTObject()
{
}

VTObject::~VTObject()
{
}

/*
========================================================================
Implement the VTObArray class to handls object arrays
========================================================================
*/

IMPLEMENT_DYNCREATE(VTObArray, VTObject)

VTObArray::VTObArray()
{
	m_pData = 0;
	m_Count = 0;
	m_GrowBy = 16;
	m_Size = 0;
}

VTObArray::~VTObArray()
{
	if (m_pData != 0)
		delete[] (char*) m_pData;

	m_pData = 0;
}

int VTObArray::Add(VTObject *pObj)
{
	int		index = m_Count;

	InsertAt(m_Count, pObj);

	return index;
}

VTObject* VTObArray::GetAt(int nIndex) const
{
	VTObject*	pObj;

	// Test if index out of bounds
	if (nIndex > m_Count)
		return 0;

	memcpy(&pObj, (char *) m_pData + nIndex * sizeof(VTObject *), sizeof(VTObject *));

	return pObj;
}

VTObject* VTObArray::operator [](int nIndex) const
{
	return GetAt(nIndex);
}

void VTObArray::InsertAt(int nIndex, VTObject *newElement)
{
	int		newCount = m_Count + 1;
	char*	pInsertLoc;
	int		c;

	// Test if array is big enough
	if (newCount > m_Size)
		SetSize(m_Size + m_GrowBy, 0);

	// Test if adding a end of array
	if (nIndex == -1)
		nIndex = m_Count;

	// Determine location for new entry
	pInsertLoc = (char *) m_pData + (nIndex * sizeof(VTObject *));

	// Make space for new entry in the array
	for (c = m_Count - 1; c >= nIndex; c--)
		memcpy((char *) m_pData + (c + 1) * sizeof(VTObject *), 
			(char *) m_pData + c * sizeof(VTObject *), sizeof(VTObject *));

	// Add new entry to array
	memcpy(pInsertLoc, &newElement, sizeof(VTObject *));

	// Update Count
	m_Count++;
}

void VTObArray::RemoveAt(int nIndex, int nCount)
{
	int		c;

	if (m_Count == 0)
		return;

	// Check if removing from end of array
	if (nIndex == -1)
		nIndex = m_Count - 1;

	for (c = nIndex; c < m_Count-nCount; c++)
		memcpy((char *) m_pData + c * sizeof(VTObject *), (char *) m_pData + (c + nCount) * sizeof(VTObject *),
			sizeof(VTObject *));

	// Drecrement count
	m_Count--;
}

void VTObArray::SetSize(int nNewSize, int nGrowBy)
{
	void*		pNew;

	// Check if GrowBy value needs to be updated
	if (nGrowBy != 0)
		m_GrowBy = nGrowBy;

	// Check if new size is same as old size
	if (nNewSize == m_Size)
		return;

	pNew = (void *) new char[nNewSize * sizeof(VTObject*)];
	memcpy(pNew, m_pData, m_Count * sizeof(VTObject*));

	// Delete old data
    if (m_pData != NULL)
        delete[] (char *)m_pData;

	// Assign new data
	m_pData = pNew;
	m_Size = nNewSize;
}

IMPLEMENT_DYNCREATE(VTMapStringToOb, VTObject)

VTMapStringToOb::VTMapStringToOb()
{
	int		c;

	for (c = 0; c < 256; c++)
		m_HashPtrs[c] = 0;
	m_Count = 0;
	m_Null = (VTObject *) 0;
}

VTMapStringToOb::~VTMapStringToOb()
{
	RemoveAll();
}

unsigned char VTMapStringToOb::HashKey(const char *key)
{
	int				c;
	unsigned char	hash[2];
	unsigned short	*sHash;

	sHash = (unsigned short *) hash;
	*sHash = 0;
	for (c = 0; key[c] != 0; c++)
	{
		*sHash = (*sHash << 3) + key[c] + 0x26;
		hash[0] -= hash[1] ^ 0x6A;
	}

	return hash[0];
}

int VTMapStringToOb::Lookup(const char *key, VTObject *&rValue)
{
	POSITION		iter;
	unsigned char	hash;

	// Calculate hash and get pointer to first entry in hash array
	hash = HashKey(key);
	iter = m_HashPtrs[hash];

	// Iterate through all items which have this hash
	while (iter != 0)
	{
		if (strcmp(iter->pKey, key) == 0)
		{
			rValue = iter->pObj;
			return 1;
		}

		iter = iter->pNext;
	}

	return 0;
}

void VTMapStringToOb::RemoveAt(MString &rKey)
{
	POSITION		iter, iterPrev;
	unsigned char	hash;
	const char		*key;

	// Calculate hash and get pointer to first entry in hash array
	key = (const char *) rKey;
	hash = HashKey(key);
	iter = m_HashPtrs[hash];
	// Save previous iter so we can delete
	iterPrev = iter;

	// Iterate through all items which have this hash
	while (iter != 0)
	{
		// Test if this is the item to be deleted
		if (strcmp(iter->pKey, key) == 0)
		{
			// First remove this item from the list
			if (iterPrev == m_HashPtrs[hash])
			{
				// Assign new head pointer
				m_HashPtrs[hash] = iter->pNext;
			}
			else
			{
				// Remove from list
				iterPrev->pNext = iter->pNext;
			}

			// Free the resouces for this entry
			free(iter->pKey);
			delete iter;
			return;
		}

		// Save previous iter so we can delete
		iterPrev = iter;
		iter = iter->pNext;
	}
}

VTObject*& VTMapStringToOb::operator[](const char *key)
{
	POSITION		iter;
	unsigned char	hash;

	// Calculate hash and get pointer to first entry in hash array
	hash = HashKey(key);
	iter = m_HashPtrs[hash];

	// Iterate through all items which have this hash
	while (iter != 0)
	{
		if (strcmp(iter->pKey, key) == 0)
		{
			return iter->pObj;
		}

		iter = iter->pNext;
	}

	// Create new entry for this key
	iter = new StrToObStruct;
	iter->pKey = (char *) malloc(strlen(key) + 1);
	strcpy(iter->pKey, key);
	iter->hash = hash;

	// Insert new entry into hash
	if (m_HashPtrs[hash] != 0)
		iter->pNext = m_HashPtrs[hash];
	else
		iter->pNext= 0;
	m_HashPtrs[hash] = iter;
	m_Count++;

	return iter->pObj;
}

void VTMapStringToOb::GetNextAssoc(POSITION& rNextPosition,	MString& rKey, VTObject*& rValue) const
{
	unsigned char	hash;

	// First check if iterator is NULL - end of list
	if (rNextPosition == (POSITION ) 0)
		return;

	// Get Entry for this position
	rKey = rNextPosition->pKey;
	rValue = rNextPosition->pObj;

	// Now update the iterator
	if (rNextPosition->pNext != 0)
	{
		rNextPosition = rNextPosition->pNext;
		return;
	}

	hash = rNextPosition->hash;
	if (hash == 255)
		rNextPosition = (POSITION) 0;
	else
	{
		// Find next pointer in hash table
		hash++;
		while ((m_HashPtrs[hash] == 0) && (hash != 255))
			hash++;
		
		// Check if an entry was found or if we are at the end of the table
		if (m_HashPtrs[hash] == 0)
			rNextPosition = (POSITION) 0;
		else
			rNextPosition = m_HashPtrs[hash];
	}
}

POSITION VTMapStringToOb::GetStartPosition(void)
{
	unsigned char	hash;

	// Start at beginning of hash table
	hash = 0;

	// Find first pointer in hash table
	while ((m_HashPtrs[hash] == 0) && (hash != 255))
		hash++;
	
	// Check if an entry was found or if we are at the end of the table
	if (m_HashPtrs[hash] == 0)
		return (POSITION) 0;
	else
		return m_HashPtrs[hash];

}

void VTMapStringToOb::RemoveAll(void)
{
	POSITION		iter, next;
	int				hash;

	// Start at beginning of hash table
	for (hash = 0; hash < 256; hash++)
	{
		// Get pointer to first item for this index
		iter = m_HashPtrs[hash];
		while (iter != 0)
		{
			free(iter->pKey);
			next = iter->pNext;
			delete iter;

			iter = next;
		}

		m_HashPtrs[hash] = 0;
	}
}

