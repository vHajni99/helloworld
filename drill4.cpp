#include "std_lib_facilities.h"
int main(){

vector<double>v;
double sum=0.0;
double min;
double max=0.0;
string unit="none";
double x;          //a beolvasott ertekek

while(unit!="quit"){

 cout<<"Please enter a unit (m, cm, in or ft) for this value or  'quit' if you would like to terminate the input!\n";
 cin>>unit;

if (unit!="quit"&&unit!="m"&&unit!="cm"&&unit!="in"&&unit!="ft") {
	cout<<"This unit is illegal";
	cin>>unit; } //uj mertekegyseg beolvasasa

if (unit!="quit") {

cout<<"Please enter a floating-point value!\n";
 cin>>x;

 if (unit=="cm")
	x*=0.01;

 if (unit=="in")
	x*=0.0254;

 if (unit=="ft")
	x*=0.305;
 v.push_back(x);

 if (v.size()==1) min=v[0];

 sum+=v[v.size()-1];

 if (x>max) max=x;
 if (x<min) min=x;

 
 if (x==max) cout<<x<<" m is the largest value so far\n";
 if (x==min) cout<<x<<" m is the smallest value so far\n";
}
} //while loop vege

cout<<"The largest value is:"<<max<<"\n";
cout<<"The smallest value is:"<<min<<"\n";
cout<<"The sum of the values is:"<<sum<<"\n";
sort(v);
cout<<"The number of values is:"<<v.size()<<"\n";
cout<<"The values in increasing order are:";
for(double y:v) cout<<y<<"\t";
}
        
