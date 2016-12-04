// Advent of Code 2016 - Day 4 part 1
// Peter Westerström

#include "config.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>
//#include <cassert>

using namespace std;

vector<string> readRoomStrings(string inputFile)
{
	vector<string> rooms;
	fstream f(inputFile);

	int validCount{0};
	while (!f.eof())
	{
		string line;
		if (getline(f, line))
		{
			rooms.push_back(line);
		}
	}
	return rooms;
}

template <typename A, typename B> std::pair<B, A> flip_pair(const std::pair<A, B>& p)
{
	return std::pair<B, A>(p.second, p.first);
}

// flips an associative container of A,B pairs to B,A pairs
template <typename A, typename B, template <class, class, class...> class M, class... Args>
std::multimap<B, A> flip_map(const M<A, B, Args...>& src)
{
	std::multimap<B, A> dst;
	std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), flip_pair<A, B>);
	return dst;
}

// return -1 if not valid
int parseRoomID(string room)
{
	regex rgx("(([a-z]+[-])+)([0-9]+)\\[([a-z]+)\\]");
	std::smatch m;
	if (regex_match(room, m, rgx))
	{
		auto n = m.size();
		auto name = m[1].str();
		auto sectorID = m[3].str();
		auto checksum = m[4].str();

		name.erase(remove(name.begin(), name.end(), '-'), name.end());
		sort(name.begin(), name.end());
		unordered_map<char, int> histogram;
		for (char c : name)
		{
			if (histogram.find(c) == histogram.end())
			{
				histogram[c] = 1;
			} else
			{
				histogram[c]++;
			}
		}
		auto countToCharMap = flip_map(histogram);
		vector<pair<int, char>> countToCharVec;
		copy(countToCharMap.rbegin(), countToCharMap.rend(), back_inserter(countToCharVec));

		sort(countToCharVec.begin(), countToCharVec.end(),
		     [](const pair<int, char>& lhs, const pair<int, char>& rhs) {
			     if (lhs.first > rhs.first)
				     return true;
			     if (lhs.first < rhs.first)
				     return false;

			     return (lhs.second < rhs.second);
			 });

		string computedChecksum;
		for (int i = 0; i < 5 && i < countToCharVec.size(); i++)
		{
			computedChecksum += countToCharVec[i].second;
		}

		if (checksum == computedChecksum)
			return atoi(sectorID.c_str());
		else
			return -1;
	}
	return -1;
}

int main()
{
	// 	assert(parseRoomID("aaaaa-bbb-z-y-x-123[abxyz]")>0);
	// 	assert(parseRoomID("a-b-c-d-e-f-g-h-987[abcde]")>0);
	// 	assert(parseRoomID("not-a-real-room-404[oarel]")>0);
	// 	assert(parseRoomID("totally-real-room-200[decoy]")==-1);

	auto rooms = readRoomStrings(INPUT_FILE);
	int validRoomsIDSum{0};
	for (auto& room : rooms)
	{
		auto id = parseRoomID(room);
		if (id >= 0)
			validRoomsIDSum += id;
	}

	cout << "Day 4 part 1 result: " << validRoomsIDSum << endl;
	return 0;
}
