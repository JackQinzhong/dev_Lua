


#include <stdio.h>

//很重要，不然函数没有导出
#define LUA_BUILD_AS_DLL
#define LUA_COMPAT_MODULE


extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment (lib,"LuaDll.lib")


int FUN (lua_State*L)
{
	int a = lua_tointeger(L,1);
	int b = lua_tointeger(L,2);

	lua_pushinteger(L,a+b);

	//1：表示压入站数据个数
	return 1;
}
luaL_Reg funcs [] = 
{
	{"test1",FUN},
	{"test2",FUN},
	{"test3",FUN},
	{"test4",FUN},
	{"test5",FUN},
	{0,0},
};
void printTable(lua_State * pState ,int index)
{

}

void stackDump(lua_State *l)
{
	int i;
	int top = lua_gettop(l);
	printf("---------start------%d\n",top);

	for (i=1;i<top;i++)
	{
		int t = lua_type(l,i);
		printf("type: %s value:",lua_typename(l,t));

		switch (t)
		{
		case LUA_TSTRING:
			printf("%s",lua_tostring(l,i));
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(l,i)? "true":"false");
			break;
		case LUA_TNUMBER:
			printf("%g",lua_tonumber(l,i));
			break;
		case LUA_TTABLE:
			printTable(l,i);
			break;
		default:
			printf("%s",lua_typename(l,t));
			break;

		}
		printf("\n");
	}
	printf("-------end------\n");
}

int main (int argc,char** argv)
{
	//lua envirment
	lua_State * L = luaL_newstate();


	luaopen_base(L);

	luaopen_io(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);

	
	//方法1，不常用，一次只注册一个函数
	lua_register(L,"Fun",FUN);

	luaL_dostring(L,"Fun(111,333)");

	//方法2 注册多个函数
	luaL_openlib(L,"mylib",funcs,0);

	

	stackDump(L);

	//run lua pragram 
	luaL_dostring(L,"print('Hello lua')");

	// run lua file code
	luaL_dofile(L,"test.lua");

	//find addFunc function and push back at bottom of stack
	lua_getglobal(L,"addFunc");
	//first param push back
	lua_pushinteger(L,6);
	//second param push back
	lua_pushinteger(L,5);

	//call addFunc function
	lua_call(L,2,1);//2: number of param  ;  1: number of return value ;
	
	//get value form the top of stack
	int result = lua_tointeger(L,-1);
	//clear stack ,cause one return value
	lua_pop(L,1);

	printf("result = %d",result);


	//close lua envirement
	lua_close(L);

	return 0;
}



