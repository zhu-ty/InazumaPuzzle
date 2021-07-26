#include<iostream>
#include<queue>
#include "SKCommon.hpp"
#include "INIReader.h"
#include "skversion.h"

struct PuzzuleConfig
{
	int mode = 0; // 0: side mode, 1: round mode
	int stat_max = 3;
	int count = 3;
	std::vector<int> stats;
	bool found = false;
	std::vector<string> ans;
} puconf;

std::queue<PuzzuleConfig> stat_queue;

bool checkStat(std::vector<int> status)
{
	auto t = status[0];
	for (int i = 0; i < status.size(); i++)
		if (t != status[i])
			return false;
	return true;
}

std::string genStatString(std::vector<int> status)
{
	std::string ret = "";
	for (int i = 0; i < status.size(); i++)
		ret = ret + SKCommon::format("%d ", status[i]);
	return ret;
}

PuzzuleConfig achiveAns()
{
	//puconf.depth += 1;
	//if (puconf.depth >= 100)
	//{
	//	SKCommon::errorOutput("Not found in depth > 100!");
	//	puconf.found = true;
	//	return;
	//}
	auto this_status = stat_queue.front();
	stat_queue.pop();


	if (checkStat(this_status.stats))
	{
		this_status.found = true;
		return this_status;
	}

	for (int i = 0; i < this_status.stats.size(); i++)
	{
		if (i == 0)
		{
			if (puconf.mode == 1)
				this_status.stats[this_status.stats.size() - 1] = (this_status.stats[this_status.stats.size() - 1] + 1) % puconf.stat_max;
			this_status.stats[i] = (this_status.stats[i] + 1) % puconf.stat_max;
			this_status.stats[i+1] = (this_status.stats[i+1] + 1) % puconf.stat_max;

			auto cp = this_status;
			cp.ans.push_back(SKCommon::format("Move at %d, %s", i, genStatString(this_status.stats).c_str()));
			stat_queue.push(cp);
			if (puconf.mode == 1)
				this_status.stats[this_status.stats.size() - 1] = (this_status.stats[this_status.stats.size() - 1] - 1 + puconf.stat_max) % puconf.stat_max;
			this_status.stats[i] = (this_status.stats[i] - 1 + puconf.stat_max) % puconf.stat_max;
			this_status.stats[i + 1] = (this_status.stats[i + 1] - 1 + puconf.stat_max) % puconf.stat_max;
		}
		else if (i == this_status.stats.size() - 1)
		{
			if (puconf.mode == 1)
				this_status.stats[0] = (this_status.stats[0] + 1) % puconf.stat_max;
			this_status.stats[i] = (this_status.stats[i] + 1) % puconf.stat_max;
			this_status.stats[i - 1] = (this_status.stats[i - 1] + 1) % puconf.stat_max;
			auto cp = this_status;
			cp.ans.push_back(SKCommon::format("Move at %d, %s", i, genStatString(this_status.stats).c_str()));
			stat_queue.push(cp);
			if (puconf.mode == 1)
				this_status.stats[0] = (this_status.stats[0] - 1 + puconf.stat_max) % puconf.stat_max;
			this_status.stats[i] = (this_status.stats[i] - 1 + puconf.stat_max) % puconf.stat_max;
			this_status.stats[i - 1] = (this_status.stats[i - 1] - 1 + puconf.stat_max) % puconf.stat_max;
		}
		else
		{
			this_status.stats[i] = (this_status.stats[i] + 1) % puconf.stat_max;
			this_status.stats[i - 1] = (this_status.stats[i - 1] + 1) % puconf.stat_max;
			this_status.stats[i + 1] = (this_status.stats[i + 1] + 1) % puconf.stat_max;
			auto cp = this_status;
			cp.ans.push_back(SKCommon::format("Move at %d, %s", i, genStatString(this_status.stats).c_str()));
			stat_queue.push(cp);
			this_status.stats[i] = (this_status.stats[i] - 1 + puconf.stat_max) % puconf.stat_max;
			this_status.stats[i - 1] = (this_status.stats[i - 1] - 1 + puconf.stat_max) % puconf.stat_max;
			this_status.stats[i + 1] = (this_status.stats[i + 1] - 1 + puconf.stat_max) % puconf.stat_max;
		}
	}
	return this_status;
}


int main(int argc, char* argv[])
{
	SKCommon::infoOutput("Version = %d.%d.%s", __SK_MAJOR_VERSION__, __SK_MINOR_VERSION__, __GIT_VERSION__);
	std::string ini_file = "config.ini";
	if (argc > 1)
		ini_file = atoi(argv[1]);
	INIReader ini(ini_file);
	puconf.mode = ini.GetInteger("GLOBAL", "Mode", 0);
	puconf.stat_max = ini.GetInteger("GLOBAL", "StatMax", 3);
	puconf.count = ini.GetInteger("GLOBAL", "Count", 3);
	auto conStr = ini.Get("GLOBAL", "Stat", "000");
	if (puconf.count != conStr.size() || puconf.count <= 2)
	{
		SKCommon::errorOutput("Count != Str length or count is too small");
		system("pause");
		return -1;
	}
	if (puconf.stat_max > 10)
	{
		SKCommon::errorOutput("Max stat_max = 10 is supported");
		system("pause");
		return -2;
	}

	puconf.stats.resize(puconf.count);
	for (int i = 0; i < puconf.count; i++)
	{
		puconf.stats[i] = conStr[i] - '0';
		if(puconf.stats[i] < 0 || puconf.stats[i] >= puconf.stat_max)
		{
			SKCommon::errorOutput("Error in string!");
			system("pause");
			return -3;
		}
 	}

	PuzzuleConfig anss;
	int times = 0;
	stat_queue.push(puconf);
	while (true)
	{
		times++;
		if (times > 100000)
		{
			SKCommon::errorOutput("No answer in 100000 steps");
			system("pause");
			return 0;
		}
		anss = achiveAns();
		if (anss.found == true)
			break;
	}
	printf("start:%s\n", conStr.c_str());
	for (int i = 0; i < anss.ans.size(); i++)
	{
		printf("%s\n", anss.ans[i].c_str());
	}
	system("pause");
	return 0;
}