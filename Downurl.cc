#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <thread>         
#include <mutex>    
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <string>
#include <iterator>

#include <regex>
using namespace std;


struct hp{
	string host;
	string path;
	};
struct hpt{
	vector<hp> url_info;
	time_t downtime;
	};
size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream) {
    string data((const char*)buffer, (size_t) size * nmemb);
    *((stringstream*) stream) << data << endl;
    return size * nmemb;
}
class HTML_DOWN{
	private:
	void* curl;
	public:
	HTML_DOWN(){
		
	 }
	 ~HTML_DOWN(){
		
	 }
	string download(string url) {
		//cout<<"okay MAN DOWNLOAD starting 123\n";
		 curl = curl_easy_init();
	 std::stringstream out;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
     curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cout<<stderr<<"UNABLE TO ACCESS "<<url<<" "<<curl_easy_strerror(res)<<"\n";
    }
     curl_easy_cleanup(curl);
   // cout<<"okay MAN DOWNLOAD COMPLETE\n";
    return out.str();
	}
	
	void rm_getvar(string *s){

		size_t found1 = s->find("href=\"");
		if(found1!=std::string::npos)
			s->assign(s->substr(found1+6));

		int n = s->size();
		string st="";
		for(int i=0;i<n;i++){
			if(s->at(i) !=' ' && s->at(i) !='\n' && s->at(i) != '\t'){
				if(s->at(i) =='?')break;
				if(s->at(i) =='"')break;
				if(s->at(i) =='#')break;
				st.push_back(s->at(i));
			}
		}
		if(st.size() ==0)st="/";
		 s->assign(st);
			
	}
	void is_dir(string *s){
		
	}
	void url_res(string *s){
		size_t found = s->find("//");
		if(found!=std::string::npos){
			 s->assign(s->substr(found+2));
		}
	}
		
	hp host_path(string *s, string curr_url){
		hp temp;
		temp.host="";
		temp.host="";
		size_t path =std::string::npos;
		size_t found = s->find("//");
		if(found!=std::string::npos){
			s->assign(s->substr(found+2));
			int n = s->size(); 
			for(int i=0;i<n;++i){
				 if(s->at(i) =='/'){ path =i;break;}
				 temp.host.push_back(s->at(i));
			 }
			 if(path !=std::string::npos)
				temp.path = s->substr(path);	
			 else  temp.path ="/";
			 return temp;
		}
		else {
			url_res(&curr_url);
			int n = curr_url.size();
			size_t t2 =std::string::npos;
			for(int i=0;i<n;++i){
				if(curr_url.at(i) =='/'){ path =i;break;}
				temp.host.push_back(curr_url.at(i));
			}
			if(path!=std::string::npos){
				for(int i=n-1;i >=0;i--){
					if(curr_url.at(i) =='/'){t2 =i;break;}
				}	 
				if(t2!=std::string::npos){
					for(int i=path;i<=t2;++i){
						temp.path.push_back(curr_url.at(i));
					}
					
					if(s->at(0) == '/'){
						if(s->size()>1)
							temp.path.assign(s->substr(0));
					}
					else temp.path.append(s->substr(0));
					return temp;
				}
			}
			else{
				if(s->at(0) == '/')
				temp.path.append((*s));
				else temp.path.append("/").append((*s));
				return temp;
			}	
		}
		return temp;
	}
	vector<hp> get_links(string s,string url){
			vector<hp> temp;
			 std::regex word_regex("\\s*?href\\s*=\\s*\"[^\"]*\"");
			//std::regex word_regex("(\\S*(?)href\\S*=\\S*(\"([^\"]*\")|'[^']*'|([^'\">\\S]+)))");
			auto words_begin = 
			std::sregex_iterator(s.begin(), s.end(), word_regex);
				auto words_end = std::sregex_iterator();

			for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			    if(match_str.size()>1){
				rm_getvar(&match_str);
				hp test = host_path(&match_str,url);
				temp.push_back(test);
				}
			}
	s.clear();
    return temp;
	}
};

