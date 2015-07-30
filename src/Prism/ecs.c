/* File name : ecs.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Entity Component System implementation
 * Dependencies :	<Prism/ecs.h>
					<Prism/list.h>, 
					<Prism/array.h>, 
					<string.h>,
					<stdio.h>
 * Last modifications : 29/7/2015
 */

#include <Prism/ecs.h>

#include <Prism/list.h>
#include <Prism/array.h>
#include <string.h>
#include <stdio.h>

struct pr_componentlist_t {
	Pr_String * name;
	Pr_ComponentId id;
	unsigned int sizeAlloc;
	unsigned long freeIndex;
	PR_ARRAY(char, list);
};

struct pr_world_t {
	Pr_List * systems;
	Pr_List * components;
	Pr_EntityId index;
	PR_ARRAY(Pr_List *, entities);
};

typedef struct pr_componentlink_t Pr_ComponentLink;
struct pr_componentlink_t {
	Pr_ComponentId id;
	unsigned long index;
};

static void s_Pr_DeleteComponentList(Pr_ComponentList * ap_list)
{
	if (ap_list) {
		Pr_ClearArray(ap_list->list);
		Pr_DeleteString(ap_list->name);
		free(ap_list);
	}
}

static Pr_String * s_Pr_GetComponentNameString(Pr_List * ap_list, Pr_ComponentId a_id)
{
	Pr_ListIterator lp_it;

	if (!ap_list) return NULL;

	PR_LIST_FOREACH(ap_list, lp_it) {
		Pr_ComponentList * lp_tmp = Pr_ListIteratorData(lp_it);
		if (lp_tmp->id == a_id) {
			return lp_tmp->name;
		}
	}

	return NULL;
}

Pr_ComponentList * Pr_GetComponentListByName(Pr_World * ap_world, char const * ap_name)
{
	Pr_ListIterator l_it;

	if (!ap_world || !ap_name) return NULL;
	if (!ap_world->components) return NULL;

	PR_LIST_FOREACH(ap_world->components, l_it) {
		Pr_ComponentList * lp_tmp = Pr_ListIteratorData(l_it);
		if (lp_tmp) {
			if (strcmp(Pr_StringCStr(lp_tmp->name), ap_name) == 0) {
				return lp_tmp;
			}
		}
	}

	return NULL;
}

Pr_ComponentList * Pr_GetComponentListById(Pr_World * ap_world, Pr_ComponentId a_id)
{
	Pr_ListIterator l_it;

	if (!ap_world) return NULL;
	if (!ap_world->components) return NULL;

	PR_LIST_FOREACH(ap_world->components, l_it) {
		Pr_ComponentList * lp_clist = Pr_ListIteratorData(l_it);
		if (lp_clist) {
			if (lp_clist->id == a_id) {
				return lp_clist;
			}
		}
	}

	return NULL;
}

static Pr_ComponentList * s_Pr_NewComponentList(Pr_ComponentId a_id, unsigned int a_sizeAlloc)
{
	Pr_ComponentList * lp_out;
	char num[32];

	lp_out = malloc(sizeof(Pr_ComponentList));

	if (!lp_out) return NULL;

	sprintf(num, "%d",a_id);
	Pr_MakeArray(lp_out->list);
	lp_out->id = a_id;
	lp_out->sizeAlloc = a_sizeAlloc;
	lp_out->freeIndex = 0;
	lp_out->name = Pr_StringAppendResult("UnnamedComponent", num);

	if (!lp_out->name) {
		free(lp_out);
		return NULL;
	}

	return lp_out;
}

static void s_Pr_RemoveEntities(Pr_World * ap_world)
{
	unsigned int l_i;

	for (l_i = 1; l_i <= ap_world->entities.size ; l_i++) {
		Pr_EntityId l_tmp = l_i;
		Pr_DeleteEntity(ap_world,&l_tmp);
	}
}

static int s_Pr_CreateEntities(Pr_World * ap_world, unsigned long a_size)
{
	Pr_MakeSizedArray(ap_world->entities,a_size);

	if (!ap_world->entities.size) return 0;

	memset(ap_world->entities.list,0,a_size*sizeof(Pr_List*));

	return 1;
}

