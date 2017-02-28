#pragma once
#include "std.h"

/* Standard destructor signature */
typedef void destructor(void *);

/* "m" suffix = "mutator" */

/* Action run on data, with optional external state (e.g. iterator/foreach) */
typedef void action(void *state, const void *item);
typedef void actionm(void *state, void *item);

/* Abortable foreach, filter/where, reduction/folding */
typedef bool predicate(void *state, const void *item);
typedef bool predicatem(void *state, void *item);

#define DECLARE_DESTRUCTOR(name, item_type) \
	typedef void destructor_##name(item_type item);

#define DECLARE_ACTION(name, state_type, item_type) \
	typedef void action_##name(state_type state, const item_type item); \
	typedef void actionm_##name(state_type state, item_type item);

#define DECLARE_PREDICATE(name, state_type, item_type) \
	typedef bool predicate_##name(state_type state, const item_type item); \
	typedef bool predicatem_##name(state_type state, item_type item);
