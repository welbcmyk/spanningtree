// spanningtee_simulation.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <ostream>
#include "inputHandler.h"

using namespace std;

int main()
{
    inputHandler handler;
    SpanTreeGenerator generator;
    TreeMatrix spanTree;
    string file = handler.getPath();
    cout << "Creating Spantree from " << file << endl;
    try
    {
        spanTree = generator.getSpanTree(file);
    }
    catch (const InvalidSpanTreeFileException e)
    {
        cout << "An exception occuered..." << endl << e.what() << endl;
        return 0;
    }
    cout << "Created Spantree\n\n" << endl;
    spanTree.printTree(cout);
    cout << endl;
    cout << "Start:\n";
    //spanTree.printNodes(cout);
    spanTree.printMatrix(cout);
    int i = 0;
    while(!spanTree.allNodesReceivedMessages(spanTree.matrix.size()))
    {
        spanTree.updateAllNodes();
        cout << "===\n" << to_string(++i) << ". Iteration\n---\n";
        //spanTree.printNodes(cout);
        spanTree.printMatrix(cout);
    }
    spanTree.printResult(cout);
}

// Programm ausführen: STRG+F5 oder "Debuggen" > Menü "Ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