Pr_World * Pr_NewWorld(unsigned long a_size)
{
	Pr_World * lp_out;
	Pr_List * lp_sys = NULL;
	Pr_List * lp_com = NULL;

	lp_out = malloc(sizeof(Pr_World));

	if (lp_out) {
		int l_bool = s_Pr_CreateEntities(lp_out,a_size);
		lp_sys = Pr_NewList();
		lp_com = Pr_NewList();

		if (lp_sys && lp_com && l_bool) { 
			lp_out->systems = lp_sys;
			lp_out->components = lp_com;
			lp_out->index = 1;
			return lp_out;
		}

		s_Pr_RemoveEntities(lp_out);
		free(lp_out);
		Pr_DeleteList(lp_sys);
		Pr_DeleteList(lp_com);
	}

	return NULL;
}

void Pr_DeleteWorld(Pr_World * ap_world)
{
	Pr_ListIterator l_it;

	if (!ap_world) return;

	s_Pr_RemoveEntities(ap_world);
	Pr_ClearArray(ap_world->entities);
	Pr_DeleteList(ap_world->systems);

	if (ap_world->components) {
		PR_LIST_FOREACH(ap_world->components, l_it) {
			Pr_ComponentList * lp_tmp = Pr_ListIteratorData(l_it);
			if (lp_tmp) {
				s_Pr_DeleteComponentList(lp_tmp);
			}
		}

		Pr_DeleteList(ap_world->components);
	}

	free(ap_world);
}

int Pr_ComponentRegistered(Pr_World * ap_world, Pr_ComponentId a_id)
{
	return Pr_GetComponentName(ap_world, a_id) ? 1 : 0;
}


char * Pr_GetComponentName(Pr_World * ap_world, Pr_ComponentId a_id)
{
	if (!ap_world) return NULL;

	return Pr_StringCStr(s_Pr_GetComponentNameString(ap_world->components, a_id));
}

Pr_Strresult Pr_SetComponentName(Pr_World * ap_world, Pr_ComponentId a_id, char const * ap_name)
{
	if (ap_world && ap_name) {
		if (ap_world->components && ap_name[0] != '\0') {
			Pr_ComponentList * lp_tmp = Pr_GetComponentListByName(ap_world, ap_name);
			if (!lp_tmp) {
				Pr_String * lp_s = s_Pr_GetComponentNameString(ap_world->components, a_id);
				if (lp_s) {
					return Pr_SetStringStr(lp_s, ap_name);
				}
			}
		}
	}

	return PR_STRING_UNMODIFIED;
}

int	Pr_RegisterComponent(Pr_World * ap_world, Pr_ComponentId a_id, unsigned int a_sizeAlloc)
{
	Pr_ComponentList * lp_new;

	if (!ap_world || a_sizeAlloc < 1) return 0;

	if (Pr_ComponentRegistered(ap_world, a_id)) return 0;

	lp_new = s_Pr_NewComponentList(a_id, a_sizeAlloc);

	if (!lp_new) return 0;

	if (!Pr_PushBackListData(ap_world->components, lp_new)) {
		s_Pr_DeleteComponentList(lp_new);
		return 0;
	}

	return 1;
}

int Pr_AddWorldSystem(Pr_World * ap_world, Pr_System ap_sys)
{
	if (!ap_world || !ap_sys) return 0;

	return Pr_PushBackListData(ap_world->systems, ap_sys);
}

static void s_Pr_FindNextEntitySlot(Pr_World * ap_world)
{
	unsigned l_i;
	
	for (l_i = 0; l_i < ap_world->entities.size; l_i++) {
		if (!ap_world->entities.list[l_i]) {
			ap_world->index = l_i + 1;
			return;
		}
	}

	ap_world->index = 0;
}

Pr_EntityId	Pr_NewEntity(Pr_World * ap_world)
{
	Pr_List * lp_list;
	Pr_EntityId l_out;

	if (!ap_world) return 0;
	if (!ap_world->entities.size) return 0;
	if (!ap_world->index) return 0;

	lp_list = Pr_NewList();

	if (!lp_list) return 0;

	ap_world->entities.list[ap_world->index - 1] = lp_list;

	l_out = ap_world->index;

	s_Pr_FindNextEntitySlot(ap_world);

	return l_out;
}

static void s_Pr_RemoveFastComponent(Pr_World * ap_world, Pr_ComponentLink * ap_link)
{
	Pr_ListIterator l_it;

	PR_LIST_FOREACH(ap_world->components, l_it) {
		Pr_ComponentList * lp_clist = Pr_ListIteratorData(l_it);
		if (lp_clist) {
			if (lp_clist->id == ap_link->id) {
				lp_clist->freeIndex = ap_link->index;
				return;
			}
		}
	}
}

