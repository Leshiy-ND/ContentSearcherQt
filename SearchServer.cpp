#include "SearchServer.hpp"

#include <map>
#include <set>
#include <cmath>
#include "ConverterJSON.hpp"


bool RelativeIndex::operator==(const RelativeIndex& other) const
{
	return (this->doc_id == other.doc_id && fabsf(this->rank - other.rank) < .01f);
}


std::vector<std::vector<RelativeIndex>> SearchServer::search(
		const std::vector<std::string>& requests_input)
{
	std::vector<std::vector<RelativeIndex>> answersToAllRequests;
	answersToAllRequests.reserve(requests_input.size());
	auto max_responses = ConverterJSON::GetResponsesLimit();

	for (auto&& request : requests_input)
	{
		if (request.empty())
		{
			answersToAllRequests.emplace_back();
			continue;
		}
		// Если запрос пуст, то ответа на него не будет (предварительный шаг 6)

		std::string word;
		std::map<std::string, std::size_t> tmp_dict; // После заполнения, переведётся в vec<str> unique_words
		for (auto&& symbol : request)
		{
			if      ('a' <= symbol && symbol <= 'z') word += symbol;
			else if ('A' <= symbol && symbol <= 'Z') word += char(_tolower(symbol));
			else
			{
				if (word.empty()) continue;
				if (tmp_dict.find(word) == tmp_dict.end()) // v copy_start v
				{
					std::size_t tmp_int = 0;
					auto entries = index.GetWordCount(word);
					for (auto&& entry : entries)
						tmp_int += entry.count;
					tmp_dict[word] = tmp_int;
				}
				word.clear();  //  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  copy_end  ^
			}
		}
		if (tmp_dict.find(word) == tmp_dict.end()) // v copy_start v
		{
			std::size_t tmp_int = 0;
			auto entries = index.GetWordCount(word);
			for (auto&& entry : entries)
				tmp_int += entry.count;
			tmp_dict[word] = tmp_int;
		}
		word.clear();  //  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  copy_end  ^
		// Запрос разделён на слова с приложенной суммой вхождений (шаг 0.5)

		std::vector<std::string> unique_words; // Это vector (не set), так как нам важен порядок элементов
		unique_words.reserve(tmp_dict.size());
		for (auto&& word_n_count : tmp_dict)
			unique_words.emplace_back(word_n_count.first);
		for (auto itI = unique_words.begin(); itI < unique_words.end() - 1; ++itI)
			for (auto itJ = itI + 1; itJ < unique_words.end(); ++itJ)
				if (tmp_dict[*itI] > tmp_dict[*itJ])
					std::iter_swap(itI, itJ);
		tmp_dict.clear();
		// Слова запроса рассортированы в vec<str> unique_words по частоте вхождения в документы (шаги 1-3)

		std::set<std::size_t> doc_ids; // Это set (не vector), так как он удобнее для хранения уникальных значений
		if (!unique_words.empty())
			for (auto&& entry : index.GetWordCount(unique_words[0]))
				doc_ids.insert(entry.doc_id);
		if (unique_words.size() > 1)
			for (auto it = unique_words.begin() + 1; it != unique_words.end(); ++it)
			{
				for (auto &&entry : index.GetWordCount(*it))
					if (doc_ids.find(entry.doc_id) == doc_ids.end())
						doc_ids.insert(entry.doc_id);
			}
		// Составлен список подходящих по запросу документов (шаги 4-5)

		if (doc_ids.empty())
		{
			answersToAllRequests.emplace_back();
			continue;
		}
		// Если должные документы не найдены (шаг 6)

		std::map<std::size_t, std::size_t> absRelevances; // {doc_id, absRelevance}
		for (auto&& unique_word : unique_words)
		{
			auto entries = index.GetWordCount(unique_word);
			for (auto&& entry : entries)
			{
				if (doc_ids.find(entry.doc_id) == doc_ids.end()) // Если это вхождение неудобоваримово документа
					continue;
				if (absRelevances.find(entry.doc_id) == absRelevances.end())
					absRelevances[entry.doc_id] = 0;
				absRelevances[entry.doc_id] += entry.count;
			}
		}
		unique_words.clear();
		doc_ids.clear(); // Отныне все должные документы перечислены в map absRelevances

		std::size_t highestRelevance = 1;
		for (auto&& absRelevance : absRelevances)
			if (highestRelevance < absRelevance.second)
				highestRelevance = absRelevance.second;

		std::vector<RelativeIndex> answersToOneRequest; // Составляющая answersToAllRequests (vector (of i) для vector of vectors (of i))
		answersToOneRequest.reserve(doc_ids.size());
		for (auto&& absRelevance : absRelevances)
		{
			auto& doc_id = absRelevance.first;
			float rank = (float)absRelevance.second / (float)highestRelevance;
			answersToOneRequest.push_back({doc_id, rank});
		}
		absRelevances.clear();
		// Относительные релевантности подсчитаны (шаг 7)

		auto stopPoint = answersToOneRequest.end() - 1;
		if (stopPoint > answersToOneRequest.begin() + max_responses)
			stopPoint = answersToOneRequest.begin() + max_responses;

		for (auto itI = answersToOneRequest.begin(); itI < stopPoint; ++itI)
			for (auto itJ = itI + 1; itJ < answersToOneRequest.end(); ++itJ)
				if (fabsf(itI->rank - itJ->rank) < .01f && itI->doc_id > itJ->doc_id
						|| itI->rank < itJ->rank)
					std::iter_swap(itI, itJ);
		// Результаты отсортированы по относительной релевантности (Б->М) и id документов (М->Б) (шаг 8)
		// Только те, что будут выведены

		if (answersToOneRequest.size() > max_responses)
			answersToOneRequest.resize(max_responses);
		answersToAllRequests.emplace_back(std::move(answersToOneRequest));
	}
	return answersToAllRequests;
}
