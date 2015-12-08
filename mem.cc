#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include "Downurl.cc"
using namespace std;

struct domain_ext{
	map<string,time_t> path;
	time_t lt;
	};
	
class URLS{
	public:
	 map<string ,domain_ext>urls;
	 std::mutex DOMAIN_WRITE_MTX;
	public:
	URLS(){;};
	~URLS(){;};
	void add_path(string path,domain_ext *itt ){
		std::map<string,time_t>::iterator its;
		its = ((itt)->path).find(path);
		if(its != (itt)->path.end()){
			//found
		}
		else{
			//(itt)->PATH_WRITE_MTX.lock();
			its = ((itt)->path).find(path);
			if(its != (itt)->path.end()){
			//found
			//(itt)->PATH_WRITE_MTX.unlock();
			}
			else{
			time_t tl = time(0);
			(itt)->path.insert(std::pair<string,time_t>(path,tl));
			(itt)->lt = time(0);
			//(itt)->PATH_WRITE_MTX.unlock();
			}
		}
	}
	void add_url(string domain,string path,time_t temp_time){ //to add url if not exists
		std::map<string,domain_ext>::iterator it;
		it = urls.find(domain);
		if (it != urls.end()){
			add_path(path,&(it->second));
		}
		else{    //if domain not found
			DOMAIN_WRITE_MTX.lock();
			it = urls.find(domain);
			if (it != urls.end()){
				DOMAIN_WRITE_MTX.unlock();   //domain found recent entry
				add_path(path,&(it->second));
			}
			else{
				map<string,time_t> t;
				t.insert(std::pair<string,time_t>(path,time(0)));
				domain_ext temp ;
				temp.path = t;
				temp.lt = temp_time;
				urls.insert(std::make_pair(domain,temp));
				DOMAIN_WRITE_MTX.unlock();
			}
		}
	}
	void get_path(domain_ext *ext,string host){
		
		for (std::map<string,time_t>::iterator it=ext->path.begin(); it!=ext->path.end(); ++it)
		std::cout << "http://"<<host<<it->first << "   => " << it->second << '\n';
	}
	void get_domain(){
		for (std::map<string,domain_ext>::iterator it=urls.begin(); it!=urls.end(); ++it){
			//std::cout << it->first << " => ";
			get_path(&(it->second),it->first);
		}
	}
	
	
};
	
	

class Ctrler{
	URLS *urls ;
	
	//mutex mtx;           // mutex for critical section
	public:
	HTML_DOWN htm;
	map<string,domain_ext>::iterator  itn;
	string s;
	Ctrler(URLS *url){
		urls = url;
	}
	Ctrler(){};
	void toask(string url){
		
		vector<hp> hp1 = htm.get_links(htm.download(url),url);
		
		 int size = hp1.size();
		 for(int i=0;i<size;++i){
			// cout<<hp1[i].host <<"   ~~~~~"<<hp1[i].path<<endl;
			 urls->add_url(hp1.at(i).host,hp1.at(i).path,0);
		 }
		 hp1.clear();
	 }
	void dotask(map<string,domain_ext>::iterator  *itp){
		
		(*itp)->second.lt =time(0);
		time_t max =time(0);
		
		std::map<string,time_t>::iterator its= (*itp)->second.path.begin();
		for (std::map<string,time_t>::iterator it=(*itp)->second.path.begin(); it!=(*itp)->second.path.end(); ++it)
		     if((it)->second < max)its = it;
	
		string temp_url = (*itp)->first;
		temp_url.append((its)->first);
		s =htm.download(temp_url);
		 vector<hp> hp1 = htm.get_links(s,temp_url);
		 s="";
		 int size = hp1.size();
		 for(int i=0;i<size;++i){
			 urls->add_url(hp1.at(i).host,hp1.at(i).path,0);
		 }
		 hp1.clear();
		 (*itp)->second.lt =time(0);
	}
	std::map<string,time_t>::iterator* _last(map<string,domain_ext>::iterator  *itp){
		
		domain_ext *ext = &(itn)->second;
	
		time_t max =time(0);
		
		std::map<string,time_t>::iterator *its;
		
		for (std::map<string,time_t>::iterator it=(ext)->path.begin(); it!=(ext)->path.end(); ++it)
		     if((it)->second < max)its = &it;
		     return its;
		}
		
	void nexttask(){
		int t=1;
		for(itn =urls->urls.begin() ;(itn) !=urls->urls.end();itn++){
			if(difftime(time(0),(itn)->second.lt) > 10000){	
				//cin>>t;
				if(t==1)
			dotask(&itn);
			else if(t==5)break;
			}
		}
	}
};

int main(){
	URLS urls;
	Ctrler ctr =Ctrler(&urls);
	string url;
	int t;
	cin>>url;
	while(cin>>t&& t !=-1){
		if(t==1)ctr.toask(url);
		else if(t==2)ctr.nexttask();
		else if(t==3)urls.get_domain();
		cout<<"OH WELL DONE\n";
	
}
}
