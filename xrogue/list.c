/*
 * Functions for dealing with linked lists of goodies
 */

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "rogue.h"

/*
 * detach:
 *      Takes an item out of whatever linked list it might be in
 */

void
_detach(register struct linked_list **list, register struct linked_list *item)
{
    if (*list == item)
        *list = next(item);
    if (prev(item) != NULL) item->l_prev->l_next = next(item);
    if (next(item) != NULL) item->l_next->l_prev = prev(item);
    item->l_next = NULL;
    item->l_prev = NULL;
}

/*
 * _attach:
 *      add an item to the head of a list
 */

void
_attach(register struct linked_list **list, register struct linked_list *item)
{
    if (*list != NULL)
    {
        item->l_next = *list;
        (*list)->l_prev = item;
        item->l_prev = NULL;
    }
    else
    {
        item->l_next = NULL;
        item->l_prev = NULL;
    }

    *list = item;
}

/*
 * o_free_list:
 *      Throw the whole object list away
 */

void
_o_free_list(register struct linked_list **ptr)
{
    register struct linked_list *item;

    while (*ptr != NULL)
    {
        item = *ptr;
        *ptr = next(item);
        o_discard(item);
    }
}

/*
 * o_discard:
 *      free up an item and its object(and maybe contents)
 */

void
o_discard(register struct linked_list *item)
{
    register struct object *obj;

    obj = OBJPTR(item);
    if (obj->contents != NULL)
        o_free_list(obj->contents);
    total -= 2;
    FREE(obj);
    FREE(item);
}

/*
   r_free_fire_list
       Throw the whole list of fire monsters away. But don't
       discard the item (monster) itself as that belong to mlist.
*/

void
_r_free_fire_list(register struct linked_list **ptr)
{
    register struct linked_list *item;

        while (*ptr != NULL)
        {
            item = *ptr;
            *ptr = next(item);
            free(item);
        }
}
/*
 * r_free_list:
 *      Throw the whole list of room exits away
 */

void
_r_free_list(register struct linked_list **ptr)
{
    register struct linked_list *item;

    while (*ptr != NULL)
    {
        item = *ptr;
        *ptr = next(item);
        r_discard(item);
    }
}

/*
 * r_discard:
 *      free up an item and its room
 */

void
r_discard(register struct linked_list *item)
{
    total -= 2;
    FREE(DOORPTR(item));
    FREE(item);
}

/*
 * t_free_list:
 *      Throw the whole thing list away
 */

void
_t_free_list(register struct linked_list **ptr)
{
    register struct linked_list *item;

    while (*ptr != NULL)
    {
        item = *ptr;
        *ptr = next(item);
        t_discard(item);
    }
}

/*
 * t_discard:
 *      free up an item and its thing
 */

void
t_discard(register struct linked_list *item)
{
    register struct thing *tp;

    total -= 2;
    tp = THINGPTR(item);
    if (tp->t_name != NULL) FREE(tp->t_name);
    if (tp->t_pack != NULL)
        o_free_list(tp->t_pack);
    FREE(tp);
    FREE(item);
}

/*
 * destroy_item:
 *      get rid of an item structure -- don't worry about contents
 */

void
destroy_item(register struct linked_list *item)
{
    total--;
    FREE(item);
}

/*
 * new_item
 *      get a new item with a specified size
 */

struct linked_list *
new_item(int size)
{
    register struct linked_list *item;

    if ((item = (struct linked_list *) new(sizeof *item)) == NULL)
        msg("Ran out of memory for header after %d items", total);
    if ((item->l_data = new(size)) == NULL)
        msg("Ran out of memory for data after %d items", total);
    item->l_next = item->l_prev = NULL;
    memset(item->l_data,0,size);
    return item;
}

/*
 * creat_item:
 *      Create just an item structure -- don't make any contents
 */

struct linked_list *
creat_item()
{
    register struct linked_list *item;

    if ((item = (struct linked_list *) new(sizeof *item)) == NULL)
        msg("Ran out of memory for header after %d items", total);
    item->l_next = item->l_prev = NULL;
    return item;
}

char *
new(int size)
{
    register char *space = ALLOC(size);

    if (space == NULL) {
        sprintf(prbuf,"Rogue ran out of memory (used = %ld, wanted = %d).",
                md_memused(), size);
        fatal(prbuf);
    }
    total++;
    return space;
}

