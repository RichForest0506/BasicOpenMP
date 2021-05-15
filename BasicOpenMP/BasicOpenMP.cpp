#include <iostream>
#include <omp.h>

using namespace std;

void rnd()
{
	for (int i = 0; i < 100000; i++)
		rand();
}

int main()
{
	//omp_set_num_threads(2);

	cout << "1 threads" << endl;
	for (int i = 0; i < 10; i++)
		cout << omp_get_num_threads();

	cout << "\n2 threads\n";
#pragma omp parallel for 
	for (int i = 0; i < 10; i++)
		cout << omp_get_num_threads();

	cout << "\nUse section\n";
#pragma omp parallel
	{
#pragma omp sections nowait
		{
#pragma omp section
			{
				for (int i = 0; i < 10; i++) {
					cout << 1;
				}
			}
#pragma omp section
			{
				for (int i = 0; i < 30; i++) {
					cout << 2;
				}
			}
		}
#pragma omp barrier

#pragma omp sections
		{
#pragma omp section
			{
				for (int i = 0; i < 5; i++) {
					cout << 3;
				}
			}
#pragma omp section
			{
				for (int i = 0; i < 15; i++) {
					cout << 4;
				}
			}
		}
	}


	cout << "\nCritical\n";
#pragma omp parallel for
	for (int i = 0; i < 10; i++) {
#pragma omp critical
		cout << 1 << " ";
	}

#pragma omp parallel for
	for (int i = 0; i < 10; i++)
		cout << 2 << " ";

	cout << "\nMaster\n";

	int sum[10];

#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < 5; i++)
			sum[i] = i * i;

#pragma omp for
		for (int i = 5; i < 10; i++)
			sum[i] = i * i * i;


#pragma omp master
		{
			for (int i = 0; i < 10; i++)
				cout << sum[i] << " ";
			cout << endl;
		}

	}

	int counter = 0;

	cout << "Init Lock: ";

	omp_lock_t lock;
	int i, p_sum = 0, res = 0;

	omp_init_lock(&lock);
#pragma omp parallel firstprivate(p_sum)
	{
#pragma parallel for private(i)
		for (i = 0; i < 10; i++)
			p_sum += i;
		omp_set_lock(&lock);
		res += p_sum;
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
	cout << res;
}