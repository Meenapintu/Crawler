#ifndef MAIN_CPP
#define MAIN_CPP
#include "class_struct.h"

int main(int argc ,char** argv){
	URLS urls;
	Ctrler ctr =Ctrler(&urls);
	string url;
	int t;
	cout<<"first input url, then 1 for list . then ....\n";
	cout<<" 2 for next level lookup, 3 for see all links in dbms, 4 for download and save file\n \n";
	cin>>url;
	while(cin>>t&& t !=-1)
	{
		if(t==1)ctr.toask(url);
		else if(t==2)ctr.nexttask();
		else if(t==3){urls.get_domain();}
		else if(t==4) ctr.save();
		cout<<"OH WELL DONE\n";
	}
}

#endif
