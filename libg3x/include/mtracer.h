/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie mtracer - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2008                        
  ============================================================= */
#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

#ifdef _MTRACER_
# ifndef _MTRACER_H
#   define _MTRACER_H

#   if _MTRACER_<0 || _MTRACER>4
#     warning INVALID PARAMETER _MTRACER_ -> USE DEFAULT VALUE _MTRACER_=1 
#     undef  _MTRACER_
#     define _MTRACER_ 1
#   endif	

#   include <stdlib.h>
#   include <string.h>
	
		void  _mtrac_auto_start_(int);
		void  _mtracer_(int* argc, char* argv[]);
		void  _mtrac_stop_(int reset);
	  void  _mtrac_check_(int close);
#   define GETINFO ( _mtrac_info_ ? _mtrac_info_(__LINE__,__FILE__) : 1 )
#   define calloc  ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_calloc_  )
#   define malloc  ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_malloc_  )
#   define memset  ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_memset_  )
#   define memcpy  ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_memcpy_  )
#   define memmove ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_memmove_ )
#   ifdef _SET_FREED_TO_NULL_
#     warning FREED ADDRESS IS AUTOMATICALLY SET TO NULL : free(ptr) -> (free(ptr),(ptr)=NULL) 
#     define _free ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_free_    )
#     define free(ptr) (_free(ptr), (ptr)=NULL)
#     define _realloc ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_realloc_ )
      static void* _mtrac_ptr_=NULL;
#     define realloc(ptr,size) ((ptr)?(_mtrac_ptr_=malloc(size),\
                                        memcpy(_mtrac_ptr_,ptr,size),\
													              free(ptr),\
													             _mtrac_ptr_)\
																			:_realloc((ptr),(size)))
#   else
#     define free    ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_free_    ) 
#     define realloc ( _mtrac_auto_start_(_MTRACER_), GETINFO , *_mtrac_realloc_ )
#   endif	

	  void  (*_mtrac_info_)();
	  void* (*_mtrac_calloc_ )(size_t nmemb, size_t size);
	  void* (*_mtrac_malloc_ )(size_t size);
	  void* (*_mtrac_realloc_)(void * ptr, size_t size);
	  void  (*_mtrac_free_   )(void *ptr);
	  void* (*_mtrac_memset_ )(void *ptr, int c, size_t size);
		void* (*_mtrac_memcpy_ )(void *dest, const void *src, size_t size);
		void* (*_mtrac_memmove_)(void *dest, const void *src, size_t size);

#endif
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
