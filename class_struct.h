#ifndef CLASS_STRUCT_H
#define CLASS_STRUCT_H
#include "header.h"

class HTML_DOWN
{
	private:
		void* curl;
	public:
		HTML_DOWN();
		~HTML_DOWN();
		string download(string url) ;
		void save_down(string url,string file);
		hp path_to_save(string url);
		void save(string url);
		void rm_getvar(string *s);
		void is_dir(string *s);
		void url_res(string *s);
		hp host_path(string *s, string curr_url);
		vector<hp> get_links(string s,string url);
};

class URLS
{
	public:
		map<string ,domain_ext>urls;
		std::mutex DOMAIN_WRITE_MTX;
		URLS();
		~URLS();
		void add_path(string path,domain_ext *itt );
		void add_url(string domain,string path,time_t temp_time);
		void get_path(domain_ext *ext,string host);
		void get_domain();
};
	
	

class Ctrler
{
	URLS *urls ;
	//mutex mtx;           // mutex for critical section
	public:
		HTML_DOWN htm;
		map<string,domain_ext>::iterator  itn;
		string s;
		Ctrler(URLS *url);
		Ctrler();
		void toask(string url);
		void dotask(map<string,domain_ext>::iterator  *itp);
		std::map<string,time_t>::iterator* _last(map<string,domain_ext>::iterator  *itp);
		
		void nexttask();
		void save_help(map<string,domain_ext>::iterator  *itp);
		void save();
};

#endif

