#ifndef _HASH_H_
#define _HASH_H_
#endif

#define HASHSIZE 101

struct nlist{
	struct nlist *next;
	char * name;
	char * defn;

};

static struct nlist * hashtable[HASHSIZE];
/*lookup/search if name member exists*/
struct nlist * lookup(char * name);
/* install hastable entry; name, defn members*/
struct nlist * install(char * name, char * defn);
/*generate hashtable index*/
unsigned hash(char * s);
