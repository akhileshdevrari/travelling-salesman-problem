#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <math.h>
#define INF 1.7E+308

static inline void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static inline double newtemperature(double temperature, int n)
{
	//fit some goddamn function here, taking care that temprature reaches to 0.00001 for different values of n
	//if temperature is less than absolute zero
	if(temperature < 0.000001)
		return 0;
	if(temperature > 200000)
		return temperature/50.0;
	if(temperature > n)
		return temperature/4.0;
	if(temperature > 10)
		return temperature/2.4;
	return temperature/1.4;
}

static inline int shouldMove(double deltaE, double temperature)
{
	double expTerm = pow(2.718281828459, deltaE/temperature);
	double probability = 1.0/(1.0 + expTerm);
	double randomNum = (double)rand() / (double)RAND_MAX;
	if(probability > randomNum)
		return 1;
	return 0;
}

int main()
{
	int n, i, j, numIterations; // n = no. of cities;
	//temperature = T for simulated annealing;	deltaE = difference between length of two paths
	//bestPathLength = length of bestPath;	
	double temperature, deltaE, bestPathLength = INF, currPathLength, neighbourPathLength;
	char distType[12]; //whether distance is euclidian or not, I don't see any use
	scanf("%s%d", distType, &n);
	double coordinates[n][2]; //Coordinates of points, I don't see any use of them either
	double dist[n][n]; //dist[i][j] means distance to travel to reach from i to j
	int path[n]; //Hamiltonian cycle, to reduce whose total length is out task
	int bestPath[n]; //Best path found so far

	//Input
	for(i=0; i<n; i++)
		scanf("%lf%lf", &coordinates[i][0], &coordinates[i][1]);
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			scanf("%lf", &dist[i][j]);

	//Initialize with a hamiltonian cycle
	//For simplicity take : [0, 1, 2, 3, ....... n-1]
	for(int i=0; i<n; i++)
		path[i] = i;

	//starting with highest temperature
	temperature = INF;
	//Set some value of cooling rate and absolute zero according to input

	//seeding for generating random numbers
	srand(time(0));

	//initialize currPathLength
	currPathLength = dist[path[n-1]][path[0]];
	for(i=0; i<n-1; i++)
		currPathLength += dist[path[i]][path[i+1]];

	//Here starts the show :)
	while(temperature)
	{
		//Arbitrary chosen value.
		numIterations = 10*n;
		while(numIterations--)
		{
			//Find length of current path. Could be optimized more by taking into account previous pathLength.
			// currPathLength = dist[path[n-1]][path[0]];
			// for(i=0; i<n-1; i++)
			// 	currPathLength += dist[path[i]][path[i+1]];
			//Doing above lines more efficiently by pre-calculating currPathLength
			if(currPathLength < bestPathLength)
			{
				//bestPath = path
				for(int i=0; i<n; i++)
					bestPath[i] = path[i];
				bestPathLength = currPathLength;
			}

			//swap any two random indices to generate any neighbour and move to it with certain probability
			while(1)
			{
				i = rand()%n;
				j = rand()%n;
				while(i == j)
					j = rand()%n;

				//Doing same thing as above commented lines in more efficient way
				deltaE = dist[path[j]][path[(i+1)%n]] + dist[path[(i-1+n)%n]][path[j]] + dist[path[i]][path[(j+1)%n]] + dist[path[(j-1+n)%n]][path[i]];
				deltaE -= ( dist[path[i]][path[(i+1)%n]] + dist[path[(i-1+n)%n]][path[i]] + dist[path[j]][path[(j+1)%n]] + dist[path[(j-1+n)%n]][path[j]] );
				


				//Temprary workaround, I'll fix this later
				if(abs(i-j)==1 || i==0 || i==n-1 || j==0 || j==n-1)
				{
					swap(&path[i], &path[j]);
					// find path length for neighbour node
					neighbourPathLength = dist[path[n-1]][path[0]];
					for(int k=0; k<n-1; k++)
						neighbourPathLength += dist[path[k]][path[k+1]];
					// deltaE for stochistic probability formula
					deltaE = neighbourPathLength - currPathLength;
					swap(&path[i], &path[j]);
				}


				//try to move to neighbour with certain probality. Try neighbours till you actually move to it
				if(shouldMove(deltaE, temperature))
				{
					swap(&path[i], &path[j]);
					currPathLength += deltaE;
					break;
				}
				// swap(&path[i], &path[j]);
			}
		}
		temperature = newtemperature(temperature, n);

		//Print best path
		printf("temperature = %lf,   bestPathLength = %lf\n", temperature, bestPathLength);
		printf("bestPath : \n");
		for(int i=0; i<n; i++)
			printf("%d, ", bestPath[i]);
		printf("\n\n");
	}

	return 0;
}