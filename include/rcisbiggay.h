#ifndef RCHEADER
#define RCHEADER
#include <curl/curl.h>
#include <string>
#include "include/json.hpp"
#include "include/rapidjson/document.h"

std::string DATA;
using namespace nlohmann;
using namespace rapidjson;
size_t IHCB(void *contents, size_t size, size_t nmemb, std::string *userdata){
	size_t nL = size*nmemb;
	size_t oL = userdata->size();
	userdata->resize(oL + nL);
	std::copy((char*)contents, (char*)contents+nL, userdata->begin()+oL);
	return nL;
}

class RCAPI {
	public:
		RCAPI(std::string url);
		int Login(std::string Username, std::string Password);
		int SendMessage(std::string Channel, std::string Message);		
	private:
		int LoggedIn = 0;
		std::string APIURL = "";
		std::string APIResponse;
		CURL *CurlAPI;
		struct curl_slist *headers = NULL;
};
RCAPI::RCAPI(std::string url){
	APIURL = url;
	CurlAPI = curl_easy_init();
	curl_easy_setopt(CurlAPI, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(CurlAPI, CURLOPT_WRITEFUNCTION, IHCB);
	curl_easy_setopt(CurlAPI, CURLOPT_WRITEDATA, &APIResponse);
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(CurlAPI, CURLOPT_HTTPHEADER, headers);
	
}

int RCAPI::Login(std::string Username, std::string Password){
	json senddata = { {"password", Password.c_str()}, {"username", Username.c_str()}};
	Document JP;
	std::string AuthToken = "X-Auth-Token: ";
	std::string UserId = "X-User-Id: ";
	std::string jsenddata = senddata.dump();
	curl_easy_setopt(CurlAPI, CURLOPT_URL, (APIURL+"login").c_str());
	curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, jsenddata.c_str());
	curl_easy_perform(CurlAPI);
	printf(APIResponse.c_str());
	JP.Parse(APIResponse.c_str());
	const Value& user = JP["data"];
	AuthToken.append(user["authToken"].GetString());
	UserId.append(user["userId"].GetString());
	headers = curl_slist_append(headers, AuthToken.c_str());
	headers = curl_slist_append(headers, UserId.c_str());
	curl_easy_setopt(CurlAPI, CURLOPT_URL, NULL);
	curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, NULL);
       	LoggedIn = 1;	
}
//Please implement better error checking thanks xoxo sleep deprived 179
int RCAPI::SendMessage(std::string Channel, std::string Message){
	if(LoggedIn){
		json sendmessage = {{"channel", Channel.c_str()},{"text", Message.c_str()}};
		std::string jsendmessage = sendmessage.dump();
		curl_easy_setopt(CurlAPI, CURLOPT_URL, (APIURL+"chat.postMessage").c_str());
		curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, jsendmessage.c_str());
		curl_easy_perform(CurlAPI);	
	}

}	


#endif
