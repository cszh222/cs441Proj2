
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* Added JWJ */
#ifdef __cplusplus
extern "C" {
#endif
	
struct symbol_entry {
	int 	addr;
	int 	blk_level;
	int 	type;
	int 	size;
	int 	offset;
	bool    refetch;
	bool    scoped;
	int     prev_type;
	symbol_entry* relentry;
    union { /* Added */
      int iVal;
      float fVal;
      int pAddr;
    };
	const char *name; /* changed JWJ */
};

typedef struct symbol_entry symbol_entry;

extern void printSymbolTable();
extern void popSymbolTable();
extern void pushSymbolTable();    // once
extern void addSymbol(struct symbol_entry*, int lineno); // need this
extern struct symbol_entry* getSymbolEntryByRelAddr(int, int);
extern struct symbol_entry* getSymbolEntry(const char *); // need this
extern int getTotalSymbolTableSize();
extern int getCurrentSymbolTableSize();
extern int getSymbolTableSize(int);
extern int getCurrentLevel();

/* Added JWJ */
#ifdef __cplusplus
}
#endif

#endif /* SYMBOL_TABLE_H */

