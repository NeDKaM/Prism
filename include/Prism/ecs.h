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

#include <Prism/config.h>
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

	extern PRISM_API Pr_World *         Pr_NewWorld(unsigned long);
	extern PRISM_API Pr_EntityId        Pr_NewEntity(Pr_World *);
	extern PRISM_API void *             Pr_GetEntityComponent(Pr_World *, Pr_EntityId, Pr_ComponentId);
	extern PRISM_API int                Pr_HasComponentId(Pr_World *, Pr_EntityId, Pr_ComponentId);
	extern PRISM_API int                Pr_AddEntityComponent(Pr_World *, Pr_EntityId, Pr_ComponentId);
	extern PRISM_API void               Pr_DeleteEntity(Pr_World *, Pr_EntityId *);
	extern PRISM_API int                Pr_AddWorldSystem(Pr_World *, Pr_System);
	extern PRISM_API int                Pr_RegisterComponent(Pr_World *, Pr_ComponentId, unsigned int);
	extern PRISM_API char *             Pr_GetComponentName(Pr_World *, Pr_ComponentId);
	extern PRISM_API int                Pr_ComponentRegistered(Pr_World *, Pr_ComponentId);
	extern PRISM_API void               Pr_DeleteWorld(Pr_World *);
	extern PRISM_API void *             Pr_GetComponents(Pr_ComponentList *);
	extern PRISM_API void               Pr_UpdateWorld(Pr_World *, float);
	extern PRISM_API Pr_ComponentList * Pr_GetComponentListById(Pr_World *, Pr_ComponentId);
	extern PRISM_API Pr_ComponentList * Pr_GetComponentListByName(Pr_World *, char const *);
	extern PRISM_API Pr_Strresult       Pr_SetComponentName(Pr_World *, Pr_ComponentId, char const *);
	extern PRISM_API unsigned long      Pr_GetComponentListSize(Pr_ComponentList *);
	extern PRISM_API int                Pr_AddWorldSystem(Pr_World *, Pr_System);

#ifdef __cplusplus
}
#endif

#endif /* H_PRISM_ECS_INCLUDED */
