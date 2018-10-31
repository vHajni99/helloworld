#include "std_lib_facilities.h"
int main()
{
vector<double>temps;
double temp;
while (cin>>temp) temps.push_back(temp);
double sum=0;
for (int i=0; i<temps.size(); ++i)
sum +=temps[i];
cout<< "mean temperature:"<<sum/temps.size()<<'\n';
sort(temps);
cout<<"median temperature:"<<temps[temps.size()/2]<<'\n';
}
