// TODO: free stack itc
#define __ ,

#define _HLT														\
{																	\
	return 0;														\
}

#define _TOP														\
{																	\
	StackTop(&cpu_storage.stk);											\
}

#define _PUSH(arg)													\
{																	\
	StackPush(&cpu_storage.stk __ (arg));										\
}

#define _POP														\
{																	\
	StackPop(&cpu_storage.stk);											\
}

#undef __