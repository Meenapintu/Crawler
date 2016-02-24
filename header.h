#ifndef header_H
#define header_H
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <cmath>
#include <cstdio>
#include <vector>   
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <string>
#include <iterator>
#include <fstream>
#include <regex>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

struct hp
{
	string host;
	string path;
};
struct hpt
{
	vector<hp> url_info;
	time_t downtime;
};
struct domain_ext
{
	map<string,time_t> path;
	time_t lt;
};
	
#endif
