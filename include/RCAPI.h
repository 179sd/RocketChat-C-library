            /////////////////          /////////////////    ///////////////////   ////////////////////    ///////////////////////
           ///           ///          ///                  ///             ///   ///              ///              ///
          ///           ///          ///                  ///             ///   ///              ///              ///
         ///           ///          ///                  ///             ///   ///              ///              ///
        /////////////////          ///                  ///////////////////   ////////////////////              ///
       ///            \\\         ///                  ///             ///   ///                               ///
      ///              \\\       ///                  ///             ///   ///                               ///
     ///                \\\     ///                  ///             ///   ///                               ///
    ///                  \\\   //////////////////   ///             ///   ///                     ////////////////////////

#ifndef RCHEADER
#define RCHEADER

//Includes
#include <curl/curl.h>
#include <string>
#include "include/tao/json.hpp"

//Namespaces
using namespace ;

//Typedef
typedef const unsigned int ERROR;
typedef const unsigned int GOOD;

//Error 5012 means the error code was not set yet!!! Remind me please!!!
//Good codes
GOOD RCAPI_GOOD = 0;
GOOD MESSAGEOBJ_GOOD = 1;

//Error codes
//General Errors 11 - 20
ERROR RCAPI_NOTLOGGEDIN = 11;
ERROR RCAPI_EMPTYSTRING = 12;

//Connection Errors 21-30

//Message function errors 31-40
ERROR RCAPI_NOMESSAGEORATTACHMENT = 31;

//Login function errors 41-50
ERROR RCAPI_INCORRECTLOGIN = 41;

//Message Obj 51-60
ERROR MESSAGEOBJ_NOVALUE = 51;

//Array storing message request parameters
const std::string MESSAGEVARS[] = {
	"channel",
	"text",
	"alias",
	"emoji",
	"avatar",
	"color",
	"text",
	"ts",
	"thumb_url",
	"message_url",
	"collapsed",
	"author_name",
	"author_link",
	"title",
	"title_link",
	"title_link_download",
	"image_url",
       	"audio_url",
	"video_url"	
}
//Callback function for libcUrl to writedata to a string
//Notice the lack of error catching
//I like to live dangerously ;)
size_t IHCB(void *contents, size_t size, size_t nmemb, std::string *userdata){
	size_t nL = size*nmemb;
	size_t oL = userdata->size();
	userdata->resize(oL + nL);
	std::copy((char*)contents, (char*)contents+nL, userdata->begin()+oL);
	return nL;
}


//The RCAPI object
//Will allow you to have multiple instances of different accounts, perfect for mutiple servers
class RCAPI {

	public:
		//Constructor
		RCAPI(std::string url);
		//Destructor
		~RCAPI();
		//Login function
		int Login(std::string Username, std::string Password);

		//Logout function
		int Logout();

		//Message send function
		int SendMessage(std::string Channel, rcmessage Message);		
	
	private:
		//login status
		bool LoggedIn = false;
		
		//Rocket Chat API URL
		std::string APIUrl = "";
		
		//Response from API call
		std::string APIResponse;
		
		//Curl stuff
		//Information about the connection
		CURL *CurlAPI;
		CURLcode RequestResponse;
		
		//http header
		struct curl_slist *headers = NULL;
		
		//Function for sending data
		int SendData(std::string Data, std::string Url);

		//Function for extracting data from json
		int ExtractData(std::string Json, std::string key);
};

RCAPI::RCAPI(std::string url){
	//Store the url given
	APIUrl = url;

	//Initiate Curl
	CurlAPI = curl_easy_init();
	
	//Curl options
	//Sets request to POST
	curl_easy_setopt(CurlAPI, CURLOPT_CUSTOMREQUEST, "POST");
	
	//Declares callback function for writing data recieved
	curl_easy_setopt(CurlAPI, CURLOPT_WRITEFUNCTION, IHCB);
	
	//Declares string to write to
	curl_easy_setopt(CurlAPI, CURLOPT_WRITEDATA, &APIResponse);
	
	//Sets recieve and request type to json 
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	
	//Points it to the header struct
	curl_easy_setopt(CurlAPI, CURLOPT_HTTPHEADER, headers);
	
}

int RCAPI::SendData(std::string Data, std::string Url){
	curl_easy_setopt(CurlAPI, CURLOPT_URL, (APIUrl+APIURLLINK[]).c_str());
	curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, Data.c_str());
	
}

int RCAPI::ExtractData(std::string Json, std::string Key, std::string Type, std::string * Return){
	switch(Type) {
		case("string")
			*ReturnString = JsonParse[Key].GetString();

	}
	

}

