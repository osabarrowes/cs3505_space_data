#include<iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

void secondCall(string s)
{
  
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  
  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitch.tv/helix/streams?game_id=509670&first=100");

    struct curl_slist *list = NULL;
    int n = s.length();
    char char_array[n+1];
    strcpy(char_array, s.c_str());
    list = curl_slist_append(list, s.c_str());
    list = curl_slist_append(list, "Client-Id: hamz76h0z1qqlc11h2ma34xfzxvup4");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    curl_slist_free_all(list);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
	      curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);

    std::vector<std::string> myStrings;
    std::size_t found = readBuffer.find(",");
    while(found!= std::string::npos)
      {
	int firstFound = found;
	//std::cout << readBuffer.substr(found, 5)<< std::endl;
	found = readBuffer.find(",", found+1);
	if(found!=std::string::npos){
	  myStrings.push_back(readBuffer.substr(firstFound, found - firstFound));
	}
      }
    std::vector<int> myNewStrings;
    for(int i = 0; i < myStrings.size(); i++)
      {
	if(myStrings[i].substr(0, 16) == ",\"viewer_count\":")
	  {
	    std::stringstream geek(myStrings[i].substr(16, myStrings[i].length()-1));
	    int x = 0;
	    geek >> x;
	    myNewStrings.push_back(x);
	  }
	//std::cout << myStrings[i] << std::endl;
      }
    int a = 0;
    for(int i = 0; i < myNewStrings.size(); i++)
      {
	a += myNewStrings[i];
	//std::cout << myNewStrings[i] << std::endl;
      }
    std::cout<<a<<std::endl;

    ofstream myfile;
    std::time_t result = std::time(nullptr);
    std::string myLocation = to_string(result) + ".txt";
    //std::string myLocation = "DataStorage" + str2 + ".txt";
    myfile.open (myLocation);
    myfile << a << "\n";
    //myfile.flush();
    myfile.close();
    
    //std::cout << readBuffer << std::endl;
  }

  curl_global_cleanup();
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://id.twitch.tv/oauth2/token?client_id=hamz76h0z1qqlc11h2ma34xfzxvup4&client_secret=ojasas7ohbc7rlvkn94gh8pdvj5zpu&grant_type=client_credentials");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    //curl_easy_setopt(curl, CURLOPT_URL, "https://id.twitch.tv/oauth2/token?client_id=hamz76h0z1qqlc11h2ma34xfzxvup4&client_secret=ojasas7ohbc7rlvkn94gh8pdvj5zpu&grant_type=client_credentials");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
	      curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
    std::string string2 = readBuffer.substr(17,30);
    string2 = "Authorization: Bearer " + string2;
    //std::cout << string2 <<std::endl;
    
    
    //std::cout << readBuffer<<std::endl;
    secondCall(string2);
  }

  curl_global_cleanup();
  return 0;
}
