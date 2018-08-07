#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/RCAPI.h"
#include <iostream>

REQUIRE( "The login function works correctly and catches errors" ){
	RCAPI h("https://chat.systemspace.link/api/v1/");
	std::string password;
	std::cin >> password;
	REQUIRE( h.login("179", password) == RCAPI_GOOD);
	REQUIRE( h.login("", password) == RCAPI_EMPTYSTRING);
	REQUIRE( h.login("179", "") == RCAPI_EMPTYSTRING);
	REQUIRE( h.login("179", "wrongpassword") == RCAPI_INCORRECTLOGIN);
	REQUIRE( h.login("wrongusername", password) == RCAPI_INCORRECTLOGIN);
}
