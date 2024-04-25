//
// Created by antero on 25-04-2024.
//
#include "../headerFiles/Data.h"
#include <fstream>
#include <sstream>
using namespace std;


void Data::readNodes(std::string nodeFilePath) {
    ifstream nodesFile(nodeFilePath);
    if(nodesFile.fail()){
        ostringstream error_message;
        error_message << "Could not open file \"" << nodeFilePath << '"';
        throw ios_base::failure(error_message.str());
    }

    string id_str, longitude_str, latitude_str;

    getline(nodesFile, id_str);

    while(getline(nodesFile, id_str, ','), getline(nodesFile, longitude_str, ','),
    getline(nodesFile, latitude_str, '\n')) {


        double latitude = std::stod(latitude_str);
        double longitude = std::stod(longitude_str);

        network_.addVertex(id_str, longitude, latitude);
    }
}
void Data::readEdges(bool realWorldGraphs, std::string edgesFilePath) {  //bool to skip the first line, since in the realWorldGraphs there's a 1st line to skip
    ifstream edgesFile(edgesFilePath);
    if(edgesFile.fail()){
        ostringstream error_message;
        error_message << "Could not open file \"" << edgesFilePath << '"';
        throw ios_base::failure(error_message.str());
    }

    tourism = false;

    string c1, c2, c3;

    if(realWorldGraphs) {
        getline(edgesFile,c1);
    }
    while(getline(edgesFile, c1, ','), getline(edgesFile, c2, ','),
            getline(edgesFile, c3, '\n')) {

        double weight = std::stod(c3);

        network_.addEdge(c1, c2, weight);
    }
}

void Data::parseTOY(bool tourismCSV, std::string edgesFilePath) {  //bool to store the names of locals only present in the tourismCSV
    ifstream edgesFile(edgesFilePath);
    if(edgesFile.fail()){
        ostringstream error_message;
        error_message << "Could not open file \"" << edgesFilePath << '"';
        throw ios_base::failure(error_message.str());
    }

    if(tourismCSV) {
        tourism = tourismCSV; //so we can know, after processing the tourism.csv, that we processed it (if needed)
    }
    else {
        tourism = false;
    }

    string origem, destino, distancia, labelOrigem, labelDestino;

    getline(edgesFile, origem);

    while(getline(edgesFile, origem, ','), getline(edgesFile, destino, ',')) {



        if(tourismCSV) {
            getline(edgesFile, distancia, ',');
            getline(edgesFile, labelOrigem, ',');
            getline(edgesFile, labelDestino, '\n');
            if (!(tourismLabels.find(origem) != tourismLabels.end() && tourismLabels.find(destino) != tourismLabels.end())) {
                tourismLabels[origem] = labelOrigem;

                labelDestino.erase(std::remove(labelDestino.begin(), labelDestino.end(), '\r'), labelDestino.end());
                tourismLabels[destino] = labelDestino;
            }
        }

        else {
            getline(edgesFile, distancia, '\n');
        }

        double weight = std::stod(distancia);

        if(!network_.addEdge(origem, destino, weight)) {
            network_.addVertex(origem,0,0);
            network_.addVertex(destino,0,0);
            network_.addEdge(origem, destino, weight);
        }
    }
}

Graph Data::getNetwork() {
    return network_;
}

std::map<std::string, std::string> Data::getTourismLabels() {
    return tourismLabels;
}