void Pr_DeleteEntity(Pr_World * ap_world, Pr_EntityId * ap_id)
{
	Pr_List * lp_list;
	Pr_ListIterator l_it;

	if (!ap_world || !ap_id) return;
	if (!ap_world->entities.size) return;
	if (!ap_world->components || !*ap_id) return;

	lp_list = ap_world->entities.list[*ap_id-1];

	if (!lp_list) return;

	PR_LIST_FOREACH(lp_list, l_it) {
		Pr_ComponentLink * lp_link = Pr_ListIteratorData(l_it);
		if (lp_link) {
			s_Pr_RemoveFastComponent(ap_world,lp_link); 
			free(lp_link);
			Pr_SetListIteratorData(l_it,NULL);
		}
	}

	Pr_DeleteList(lp_list);
	ap_world->entities.list[*ap_id-1] = NULL;

	*ap_id = 0;
}

static int s_Pr_AddFastComponnent(Pr_ComponentList * ap_clist, unsigned long * ap_index)
{
	unsigned long l_i = ap_clist->list.size / ap_clist->sizeAlloc;

	if (ap_clist->freeIndex >= l_i) {
		Pr_ExtendArray(ap_clist->list, ap_clist->sizeAlloc);
		if (l_i == ap_clist->list.size) return 0;
	} else {
		l_i = ap_clist->freeIndex;
	}

	ap_clist->freeIndex = ap_clist->list.size / ap_clist->sizeAlloc;
	*ap_index = l_i;

	return 1;
}

int	Pr_AddEntityComponent(Pr_World * ap_world, Pr_EntityId a_entity, Pr_ComponentId a_component)
{
	Pr_List * lp_components;
	Pr_ComponentLink * lp_link;
	Pr_ComponentList * lp_clist;
	unsigned long lp_index;

	if (!ap_world || !a_entity) return 0;
	if (!Pr_ComponentRegistered(ap_world, a_component)) return 0;
	if (Pr_HasComponentId(ap_world, a_entity, a_component)) return 0;

	lp_components = ap_world->entities.list[a_entity - 1];
	if (!lp_components) return 0;

	lp_clist = Pr_GetComponentListById(ap_world, a_component);
	if (!lp_clist) return 0;

	lp_link = malloc(sizeof(Pr_ComponentLink));
	if (lp_link) {
		if (Pr_PushBackListData(lp_components, lp_link)) {
			if (s_Pr_AddFastComponnent(lp_clist, &lp_index)) {
				lp_link->id = a_component;
				lp_link->index = lp_index;
				return 1;
			}
			Pr_PopBackList(lp_components);
		}
		free(lp_link);
	}

	return 0;
}

int	Pr_HasComponentId(Pr_World * ap_world, Pr_EntityId a_entity, Pr_ComponentId a_component)
{
	Pr_ListIterator l_it;
	Pr_List * lp_list;

	if (!ap_world || !a_entity) return 0;

	lp_list = ap_world->entities.list[a_entity];
	if (!lp_list) return 0;

	PR_LIST_FOREACH(lp_list, l_it) {
		Pr_ComponentLink * lp_link = Pr_ListIteratorData(l_it);
		if (lp_link) {
			if (lp_link->id == a_component) {
				return 1;
			}
		}
	}

	return 0;
}

void * Pr_GetEntityComponent(Pr_World * ap_world, Pr_EntityId a_entity, Pr_ComponentId a_component)
{
	Pr_ComponentList * lp_clist;
	Pr_List * lp_list;
	Pr_ListIterator l_it;

	if (!a_entity) return NULL;

	lp_clist = Pr_GetComponentListById(ap_world,a_component);
	if (!lp_clist) return NULL;

	lp_list = ap_world->entities.list[a_entity-1];
	if (!lp_list) return NULL;

	PR_LIST_FOREACH(lp_list, l_it) {
		Pr_ComponentLink * lp_link = Pr_ListIteratorData(l_it);
		if (lp_link) {
			if (lp_link->id == a_component) {
				return lp_clist->list.list + lp_link->index * lp_clist->sizeAlloc;
			}
		}
	}

	return NULL;
}

void * Pr_GetComponents(Pr_ComponentList * ap_clist)
{
	return ap_clist ? ap_clist->list.list : NULL;
}

unsigned long Pr_GetComponentListSize(Pr_ComponentList * ap_clist)
{
	return ap_clist ? ap_clist->list.size : 0;
}

void Pr_UpdateWorld(Pr_World * ap_world, float a_dT)
{
	Pr_ListIterator l_it;

	if (!ap_world) return;
	if (!ap_world->systems) return;

	PR_LIST_FOREACH(ap_world->systems, l_it) {
		Pr_System lp_sys = Pr_ListIteratorData(l_it);
		lp_sys(ap_world,a_dT);
	}
}
