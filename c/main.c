#include <stdio.h>
#include "glist.h"


void print(const void* e)
{
    printf("%d\n", *(int*)e);
}

int cmp_int(const void* a, const void* b)
{
	int ia = *(int*)a;
	int ib = *(int*)b;
	return (ib > ia ? 1 : (ib == ia ? 0 : -1));
}

void glist_test()
{
	struct generic_list list;
    generic_list_create(int, &list);

    generic_list_t *plist = &list;

	for (int i = 0; i < 3; i++) {
		generic_list_push_back(plist, &i);
	}

    //printf("%d\n", *(int*)generic_list_front(plist));
    generic_list_foreach(plist, print);
	
	generic_list_t set;
	generic_list_create(int, &set);
	int a = 3;
	generic_list_add(&set, &a, &cmp_int);
	generic_list_add(&set, &a, &cmp_int);
	int b = 4;
	generic_list_add(&set, &b, &cmp_int);
	printf("-------\n");
	generic_list_foreach(&set, print);
}

int main()
{
    glist_test();
	getchar();

    return 0;
}
