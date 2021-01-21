#include <iostream>
#include <memory>
#include <functional>
#include <array>

#include <curl/curl.h>
#include "Json.h"

typedef std::unique_ptr<CURL, std::function<void(CURL*)>> CURL_ptr;

class CurlHandle
{
public:
	CurlHandle() : curlptr(curl_easy_init(), &curl_easy_cleanup)
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	void SetURL(std::string url)
	{
		curl_easy_setopt(curlptr.get(), CURLOPT_URL, url.c_str());
	}

	CURLcode fetch()
	{
		return curl_easy_perform(curlptr.get());
	}

private:
	CURL_ptr curlptr;
	
	constexpr static auto deleter = [](CURL* c) {
		curl_easy_cleanup(c);
		curl_global_cleanup();
	};
};

class Bitcoin
{
public:
	Bitcoin() : curlHandle({})
	{
		curlHandle.SetURL(API_URL);
	}

	void FetchBitcoinData()
	{
		curlHandle.fetch();
	}

private:
	CurlHandle curlHandle;
	static constexpr const char* API_URL = "https://blockchain.info/ticker";
};

int main()
{
	Bitcoin bitcoin;
	bitcoin.FetchBitcoinData();

	return 0;
}
