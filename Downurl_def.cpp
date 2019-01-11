#ifndef DOWNURL_CPP
#define DOWNURL_CPP
#include "header.h"
#include "class_struct.h"
#include "FUNCTIONS.h"


HTML_DOWN::HTML_DOWN(){}
		
HTML_DOWN:: ~HTML_DOWN(){}

string HTML_DOWN::download(string url)
{
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
	if (res != CURLE_OK)
	{
	   cout<<stderr<<"UNABLE TO ACCESS "<<url<<" "<<curl_easy_strerror(res)<<"\n";
	}
	curl_easy_cleanup(curl);
	return out.str();
}
	
void HTML_DOWN::save_down(string url,string file)
{
	cout<<file<<"     ____FILE____\n";
	FILE* fp = fopen(file.c_str(), "wb");
	if (!fp)
	{
	    printf("!!! Failed to create file \n");
		exit(-1);
	}
	cout<<"WRITING DATA TO FILE\n";
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_file);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
	    cout<<stderr<<"UNABLE TO ACCESS "<<url<<" "<<curl_easy_strerror(res)<<"\n";
	}
	curl_easy_cleanup(curl);
	fclose(fp);
}
	
hp HTML_DOWN::path_to_save(string url)
{
	hp temp;
	size_t found = url.find("//");
	size_t found2 = url.find_last_of("/");	
	if(found2!=std::string::npos){
	if(found!=std::string::npos) temp.host =  url.substr(found+2,found2);
	temp.host =  url.substr(0,found2);
	temp.path =  url.substr(found2+1);
	//else temp.path = "NOT_FOUND_PATH_NOFILE";
	found = temp.path.find_last_of(".");
	if(found ==std::string::npos){
		temp.host.append("/").append(temp.path);
		 temp.path = ("/_index.html");
	}
	}
	else{
		temp.host =  url;
		temp.path = "NO_PATH.html";
	} 
	return temp;
}

void HTML_DOWN::save(string url)
{
	ofstream myfile;
	string tp = "mkdir -p ";
	hp t = path_to_save(url);
	if(t.path.size() >2)
	{
		tp.append(t.host);
		system(tp.c_str());
		save_down(url,t.host.append("/").append(t.path));
	}
}
	
