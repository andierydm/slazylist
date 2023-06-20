#include "slazylist.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    SLazyList *l = NULL;
    slazylist_create(NULL, &l);

    slazylist_insert(NULL, l, NULL);
    slazylist_insert(NULL, l, NULL);
    slazylist_insert(NULL, l, NULL);
    slazylist_insert(NULL, l, NULL);
    slazylist_insert(NULL, l, NULL);

    slazylist_destroy(&l);

    return 0;
}
