#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <set>
#include <ctime>
#include <regex>
#include <urlmon.h> 
#pragma comment( lib, "urlmon" ) 
#pragma warning(disable : 4996)



bool download_file(std::string url, std::string path)
{
	return URLDownloadToFileA(nullptr, url.c_str(), path.c_str(), 0, nullptr) == S_OK;
}

std::string file_to_string(std::string file_name)
{
	std::ifstream file(file_name);
	return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{} };
}

std::set<std::string> extract_links(std::string html_file_name)
{
	static const std::regex hl_regex("<a href=\"(.*?)\">", std::regex_constants::icase);

	const std::string text = file_to_string(html_file_name);

	return { std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
			 std::sregex_token_iterator{} };
}


std::set<std::string> extract_title(std::string html_file_name)
{
	static const std::regex hl_regex("<title>(.*?)</title>", std::regex_constants::icase);

	const std::string text = file_to_string(html_file_name);

	return { std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
			 std::sregex_token_iterator{} };
}

std::set<std::string> extract_para(std::string html_file_name)
{
	static const std::regex hl_regex("<p>(.*?)</p>", std::regex_constants::icase);

	const std::string text = file_to_string(html_file_name);

	return { std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
			 std::sregex_token_iterator{} };
}




int main()
{
	time_t t = time(NULL);
	tm* p = localtime(&t);
	
	char day[100];
	char dayb[100];


	sprintf(day, "%04d/%02d/%02d", p->tm_year + 1900, p->tm_mon+1, p->tm_mday);
	sprintf(dayb, "%04d/%02d/%02d", p->tm_year + 1900, p->tm_mon+1, p->tm_mday-1);

	const std::string cdate(day);
	const std::string dbdate(dayb);
	

	


	const std::string url = "https://npr.org/sections/arts/";
	const std::string path = "npr.txt";
	const std::string cdurl = "https://www.npr.org/" + cdate;
	const std::string pdurl = "https://www.npr.org/" + dbdate;

	std::cout << cdurl << "\n" << pdurl << "\n";
	int i = 0;
	

	
	if (download_file(url, path))
		for (std::string hlink : extract_links(path)) {
			if(i < 5) {
				if (hlink.find(cdurl) != std::string::npos) {
					const std::string stpath = "storyhtml" + std::to_string(i+1) + ".txt";
					const std::string storyf = "story" + std::to_string(i + 1) + ".txt";
					std::cout << hlink << "\n";
					std::ofstream outfile(storyf);
					i++;

					if (download_file(hlink, stpath)) {
						for (std::string title : extract_title(stpath)) {
							outfile << title << std::endl;
						}
						for (std::string paras : extract_para(stpath)) {
							std::string cop = paras;
							size_t n = std::count(cop.begin(), cop.end(), '<');
							for (int i = 0; i < n; i++) {
								if (cop.find("<") != std::string::npos) {
									std::size_t beg = cop.find("<");
									std::size_t end = cop.find(">");
									cop.erase(beg, end+1);
								}
							}
							outfile << cop << std::endl;
							
						}
						

					}
				}
				else if(hlink.find(pdurl) != std::string::npos){
					const std::string stpath = "storyhtml" + std::to_string(i+1) + ".txt";
					const std::string storyf = "story" + std::to_string(i + 1) + ".txt";
					std::cout << hlink <<"\n";
					std::ofstream outfile(storyf);
					i++;
					if (download_file(hlink, stpath)) {
						for (std::string title : extract_title(stpath)) {

							outfile << title << std::endl;
						}
						for (std::string paras : extract_para(stpath)) {
							std::string cop = paras;
							size_t n = std::count(cop.begin(), cop.end(), '<');
							for (int i = 0; i < n; i++) {
								if (cop.find("<") != std::string::npos) {
									std::size_t beg = cop.find("<");
									std::size_t end = cop.find(">");
									cop.erase(beg, end+1);
								}
								
							}
							outfile << cop << std::endl;

						}
					}
				}
			}
		}
}