void HTML_DOWN::rm_getvar(string *s)
{
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

void HTML_DOWN::is_dir(string *s)
{
		
}
void HTML_DOWN::url_res(string *s)
{
	size_t found = s->find("//");
	if(found!=std::string::npos)
	{
		s->assign(s->substr(found+2));
	}
}
		
hp HTML_DOWN::host_path(string *s, string curr_url)
{
	hp temp;
	temp.host="";
	temp.host="";
	size_t path =std::string::npos;
	size_t found = s->find("//");
	if(found!=std::string::npos)
	{
		s->assign(s->substr(found+2));
		int n = s->size();
		for(int i=0;i<n;++i)
		{
			if(s->at(i) =='/'){ path =i;break;}
			temp.host.push_back(s->at(i));
		}
		if(path !=std::string::npos)
			temp.path = s->substr(path);
		else  temp.path ="/";
		return temp;
	}
	else
	{
		url_res(&curr_url);
		int n = curr_url.size();
		size_t t2 =std::string::npos;
		for(int i=0;i<n;++i)
		{
			if(curr_url.at(i) =='/'){ path =i;break;}
			temp.host.push_back(curr_url.at(i));
		}
		if(path!=std::string::npos)
		{
			for(int i=n-1;i >=0;i--)
			{
				if(curr_url.at(i) =='/'){t2 =i;break;}
			}
			if(t2!=std::string::npos)
			{
				for(int i=path;i<=t2;++i)
				{
					temp.path.push_back(curr_url.at(i));
				}
				if(s->at(0) == '/')
				{
					if(s->size()>1)
						temp.path.assign(s->substr(0));
				}
				else temp.path.append(s->substr(0));
				return temp;
			}
		}
		else
		{
			if(s->at(0) == '/')
			temp.path.append((*s));
			else temp.path.append("/").append((*s));
			return temp;
		}
	}
	return temp;
}
vector<hp> HTML_DOWN::get_links(string s,string url)
{
	vector<hp> temp;
	std::regex word_regex("\\s*?href\\s*=\\s*\"[^\"]*\"");
	auto words_begin =	std::sregex_iterator(s.begin(), s.end(), word_regex);
	auto words_end = std::sregex_iterator();
	for (std::sregex_iterator i = words_begin; i != words_end; ++i)
	{
		std::smatch match = *i;
		std::string match_str = match.str();
		if(match_str.size()>1)
		{
			rm_getvar(&match_str);
			hp test = host_path(&match_str,url);
			temp.push_back(test);
		}
	}
	s.clear();
    return temp;
}






URLS::URLS(){;}
URLS::~URLS(){;};
void URLS::add_path(string path,domain_ext *itt )
{
	std::map<string,time_t>::iterator its;
	its = ((itt)->path).find(path);
	if(its != (itt)->path.end())
	{
		//found
	}
	else{
		//(itt)->PATH_WRITE_MTX.lock();
		its = ((itt)->path).find(path);
		if(its != (itt)->path.end())
		{
			//found
			//(itt)->PATH_WRITE_MTX.unlock();
		}
		else
		{
			time_t tl = time(0);
			(itt)->path.insert(std::pair<string,time_t>(path,tl));
			(itt)->lt = time(0);
			//(itt)->PATH_WRITE_MTX.unlock();
		}
	}
}

void URLS::add_url(string domain,string path,time_t temp_time)
{ 	//to add url if not exists
	std::map<string,domain_ext>::iterator it;
	it = urls.find(domain);
	if (it != urls.end())
	{
		add_path(path,&(it->second));
	}
	else
	{    //if domain not found
		DOMAIN_WRITE_MTX.lock();
		it = urls.find(domain);
		if (it != urls.end())
		{
			DOMAIN_WRITE_MTX.unlock();   //domain found recent entry
			add_path(path,&(it->second));
		}
		else
		{
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

void URLS::get_path(domain_ext *ext,string host)
{
	for (std::map<string,time_t>::iterator it=ext->path.begin(); it!=ext->path.end(); ++it)
	std::cout << "http://"<<host<<it->first << "   => " << it->second << '\n';
}

void URLS::get_domain()
{
	for (std::map<string,domain_ext>::iterator it=urls.begin(); it!=urls.end(); ++it)
	{
		//std::cout << it->first << " => ";
		get_path(&(it->second),it->first);
	}
}
	
Ctrler::Ctrler(URLS *url)
{
	urls = url;
}

Ctrler::Ctrler(){};

void Ctrler::toask(string url)
{
	vector<hp> hp1 = htm.get_links(htm.download(url),url);
		
	int size = hp1.size();
	for(int i=0;i<size;++i)
	{
		urls->add_url(hp1.at(i).host,hp1.at(i).path,0);
	}
	hp1.clear();
}

void Ctrler::dotask(map<string,domain_ext>::iterator  *itp)
{
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
	for(int i=0;i<size;++i)
	{
		urls->add_url(hp1.at(i).host,hp1.at(i).path,0);
	}
	hp1.clear();
	(*itp)->second.lt =time(0);
}

std::map<string,time_t>::iterator* Ctrler::_last(map<string,domain_ext>::iterator  *itp)
{
	domain_ext *ext = &(itn)->second;
	time_t max =time(0);
	std::map<string,time_t>::iterator *its;
	for (std::map<string,time_t>::iterator it=(ext)->path.begin(); it!=(ext)->path.end(); ++it)
		if((it)->second < max)its = &it;
		return its;
}
		
void Ctrler::nexttask()
{
	int t=1;
	for(itn =urls->urls.begin() ;(itn) !=urls->urls.end();itn++)
	{
		if(difftime(time(0),(itn)->second.lt) > 10000)
		{
			//cin>>t;
			if(t==1)
				dotask(&itn);
			else if(t==5)break;
		}
	}
}
	
void Ctrler::save_help(map<string,domain_ext>::iterator  *itp)
{
	(*itp)->second.lt =time(0);
	time_t max =time(0);
	std::map<string,time_t>::iterator its= (*itp)->second.path.begin();
	for (std::map<string,time_t>::iterator it=(*itp)->second.path.begin(); it!=(*itp)->second.path.end(); ++it){
		if((it)->second < max)its = it;
	string temp_url = (*itp)->first;
	temp_url.append((its)->first);
	htm.save(temp_url);
	}
}

void Ctrler::save()
{
	for(itn =urls->urls.begin() ;(itn) !=urls->urls.end();itn++)
	{
		//if(difftime(time(0),(itn)->second.lt) > 10){
		//cin>>t;
		save_help(&itn);

		//}
	}
}



#endif

