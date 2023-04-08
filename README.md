# ContentSearcher

Program for searching documents.

Read this in other languages: [English](README.md), [Русский](README.ru.md)

_(Incode comments are written in Russian yet.)_

_**NOTE:** This text is work in progress._

---

## Project Description

Local search engine.

This project was completed as a final work on the C++ course from SkillBox.

---

## Technology stack

- programming language **С++**
- development environment **CLion**
- build automation tool **CMake - Ninja**
- compiler **GCC v12.2.1**
- operating system **Manjaro KDE (Arch Linux)**
	- (without including OS-special libraries)
- version control **Git - GitHub**
- library for working with json files **nlohmann_json**
- test management library **google_test**

---

## Instructions for compiling

1. Make sure the following files are present and up to date:
	- folder [include](include)
	- folder [src](src)
	- [CMakeLists.txt](CMakeLists.txt)
	- [VersionConfig.h.in](VersionConfig.h.in)
2. Open the directory in any **C++** IDE that supports **CMake - Ninja**;
3. Build the **ContentSearcher** project described in [CMakeLists.txt](CMakeLists.txt);
4. Go to the **src** folder that should have been generated inside your build folder;
5. Copy/move the **ContentSearcher** executable file to any convenient directory.

---

## Instructions for use

1. Open the [examples](examples) folder;
2. Copy the files [config.json](examples/config.json) and [requests.json](examples/requests.json) into the same directory as your **ContentSearcher** executable;
3. Open your configuration file **config.json**:
	- Specify the maximum number of responses per request in the field **_"max_responses"_**,
	- list the paths to the files to be searched in the **_"files"_** field (paths specified by formulas are not accepted, one path - one file);
4. Open your requests file **requests.json**:
	- list your search requests in the **_"requests"_** field;
5. Run your **ContentSearcher** executable:
	- recomended to run within terminal/shell, otherwise you wouldn't be notified about possible errors,
	- in case of a fatal error, read its description and take appropriate countermeasures;
6. View the search results when you receive a message about writing of the **answers.json** file in the same directory.

The program performs an inverted indexing of words in each specified document and displays as a response the numbers of documents (corresponding to the positions of the provided list) in order of relevance, calculated using the sum of occurrences of the search words in each individual document.

_**WARNING:** The document is considered part of the answer even if it does not contain all of the search words. Be careful with conjunctions and other common or short words in your requests._

Terminal output:
- **[MESSAGE]** - message indicating the stage of program operation;
- **[WARNING]** - warning about a non-fatal error;
- **[ERROR]** - a fatal error that urgently closes the program.

### When reusing, start from step #3.

_**WARNING:** The new **answers.json** always overwrites an old one._