/*
 * speedtables.h --
 *
 *	Based on hash tables from Tcl.
 *
 * Copyright (c) 1987-1994 The Regents of the University of California.
 * Copyright (c) 1993-1996 Lucent Technologies.
 * Copyright (c) 1994-1998 Sun Microsystems, Inc.
 * Copyright (c) 1998-2000 by Scriptics Corporation.
 * Copyright (c) 2002 by Kevin B. Kenny.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id$
 */

#ifndef _SPEEDTABLES_H
#define _SPEEDTABLES_H

/*
 * Flags
 */
#define KEY_MASK	0x3
#define KEY_MALLOCED	0x1	// Key was allocated by malloc
#define KEY_STATIC	0x2	// Key is static
#define KEY_VOLATILE	0x0	// Key needs to be copied and freed

/*
 * Forward declarations of ctable_HashTable and related types.
 */

typedef struct ctable_HashTable ctable_HashTable;
typedef struct ctable_HashEntry ctable_HashEntry;

typedef unsigned int (ctable_HashKeyProc) (
        ctable_HashTable *tablePtr,
	VOID *keyPtr);

typedef int (ctable_CompareHashKeysProc) (
        ctable_HashTable *tablePtr, 
	VOID *keyPtr,
	ctable_HashEntry *hPtr);

typedef ctable_HashEntry *(ctable_AllocHashEntryProc) (
	ctable_HashTable *tablePtr, VOID *keyPtr);

typedef void (ctable_FreeHashEntryProc) (ctable_HashEntry *hPtr);

/*
 * Structure definition for an entry in a hash table. No-one outside Tcl
 * should access any of these fields directly; use the macros defined below.
 */

struct ctable_HashEntry {
    ctable_HashEntry *nextPtr;	/* Pointer to next entry in this hash bucket,
				 * or NULL for end of chain. */
    char             *key;
    unsigned int      hash;	/* Hash value. */
};

/*
 * Structure definition for the methods associated with a hash table key type.
 */

/*
 * Structure definition for a hash table.  Must be in speedtables.h so clients 
 * can allocate space for these structures, but clients should never access any
 * fields in this structure.
 */

#define CTABLE_SMALL_HASH_TABLE 16
struct ctable_HashTable {
    ctable_HashEntry **buckets;	/* Pointer to bucket array. Each element
				 * points to first entry in bucket's hash
				 * chain, or NULL. */
    ctable_HashEntry *staticBuckets[CTABLE_SMALL_HASH_TABLE];
				/* Bucket array used for small tables (to
				 * avoid mallocs and frees). */
    int numBuckets;		/* Total number of buckets allocated at
				 * **bucketPtr. */
    int numEntries;		/* Total number of entries present in
				 * table. */
    int rebuildSize;		/* Enlarge table when numEntries gets to be
				 * this large. */
    int downShift;		/* Shift count used in hashing function.
				 * Designed to use high-order bits of
				 * randomized keys. */
    int mask;			/* Mask value used in hashing function. */
};

/*
 * Structure definition for information used to keep track of searches through
 * hash tables:
 */

typedef struct ctable_HashSearch {
    ctable_HashTable *tablePtr;	/* Table being searched. */
    int nextIndex;		/* Index of next bucket to be enumerated after
				 * present one. */
    ctable_HashEntry *nextEntryPtr;/* Next entry to be enumerated in the current
				 * bucket. */
} ctable_HashSearch;


void ctable_InitHashTable (ctable_HashTable *tablePtr);

ctable_HashEntry *  ctable_NextHashEntry (ctable_HashSearch * searchPtr);

#endif /* _SPEEDTABLES_H */

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
