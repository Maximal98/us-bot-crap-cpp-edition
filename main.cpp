#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>

class ClientClass : public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message message) override {
		if (message.startsWith(";rh list"))
			CURL *curl;
			CURLcode res;

			curl_global_init(CURL_GLOBAL_DEFAULT);

			curl = curl_easy_init();
			std::string s;
			if(curl)
			{

				curl_easy_setopt(curl, CURLOPT_URL, "https://api.unitystation.org/serverlist");

				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
				// curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L); //remove this to disable verbose output


				/* Perform the request, res will get the return code */
				res = curl_easy_perform(curl);
				/* Check for errors */
				if(res != CURLE_OK)
				{
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
				}

				/* always cleanup */
				curl_easy_cleanup(curl);
			}

			nlohmann::json serverlist = nlohmann::json::parse(s);
			
			std::stringstream buffer;
			buffer << "-------------------------------------------\n";
			for( nlohmann::json server : serverlist["servers"] ) {
				buffer << "Server:\t\t" << server["ServerName"] << "\n";
				buffer << "Players:\t" << server["PlayerCount"] << "\n\n";
				buffer << "Map\t\t" << server["CurrentMap"] << "\n";
				buffer << "Fork:\t\t" << server["ForkName"] << "\n";
				buffer << "Game Mode:\t" << server["GameMode"] << "\n";
				buffer << "Time:\t\t" << server["IngameTime"] << "\n";
				buffer << "Linux-DL:\t" << server["LinuxDownload"] << "\n";
				buffer << "OSX-DL:\t\t" << server["OSXDownload"] << "\n";
				buffer << "Win-DL:\t\t" << server["WinDownload"] << "\n";
				buffer << "Address:\t" << server["ServerIP"] << ":" << server["ServerPort"] << std::endl;
				buffer << "-------------------------------------------\n";
			}
			std::string final = buffer.str();
			sendMessage(message.channelID, "```" + final + "```");
	}
};

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s) {
	size_t newLength = size*nmemb;
	try
	{
		s->append((char*)contents, newLength);
	}
	catch(std::bad_alloc &e)
	{
		//handle memory problem
		return 0;
	}
	return newLength;
}

int main() {

}