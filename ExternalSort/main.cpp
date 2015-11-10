#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <utility>
#include <functional>
#include <memory>
#include <cstdio>
#include <iostream>

using std::vector;
using std::string;
using std::priority_queue;

using std::shared_ptr;
using std::pair;

using std::ifstream;
using std::ofstream;

size_t partition(std::ifstream & in, size_t memory_size, string const & tmp_prefix) {
	vector<string> strings;
	size_t using_size = 0;
	size_t tmp_file_count = 0;

	while (in) {
		ofstream tmp_file(tmp_prefix + std::to_string(tmp_file_count++));

		while (using_size < memory_size) {
			string tmp = "";
			in >> tmp;

			if (!in.good()) break;

			strings.push_back(tmp);

			using_size += tmp.size();
		}

		std::sort(strings.begin(), strings.end());

		std::copy(strings.begin(), strings.end(), std::ostream_iterator<string>(tmp_file, "\n"));

		strings.clear();
		using_size = 0;
	}

	return tmp_file_count;
}

void merge(ofstream & out, size_t files_count, string const & tmp_prefix) {
	typedef shared_ptr<ifstream> ifstream_ptr;
	typedef pair<string, ifstream_ptr> str_file;

	priority_queue<str_file, vector<str_file>, std::greater<str_file>> queue;
	vector<ifstream_ptr> stream_pool(files_count);

	size_t counter = 0;
	std::generate(stream_pool.begin(), stream_pool.end(), [&counter, &tmp_prefix]() {
		return std::make_shared<ifstream>(tmp_prefix + std::to_string(counter++));
	});

	std::for_each(stream_pool.begin(), stream_pool.end(), [&queue](ifstream_ptr in) {
		string tmp = "";
		*in >> tmp;
		queue.push(std::make_pair(tmp, in));
	});

	while (!queue.empty()) {
		str_file tmp = queue.top();
		queue.pop();

		out << tmp.first << '\n';

		string new_string = "";
		*(tmp.second) >> new_string;
		if (tmp.second->good())
			queue.push(std::make_pair(new_string, tmp.second));
	}
}

void sort(ifstream & in, ofstream & out, size_t memory_size) {
	string tmp_prefix = "tmp";
	size_t tmp_files_count = partition(in, memory_size, tmp_prefix);
	merge(out, tmp_files_count, tmp_prefix);

	for (size_t i = 0; i != tmp_files_count; ++i)
		std::remove((tmp_prefix + std::to_string(i)).c_str());
}

int main(int argc, char * argv[]) {
	if (argc != 4) {
		std::cout << "Usage: ExternalSort <input_file> <output_file> <memory_size Mb>" << std::endl;
		return 1;
	}

	ifstream in(argv[1]);
	ofstream out(argv[2]);

	sort(in, out, std::atoi(argv[3]) * 1024 * 1024);

	return 0;
}