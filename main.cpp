#include "mainwindow.h"

#include <QApplication>

#include <iostream>
//#include "ConverterJSON.h"
//#include "InvertedIndex.h"
//#include "SearchServer.h"

int main(int argc, char *argv[])
{
//    try
//    {
//        InvertedIndex idx;
//        idx.UpdateDocumentBase(ConverterJSON::GetTextDocuments());

//        SearchServer server(idx);
//        auto answers = server.search(ConverterJSON::GetRequests());

//        ConverterJSON::PutAnswers(answers);
//    }
//    catch (const std::exception &x)
//    {
//        std::cerr << "[ERROR]: " << x.what() << std::endl;
//        return 64;
//    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
