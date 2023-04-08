#ifndef CONVERTERJSON_HPP
#define CONVERTERJSON_HPP


#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "SearchServer.hpp"
using Json = nlohmann::json;

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
private:
    /**
	* Метод удостоверения в наличии и полноте файла config.json
	* @throws std::logic_error с указанием отсутствия или пустоты файла
	* @exceptsafe Strong - программа завершается, ни на что не влияя
	* @return Возвращает структурированный Json-объект
	*/
    static Json ReadConfigSafely();

public:
    ConverterJSON() = default;
    /**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
    static std::vector<std::string> GetTextDocuments();
    /**
	* Метод считывает поле max_responses для определения
	* @return Возвращает предельное количества ответов на один запрос
	*/
    static int GetResponsesLimit();
    /**
	* Метод получения запросов из файла requests.json
	* @return Возвращает список запросов из файла requests.json
	*/
    static std::vector<std::string> GetRequests();
    /**
	* Положить в файл answers.json результаты поисковых запросов
	*/
    static void PutAnswers(const std::vector<std::vector<RelativeIndex>>& answers);
};
#endif // CONVERTERJSON_HPP
