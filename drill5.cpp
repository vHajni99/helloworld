#include "std_lib_facilities.h"
int main()
try{
	//1,2,3,4
	cout<<"Success!\n"; //hibas valtozatok:
	//Cout<<"Success!\n";
	//cout<<"Success!\n;
	//cout<<"Success"<<!\n"
	//cout<<success<<'\n';
	
	//5
	int res=7; //hibasan string res=7;
	vector<int>v(10);
	v[5]=res;
	cout<<"5.Success!\n";

	//6
	vector<int>v6(10);
	v6[5]=7; //hibasan v6(5)=7;
	if (v6[5]==7) cout<<"6.Success!\n"; //hibasan v6[5]!=7

	//8
	bool c=false;
	if (!c) cout<<"8.Success!\n"; //hibasan if (c)
	else cout<<"8.Fail!\n";

	//9
	string s="ape";
	bool c9="fool"<s;
	if (!c9) cout<<"9.Success!\n"; //hibasan if (c9)

	//10,11,12
	string s10="ape";
	if (s10!="fool") cout<<"Success!\n";
	//hibasan: if (s10=="fool")
	//cout<"Success!\n";
	//if (s10+"fool")

	//13
	vector<char>v13(5);
	for (int i=0; i<v13.size();++i) cout<<"13.Success!\n";
	//hibasan 0<v13.size()

	//14
	vector<char>v14(5);
	for (int i=0; i<5;++i) cout<<"14.Success!\n"; //hibasan i<6
	
	//15,18
	string s15="15.Success!\n";
	for (int i=0; i<6; ++i ) cout<<s15; //hibasan cout s[i]; s nem vector

	//16
	if (true) cout<<"16.Success!\n"; //hibasan if (true) then
	// nem szukseges: else cout<<"16.Fail!\n";

	//17
	int x17=2000;
	int c17=x17; //hibasan char
	if (c==2000) cout<<"Success!\n";


	//19
	vector<int>v19(5); //hiba:kell egy type a vektornak
	for (int i=0; i<v19.size(); ++i) cout<<"19.Success!\n";
	//hiba: a for utani ; ures utasitas

	//20
	int i20=0;
	int j20=9;
	while (i20<10) ++i20; //hiba: ++j
	if (j20=i20) cout<<"20.Success!\n"; //hiba: j<i

	//21
	int x21=2;
	double d21=5.0/x21; //hiba: x-2 nullaval nem osztunk es 5 helyett 5.0 kell
	if (d21==2.5) cout<<"21.Success!\n";

	//22. hibasan:
	//string<char> s="Success!\n";
	//for (int i=0;i=10;++i) cout<< s[i];
	vector <string> s22(11);
	for (int i=0;i<=10;++i) {
	s22[i]="22.Success!\n";	
	cout<<s22[i];
	}

	//23 hibasan:
	//int i=0;
	//while (i<10) ++j;
	//if (j<i) cout<<"Success!\n";
	int i23=0; int j23=9;
	while (i23<10) ++i23;
	if (j23<i23) cout<<"23.Success!\n";

	//24 hibasan: int x=4; double d=5/(x-2);
	// if (d=2*x+0.5) cout<<"Success!\n";
	int x24=4; double d24=5.0/(x24-2);
	if (d24==2.5) cout<<"24.Success!\n";

	//25 hibasan: cin<<"Success\n";
	cout<<"25.Success!\n";

	return 0;
}
catch (exception& e) {
	cerr<<"error: "<<e.what()<<'\n';
	return 1;	
}
catch (...) {
	cerr<<"Oops: unknown exception!"<<'\n';
	return 2;
}
