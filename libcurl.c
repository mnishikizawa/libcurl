#include <stdio.h>
#include <curl/curl.h>
#include <time.h>

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
   return size * nmemb;
}

int main(void)
{
  static const char metric_name[] = "api.response_time";
  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();
  if(curl) {
    double total;
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1/report/v1/affiliate/transaction/");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    if (CURLE_OK == res) {
       res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total);
    if (CURLE_OK == res) {
       fprintf(stdout, "%s\t%.4f\t%d\n", metric_name, total, time(NULL));
      }
    }

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}
