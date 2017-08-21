#ifndef H_PRISM_OBJECTS_INCLUDED
#define H_PRISM_OBJECTS_INCLUDED

#include <Prism/types.h>
#include <Prism/memory.h>
#include <Prism/sigslot.h>

PR_CPP_PROTECT_ON

    #define PR_OBJECT(name_t, name) \
        typedef struct name_t name; \
        typedef name * name##Ref; \
        struct name_t 

    #define Pr_SlotFunction(name) \
        Pr_Slot slot_##name; \
        void (*name)

    #define PR_INHERIT(type) type self

    typedef struct pr_class_t Pr_Class;
   
    PR_OBJECT(pr_object_t, Pr_Object) {
        Pr_Class *  class;
        Pr_Signal * onDelete;
        Pr_Signal * onConstruction;
    };

    struct pr_class_t {
        char const *    name;
        pr_u32_t        objectSize;
        pr_u32_t        refCount;
        Pr_Class *      inherit;
        Pr_MemoryPool * memoryPool;
        pr_u8_t         poolSize;
        pr_bool_t       abstract;

        pr_bool_t   (*construct)(Pr_ObjectRef);
        void        (*destruct)(Pr_ObjectRef);
    };

    extern PRISM_API Pr_Class Pr_ObjectClass;

    extern PRISM_API void *     Pr_New(Pr_Class *);
    extern PRISM_API void       Pr_Delete(Pr_ObjectRef);

    extern PRISM_API void *     Pr_Alloc(Pr_Class *);

    extern PRISM_API pr_bool_t  Pr_Construct(Pr_ObjectRef);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_OBJECTS_INCLUDED */
