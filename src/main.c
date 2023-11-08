#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"

int main()
{
    // test create
    void *symtab = symtabCreate(5);
    if (symtab == NULL)
    {
        fprintf(stderr, "symtabCreate Failed!\n");
        return -1;
    }

    // test install with a large number of characters
    for (int i = 33; i <= 122; i++)
    {
        void *info = (void*) &i;
        char temp_item = i + '0';
        const char *item = &temp_item;
        if (!symtabInstall(symtab, item, info))
        {
            fprintf(stderr, "symtabInstall Failed\n");
            exit(-1);
        }
        if ((long) symtabLookup(symtab, item) != (long) info)
        {
            fprintf(stderr, "symtabLookup Error.\n");
            exit(-1);
        }
    }
    
    // // print out all values from iterator
    // void *it = symtabCreateIterator(symtab);
    // while (it)
    //     printf("%s, ", symtabNext(it));
    // printf("\n");
    // // delete character symbol table
    // symtabDelete(symtab);
    // symtabDeleteIterator(it);

    return 0;
}

