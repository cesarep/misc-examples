/**
 * @file iostream_overload.cpp
 * @author César E Petersen (cesar.e.p@hotmail.com)
 * @brief example of progress bar implementation
 * @version 0.1
 * @date 2022-12-22
 * 
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <thread>

using namespace std;

struct ProgressBar {
	ProgressBar(unsigned size, char filler = '=')
		: size(size), filler(filler) {
		// hides cursor
		printf("\033[?25l");
		print();
	}

	~ProgressBar() {
		// show cursor
		printf("\033[?25h");
	}

	void progress(unsigned s = 1) {
		step += s;
		print();
	}

	void print() {
		printf("\r[%-50s] %d%% %d/%d", string(50 * step / size, filler).c_str(), 100 * step / size, step, size);
	}

	bool done() {
		return step >= size;
	}

	unsigned size, step = 0;
	const char filler;
};

struct MultiProgressBar {
	vector<ProgressBar*> bars;
	const int N;

	template <typename... ProgressBar>
	MultiProgressBar(ProgressBar&... bars): bars({ &bars... }), N(sizeof...(bars)) {
		for (int i = 0; i < N; ++i) putchar('\n');
	}

	template <size_t index>
	void progress(int s = 1) {
		bars[index]->step += s;
		print();
	}

	void print() {
		printf("\033[%dA", N);
		for (auto& bar : bars) {
			bar->print();
			putchar('\n');
		}
	}

	bool done() {
		bool b = true;
		for (ProgressBar *bar : bars) {
			b &= bar->done();
		}
		return b;
	}
};

int main() {
    // creates progress bars for 3 different jobs
	ProgressBar pb1(20), pb2(50), pb3(30);
    // unites the progressbar in a multiprogressbar object
	MultiProgressBar pbs(pb1, pb2, pb3);

	auto job1 = [&pbs]() {
		for (size_t i = 0; i < 20; ++i) {
			// tradicional way to progress the first bar (index 0)
            pbs.progress<0>();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	};

	auto job2 = [&pbs]() {
		for (size_t i = 0; i < 50; ++i) {
			// calling pb2.progress() can break the multiprogress lines,
            // it's best used only when not dealling with multiple progress bars
            pbs.progress<1>();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	};

	auto job3 = [&pb3]() {
		for (size_t i = 0; i < 30; ++i) {
            // directly incrementing the progress bar step is ideal for CPU intensive jobs
            // but print() is never called this way, so it needs to be handled separately
			pb3.step++;
			std::this_thread::sleep_for(std::chrono::milliseconds(60));
		}
	};

	std::thread t1(job1), t2(job2), t3(job3);

    // when directly incrementing the progress like job3 with pb3.step++
    // the printing function needs to be called separately like so:
	do {
		this_thread::sleep_for(std::chrono::milliseconds(50));
		pbs.print();
	} while (!pbs.done());

	t1.join();
	t2.join();
	t3.join();

	printf("Done\n");

	return 0;
}