int RCAPI::Login(std::string Username, std::string Password;){
	if(!LoggedIn){
		std::string jsenddata = "{\"username\":\"" + Username + "\", \"password\":\"" + Password + "\"}";

		//rapidjson variable used to parse data recieved
		//Document JP;
	
		//Authentication token
		std:::string AuthToken = "X-Auth-Token: ";
	
		//User ID
		std::string UserId = "X-User-Id: ";
	
		//Curl options
		//Combines the api url and outputs it as a c string
		curl_easy_setopt(CurlAPI, CURLOPT_URL, (APIURL+"login").c_str());
		
		//Puts the data in the post fields to be sent
		curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, jsenddata.c_str());

		//Sends the request
		curl_easy_perform(CurlAPI);
		
		/*
		//Parses the response
		JP.Parse(APIResponse.c_str());
		
		//Extracts the data json string
		const Value& user = JP["data"];
	
		//Extracts the authToken from data
		AuthToken.append(user["authToken"].GetString());
	
		//Extracts the userID from data
		UserId.append(user["userId"].GetString());
		*/

		AuthToken.append();
		UserId.append();
		//Appends the userID and AuthToken to the header
		headers = curl_slist_append(headers, AuthToken.c_str());
		headers = curl_slist_append(headers, UserId.c_str());

		//Clears the options to prevent anything bad from happening ;)
		curl_easy_setopt(CurlAPI, CURLOPT_URL, NULL);
		curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, NULL);
		LoggedIn = true;	
	}
	else {
		
	}
}
//Going to reimplement the message function to support a message object.

int RCAPI::SendMessage(MessageObj Message){
	if(LoggedIn){
		
		return RCAPI_GOOD;
	}
	else{
		return RCAPI_NOTLOGGEDIN;
	}

}	
class MessageObj {
	public:
		std::string Channel = "";
		std::string Text = "";
		std::string Alias = "";
		std::string Emoji = "";
		std::string Avatar = "";

		std::string AColor = "";
		std::string AText = "";
		std::string ATs = "";
		std::string AThumb_Url = "";
		std::string AMessage_Link = "";
		std::string ACollapsed = "";
		std::string AAuthor_Name = "";
		std::string AAuthor_Link = "";
		std::string AAuthor_Icon = "";
		std::string ATitle = "";
		std::string ATitle_Link = "";
		std::string ATitle_Link_Download = "";
		std::string AImage_Url = "";
		std::string AAudio_Url = "";
		std::string AVideo_Url = "";
	
	private:
		int AssembleJson();
		int JsonFormat(int ConstArray, std::string Value);
};

int MessageObj::JsonFormat(int ConstArray, std::string Value = "", std::string * RetJson){
	 if(Value != ""){
		*RetJson.append("\"" + MESSAGEVARS[ConstArray] + "\":\"" + Value + "\",");
	 	return MESSAGEOBJ_GOOD;
	}
	if(ConstArray == -1){
		*RetJson.append("\"a\":\"b\"}");
		return MESSAGEOBJ_GOOD
	}
	else
		return MESSAGEOBJ_;
}

int MessageObj::AssembleJson(std::string * MessageJson){
	std::string MessageJson = "{";
	int CurrentParameter = 0;

	JsonFormat(CurrentParameter, Channel);
	JsonFormat(++CurrentParameter, Text);
	JsonFormat(++CurrentParameter, Alias);
	JsonFormat(++CurrentParameter, Emoji);
	JsonFormat(++CurrentParameter, Avatar);
	JsonFormat(++CurrentParameter, AColor);
	JsonFormat(++CurrentParameter, AText);
	JsonFormat(++CurrentParameter, ATs);
	JsonFormat(++CurrentParameter, AThumb_Url);
	JsonFormat(++CurrentParameter, AMessage_Link);
	JsonFormat(++CurrentParameter, ACollapsed);
	JsonFormat(++CurrentParameter, AAuthor_Name);
	JsonFormat(++CurrentParameter, AAuthor_Link);
	JsonFormat(++CurrentParameter, ATitle);
	JsonFormat(++CurrentParameter, ATitle_Link);
	JsonFormat(++CurrentParameter, ATitle_Link_Download);
	JsonFormat(++CurrentParameter, AImage_Url);
	JsonFormat(++CurrentParameter, AAudio_Url);
	JsonFormat(++CurrentParameter, AVideo_Url);
	JsonFormat(-1);

	return MessageJson;
}
#endif
