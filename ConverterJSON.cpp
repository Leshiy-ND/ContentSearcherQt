#include "ConverterJSON.h"

#include <fstream>
#include <iostream>
#include "VersionConfig.h"
#include <exception>


Json ConverterJSON::ReadConfigSafely()
{
	std::ifstream iFile("config.json");
	if (!iFile.is_open())
		throw std::logic_error("config file is missing");
	Json json;
	try { json = Json::parse(iFile); }
	catch (const std::exception &x)
	{
		std::string error_message = "Poorly formatted config's json file:\n    ";
		throw std::logic_error(error_message + x.what());
	}
	iFile.close();
	if (!json.contains("config"))
		throw std::logic_error("config file is empty");

	if (!json["config"].contains("version"))
		throw std::logic_error("config.json has no file version");
	if (APPLICATION_VERSION != json["config"]["version"])
		throw std::logic_error("config.json has incorrect file version");

	return json;
}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
	Json json = ReadConfigSafely();

	if (json["config"].contains("name"))
		std::cout << "[MESSAGE]: Starting " << std::string(json["config"]["name"]) << "..." << std::endl;
	else
		std::cout << "[WARNING]: No \"name\" specified at config.json." << std::endl;

	if (!json.contains("files"))
	{
		std::cout << "[WARNING]: No \"files\" specified at config.json." << std::endl;
		return {};
	}
	std::vector<std::string> texts;
	texts.reserve(json["files"].size());
	std::stringstream buffer;
	for (const auto& path : json["files"])
	{
		std::ifstream document(path);
		if (document.is_open())
		{
			buffer.str(std::string());
			buffer << document.rdbuf();
			texts.emplace_back(buffer.str());
			document.close();
		}
		else
		{
			texts.emplace_back("");
			std::cout << "[WARNING]: No such file as \"" << std::string(path) << "\" or no access to path." << std::endl;
		}
	}
	return texts;
}

int ConverterJSON::GetResponsesLimit()
{
	try
	{
		std::ifstream iFile("config.json");
		Json json = Json::parse(iFile);
		iFile.close();
		return json["config"]["max_responses"];
	}
	catch (const std::exception &x)
	{
		return 5;
	}
}

std::vector<std::string> ConverterJSON::GetRequests()
{
	std::ifstream iFile("requests.json");
	if (!iFile.is_open())
		throw std::logic_error("requests file is missing");
	Json json;
	try { json = Json::parse(iFile); }
	catch (const std::exception &x)
	{
		std::string error_message = "Poorly formatted requests' json file:\n    ";
		throw std::logic_error(error_message + x.what());
	}
	iFile.close();

	if (!json.contains("requests"))
		throw std::logic_error("requests file is empty");

	return json["requests"].get<std::vector<std::string>>();
}

void ConverterJSON::PutAnswers(const std::vector<std::vector<RelativeIndex>>& answers)
{
	char zfill = answers.size() / 10 + 1;
	if (zfill < 3) zfill = 3;
	Json json;

	for (auto it = answers.begin(); it < answers.end(); ++it)
	{
		std::string reqKey = std::to_string(it - answers.begin() + 1);
		reqKey.insert(0, zfill - reqKey.size(), '0');
		reqKey.insert(0, "request");
		json["answers"][reqKey]["result"] = !it->empty();
		if (it->empty()) continue;
		if (it->size() == 1)
		{
			json["answers"][reqKey]["docid"] = it->begin()->doc_id;
			json["answers"][reqKey]["rank"]  = it->begin()->rank;
			continue;
		}
		Json relevance;
		for (auto &&pair : *it)
		{
			relevance["docid"] = pair.doc_id;
			relevance["rank"]  = pair.rank;
			json["answers"][reqKey]["relevance"].push_back(relevance);
		}
	}
	std::ofstream oFile("answers.json");
	if (oFile.is_open())
	{
		oFile << std::setw(4) << json;
		oFile.close();
		std::cout << "[MESSAGE]: The answers were put into answers.json" << std::endl;
	}
	else
	{
		std::cout << "[WARNING]: Unable to create or modify answers.json" << std::endl;
	}
}
