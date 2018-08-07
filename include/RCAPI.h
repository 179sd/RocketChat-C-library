#ifndef RCHEADER
#define RCHEADER

//Includes
#include <curl/curl.h>
#include <string>
#include "include/json.hpp"
#include "include/rapidjson/document.h"

//Namespaces
using namespace nlohmann;
using namespace rapidjson;

//Error Consts
const unsigned int RCAPI_GOOD = 0;
//General Errors 1 - 10
const unsigned int RCAPI_NOTLOGGEDIN = 1;
const unsigned int RCAPI_EMPTYSTRING = 2;

//Connection Errors 11-20

//Message function errors 21-30
const unsigned int RCAPI_NOMESSAGEORATTACHMENT = 21;

//Login function errors 31-40
const unsigned int RCAPI_INCORRECTLOGIN = 32;

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

//Prepareing struct and linked list to finsh messaging functionality
//Will include attachments
//Message - The actual message, you can change things like your nickname
//Avatar or add a attachment.
typedef struct MessageBlob{
	std::string Text = "";
	std::string Alias = "";
	std::string Emoji = "";
	std::string Avatar = "";
	rcattachment *Attachment = NULL; 

} rcmessage;
//Attachment- Allows you to embed a Picture, Video, Or Audio into 
//your message.
typedef struct AttachmentBlob{
	std::string Color = "";
	std::string Text = "";
	std::string Ts = "";
	std::string Thumb_Url = "";
	std::string Message_Link = "";
	std::string Collapsed = "";
	std::string Author_Name = "";
	std::string Author_Link = "";
	std::string Author_Icon = "";
	std::string Title = "";
	std::string Title_Link = ";
	std::string Title_Link_Download = "";
	std::string Image_Url = "";
	std::string Audio_Url = "";
	std::string Video_Url = "";
	rcfields *fields = NULL;

} rcattachment;

//Fields - Some extra fields to describe the attachments
//It's a linked list as RC API allows you to have multiple
typedef struct AttachmentFields{
	std::string Short;
	std::string Title;
	std::string Value;
	rcfields *Next;
} rcfields;

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
		std::string APIURL = "";
		//Response from API call
		std::string APIResponse;
		//Curl stuff
		//Information about the connection
		CURL *CurlAPI;
		CURLcode RequestResponse;
		//http header
		struct curl_slist *headers = NULL;
};
RCAPI::RCAPI(std::string url){
	//Store the url given
	APIURL = url;
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

int RCAPI::Login(std::string Username, std::string Password){
	//nlohmann json variable containing username and password to send
	//json senddata = { {"password", Password.c_str()}, {"username", Username.c_str()}};
		
	std::string jsenddata = "{\"username\":\"" + Username + "\", \"password\":\"" + Password + "\"}";

	json senddata = { {"password", Password.c_str()}, {"username", Username.c_str()}};

	//rapidjson variable used to parse data recieved
	Document JP;

	//Authentication token
	std:::string AuthToken = "X-Auth-Token: ";
	
	//User ID
	std::string UserId = "X-User-Id: ";

	//User ID
	std::string UserId = "X-User-Id: ";
	
	//Dumps the json into a string because of a wierd bug with libcurl
	std::string jsenddata = senddata.dump();
	
	//Curl options
	//Combines the api url and outputs it as a c string
	curl_easy_setopt(CurlAPI, CURLOPT_URL, (APIURL+"login").c_str());
	//Puts the data in the post fields to be sent
	curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, jsenddata.c_str());

	//Sends the request
	curl_easy_perform(CurlAPI);

	//Parses the response
	JP.Parse(APIResponse.c_str());

	//Extracts the data json string
	const Value& user = JP["data"];
	//Extracts the authToken from data
	AuthToken.append(user["authToken"].GetString());
	//Extracts the userID from data
	UserId.append(user["userId"].GetString());

	//Appends the userID and AuthToken to the header
	headers = curl_slist_append(headers, AuthToken.c_str());
	headers = curl_slist_append(headers, UserId.c_str());

	//Clears the options to prevent anything bad from happening ;)
	curl_easy_setopt(CurlAPI, CURLOPT_URL, NULL);
	curl_easy_setopt(CurlAPI, CURLOPT_POSTFIELDS, NULL);
       	LoggedIn = true;	
}


//Going to reimplement the message function to support the linked list.
int RCAPI::SendMessage(std::string Channel, rcmessage * Message){
	if(LoggedIn){
		if(Message->Text != "" | !(Message->Attachment)){
			return RCAPI_GOOD;
		}
		else{
			return RCAPI_NOMESSAGEORATTACHMENT;
		}
	}
	else{
		return RCAPI_NOTLOGGEDIN;
	}

}	


#endif
