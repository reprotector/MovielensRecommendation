#include<cstdio>
#include<ctime>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

#define N 943
#define M 1682
#define INF 1e9

//movies x users
int rate1[M][N];
double rate2[M][N];

void readBase(char *fileName)
{
  int user, rate, movie;
	string trash;
	
	//initialize all rates to 0
	for(int i=0; i<M; ++i)
		for(int j=0; j<N; ++j)
			rate1[i][j] = 0;
	
	//read base
	ifstream file;
	file.open(fileName);
	while(!file.eof())
	{
		file >> user; file >> movie; file >> rate; file >> trash;
		--user; --movie;
		rate1[movie][user] = rate;
	}
}

double sim2[M][M];
void knnItemItem(int k)
{
	double norm[M];
	vector< pair<double, int> > sim[M]; //.first == similarity value, .second == index of the item

	//initialize similarities to 0
	for(int i=0; i<M; ++i)
	{
		sim[i].assign(M, make_pair(0.0, 0));
		for(int j=0; j<M; ++j)
		{
			sim[i][j].first = sim2[i][j] = 0.0;
			sim[i][j].second = j;
		}
	}
	
	//calculate norm for all items
	for(int i=0; i<M; ++i)
	{
		norm[i] = 0;
		for(int j=0; j<N; ++j)
			norm[i] += pow(rate1[i][j], 2);
		norm[i] = sqrt(norm[i]);
	}
	
	//calculate cosine simlaritie for item-item
	for(int i=0; i<M; ++i)
	{
		for(int j=0; j<M; ++j)
			if(i != j)
			{
				double dotProduct = 0.0;
				for(int l=0; l<N; ++l) 
					dotProduct += rate1[i][l]*rate1[j][l];
				sim[i][j].first = -1*dotProduct/(norm[i]*norm[j]); //it's negative because the default function of sort is designed to produce the result in ascending order, but we need in descending order
				sim2[i][j] = sim[i][j].first;
			}
		//sort similarities of the item i to all others in descending order
		sort(sim[i].begin(), sim[i].end());
	}
	
	//predict missing rates
	for(int i=0; i<M; ++i)
	{
		for(int j=0; j<N; ++j)
		{
			rate2[i][j] = 0;
			if(rate1[i][j] == 0)
			{
				int count = 0;
				for(int l=0; count<k && l<M; ++l)
					if(rate1[sim[i][l].second][j] != 0)
					{
						rate2[i][j] += rate1[sim[i][l].second][j];
						++count;
					}
				if(count != 0)
					rate2[i][j] /= count;
				else
					rate2[i][j] = rand()%5 + 1; //if there isn't rates for this movie we guess one integer rate
			}	
			else
				rate2[i][j] = rate1[i][j];
		}
	}
}

double RMS(char *fileName, char *outputFileName = NULL)
{
	int user, rate, movie, n;
	double rms = 0.0;
	string trash;
	
	ofstream outputFile;
	if(outputFileName != NULL)
		outputFile.open(outputFileName);
	
	ifstream file;
	file.open(fileName);
	for(n=1; !file.eof(); ++n)
	{
		file >> user; file >> movie; file >> rate; file >> trash;
		--user; --movie;
		rms += pow(rate2[movie][user] - rate, 2);
		
		if(outputFileName != NULL)
			outputFile << user + 1 << " " << movie + 1 << " " << rate2[movie][user] << endl;
			
	}
	return sqrt(rms/n);
}

int main()
{
	char base[100] = "ua.base", testSet[100] = "ua.test", base2[100] = "ub.base", testSet2[100] = "ub.test";
	char outputRMS[100] = "rmsa.txt", outputRMS2[100] = "rmsb.txt";
	double bestRMS, rms;
	int bestK;
	
	//make tests for ua.base
	readBase(base);
	bestRMS = INF;
	for(int k=2; k<26; k+=2)
	{
		knnItemItem(k);
		rms = RMS(testSet);
		if(bestRMS > rms)
		{
			bestRMS = rms;
			bestK = k;
		}
		printf("base: %s, testSet= %s, k=%d, rms= %lf\n", base, testSet, k, rms);
	}
	//print on the output file the predicted rates
	knnItemItem(bestK);
	RMS(testSet, outputRMS);
	
	//make tests for ub.base
	readBase(base2);
	bestRMS = INF;
	for(int k=2; k<26; k+=2)
	{
		knnItemItem(k);
		rms = RMS(testSet2);
		if(bestRMS > rms)
		{
			bestRMS = rms;
			bestK = k;
		}
		printf("base: %s, testSet= %s, k=%d, rms= %lf\n", base2, testSet2, k, rms);
	}
	//print on the output file the predicted rates
	knnItemItem(bestK);
	RMS(testSet2, outputRMS2);
	
	return 0;
}