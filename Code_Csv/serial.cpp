#include <bits/stdc++.h>
#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<cstdint>
#define min(x, y) (((x) < (y)) ? (x) : (y)) 
//  Using the MONOTONIC clock 
#define CLK CLOCK_MONOTONIC

#define f(i,n) for(long long int i=0;i<n;i++)
#define fi(i,j,n) for(long long int i=j;i<=n;i++)
#define fr(i,n) for(long long int i=n-1;i>=0;i--)
#define pb push_back
#define mp make_pair
#define ll long long int
#define lld long double
#define vll vector<long long int>
#define e(v,i) erase(v.begin()+i)
#define MAX 200005
#define INF LLONG_MAX
#define ff first
#define ss second
//cout << fixed << setprecision(x) << n <<endl;
 
using namespace std;
 
struct timespec diff(struct timespec start, struct timespec end){
	struct timespec temp;
	if((end.tv_nsec-start.tv_nsec)<0){
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else{
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
 
int main() {

    struct timespec start_t, end_t , avg_time,start_t_p,end_t_p,avg_time_p;
    bool flag = false;     // to view adjacency list
    
    
    ofstream fout;
    fout.open("results.csv");

    fout << "Graph_Name,Vertices,Edges,Edges_in_TC,Run_time,num_procs\n";
    vector<string> file_name;
    file_name.pb("Tina_AskCal.csv");
    //file_name.pb("GD02_a.csv");
    //file_name.pb("ibm32.csv");
    //file_name.pb("GD95_c.csv");
    //file_name.pb("GD02_b.csv");
    //file_name.pb("GD97_a.csv");
    //file_name.pb("GD98_b.csv");
    //file_name.pb("GD96_d.csv");
    

for(int itr=0;itr<file_name.size();itr++)
{
    int64_t tot_time = 0;

    vector<int> x,y; //edges from x to y
    int tempx,tempy,n=0,cntb=0,cnta=0;
    fstream fin;
    fin.open(file_name[itr],ios::in);
    vector<string> row;
    string line,temp;
    int temptempdata;
    while(fin >> temp) // loading data from csv file
    {
        row.clear();
        line = temp;
        stringstream s(line);
        string word;
        vector<int> temp_data;
        int tempester;
        while(getline(s, word, ','))
        {
            row.push_back(word);
        }
        tempx = stoi(row[0]);
        tempy = stoi(row[1]);
        x.pb(tempx);
        y.pb(tempy);
    }

   n = x.size();
   
   //loading edges in hash table
    unordered_map<int, set<int>> umap,umap2;
    for(int i=0;i<n;i++)
    {
        umap[x[i]].insert(y[i]);
    }

    unordered_map<int, set<int>>:: iterator p;
    int cnt = 0,size_set=0;

    //Counting Edges in Input graph/list
    for(p = umap.begin();p!=umap.end();p++)
    {
        size_set = p->second.size();
        cntb += size_set;
        cnt++;
    }

    // Print initial adjacency list
    if(flag)
    {
    cout << "----------Initial Adjacency list----------\n";
    for(p = umap.begin();p!=umap.end();p++)
    {
        cout << p->first << " -> ";
        set<int>::iterator itr;
        for(itr = p->second.begin();itr != p->second.end();itr++)
        {
            cout << *itr << ' ';
        }
        cout << endl;
    }
    }
    
    //cout << "\nMAX loop iterations = " << cnt << endl;
int tot_runs = 10;

//serial implementation
for(int run_id=1;run_id<=tot_runs;run_id++)
{
    for(int i=0;i<n;i++)
    {
        umap2[x[i]].insert(y[i]);
    }

    clock_gettime(CLK, &start_t);
   for(int k=0;k<cnt;k++)           //no.of vertices
   {
       //parallelize from here
       for(p = umap2.begin();p!=umap2.end();p++)        //no.of vertices
       {
           set<int>::iterator itr;
            for(itr = p->second.begin();itr != p->second.end();itr++)    //length of adjacency list -> no.of vertices
            {
                set<int>::iterator itr2;
                for(itr2 = umap[*itr].begin();itr2!=umap[*itr].end();itr2++)    //length of adjacency list -> no.of vertices
                {
                    p->second.insert(*itr2);
                }
            }
       }
   }
    clock_gettime(CLK, &end_t);


    if(run_id!=tot_runs)
    umap2.clear();


    end_t = diff(start_t,end_t);

    tot_time += (end_t.tv_sec*pow(10,9)) + end_t.tv_nsec;       //time in neno sec

}

 //Counting Edges in Transitive Closure
        for(p = umap2.begin();p!=umap2.end();p++)
        {
            set<int>::iterator itr;
            for(itr = p->second.begin();itr != p->second.end();itr++)
            {
                cnta++;
            }
        }

        
        cout << "\n\nFile : " << file_name[itr] << endl;
        cout << fixed << setprecision(12) << "Average Run-time = " << 
        (tot_time*pow(10,-9))/tot_runs  << " second"<< endl;

        cout << "Number of edges of Input graph : " << cntb << endl << 
        "Number of edges of Transitive closure : " << cnta << endl;

        fout << fixed << setprecision(12) << file_name[itr] << ',' << 0 << ',' <<
        cntb << ',' << cnta <<','<< (tot_time*pow(10,-9))/tot_runs <<','<< 1 << "\n";


//parallel implementation
vector<int> procs;
procs.pb(2);
procs.pb(3);
procs.pb(4);

int64_t tot_para = 0;



for(int proc_id=0;proc_id<procs.size();proc_id++)
{
    tot_time = 0;
    omp_set_num_threads(procs[proc_id]);
    for(int run_id=1;run_id<=tot_runs;run_id++)
    {  
        
        umap2.clear();
        for(int i=0;i<n;i++)
        {
            umap2[x[i]].insert(y[i]);
        }
        /*MAIN KAAM KAAJ*/
        clock_gettime(CLK, &start_t);

        #pragma omp parallel
        {

        for(int k=0;k<cnt;k++)           //no.of vertices
        {
            //parallelize from here

            #pragma omp for
            for(unordered_map<int, set<int>>:: iterator p0 = umap2.begin();p0!=umap2.end();p0++)        //no.of vertices
            {
                set<int>::iterator itr;
                for(itr = p0->second.begin();itr != p0->second.end();itr++)    //length of adjacency list -> no.of vertices
                {
                    set<int>::iterator itr2;
                    for(itr2 = umap[*itr].begin();itr2!=umap[*itr].end();itr2++)    //length of adjacency list -> no.of vertices
                    {
                        p0->second.insert(*itr2);
                    }
                }
            }
        }

        }
        clock_gettime(CLK, &end_t);

        end_t = diff(start_t,end_t);

        tot_time += (end_t.tv_sec*pow(10,9)) + end_t.tv_nsec;       //time in neno sec

    }

    //Counting Edges in Transitive Closure
    cnta = 0;
        for(p = umap2.begin();p!=umap2.end();p++)
        {
            set<int>::iterator itr;
            for(itr = p->second.begin();itr != p->second.end();itr++)
            {
                cnta++;
            }
        }

        
        cout << "\n\nFile : " << file_name[itr] << endl;
        cout << fixed << setprecision(12) << "Average Paralle Run-time for P = "<<procs[proc_id]<<" = " << 
        (tot_time*pow(10,-9))/tot_runs  << " second"<< endl;


        fout << fixed << setprecision(12) << file_name[itr] << ',' << 0 << ',' <<
        cntb << ',' << cnta <<','<< (tot_time*pow(10,-9))/tot_runs <<','<< procs[proc_id] << "\n";

    

}






        //Printing Final Adjacency list of Transitive Closure
        if(flag)
        {
        cout << " \n\n\n----------Final Adjacency list----------\n";
        for(p = umap2.begin();p!=umap2.end();p++)
        {
            cout << p->first << " -> ";
            set<int>::iterator itr;
            for(itr = p->second.begin();itr != p->second.end();itr++)
            {
                cout << *itr << ' ';
            }
            cout << endl;
        }
        }
        
       
}
    fout.close();
	return 0;
}