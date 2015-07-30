/* File name : ecs.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Entity Component System implementation
 * Dependencies :	<Prism/string.h>
 * Last modifications : 29/7/2015
 */

#ifndef H_PRISM_ECS_INCLUDED
#define H_PRISM_ECS_INCLUDED

#include <Prism/string.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct pr_world_t			Pr_World;
	typedef struct pr_componentlist_t	Pr_ComponentList;

	typedef void (*Pr_System)(Pr_World *, float);

	typedef unsigned long Pr_EntityId;
	typedef unsigned long Pr_ComponentId;

	Pr_World *	Pr_NewWorld	(unsigned long);
	Pr_EntityId	Pr_NewEntity(Pr_World *);
	
	void *	Pr_GetEntityComponent	(Pr_World *, Pr_EntityId, Pr_ComponentId);
	int		Pr_HasComponentId		(Pr_World *, Pr_EntityId, Pr_ComponentId);
	int		Pr_AddEntityComponent	(Pr_World *, Pr_EntityId, Pr_ComponentId);
	void	Pr_DeleteEntity			(Pr_World *, Pr_EntityId *);
	int		Pr_AddWorldSystem		(Pr_World *, Pr_System);
	int		Pr_RegisterComponent	(Pr_World *, Pr_ComponentId, unsigned int);
	char *	Pr_GetComponentName		(Pr_World *, Pr_ComponentId);
	int		Pr_ComponentRegistered	(Pr_World *, Pr_ComponentId);
	void	Pr_DeleteWorld			(Pr_World *);
	void *	Pr_GetComponents		(Pr_ComponentList *);
	void	Pr_UpdateWorld			(Pr_World *, float);

	Pr_ComponentList *	Pr_GetComponentListById		(Pr_World *, Pr_ComponentId);
	Pr_ComponentList *	Pr_GetComponentListByName   (Pr_World *, char const *);
	Pr_Strresult		Pr_SetComponentName			(Pr_World *, Pr_ComponentId, char const *);
	unsigned long		Pr_GetComponentListSize		(Pr_ComponentList *);

	int Pr_AddWorldSystem(Pr_World *, Pr_System); 

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_ECS_INCLUDED */
