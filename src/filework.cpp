/**
 * \author Dennis Vymer, xvymer00
 * \author Pavel Gos, xgospa00
 *
 */

#include "filework.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QErrorMessage>
#include <QRegularExpression>
#include "tabmanagment.h"
#include "game.h"

//extern Game *game;
extern QList <Game *> games;
extern TabManagment *tab;

FileWork::FileWork(QString fileName)
{
    filename = fileName;
    currentTranscriptIdx = -1;
}

void FileWork::start(int idx)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(nullptr, "error", file.errorString());
        return;
    }

    QTextStream in(&file);
    QStringList linesTmp;
    while(!in.atEnd()){
        QString line = in.readLine();
        //we dont want any empty lines or eof appended in the list
        if(line != ""){
            line = line.remove(QRegExp("^\\s*")); // trim whitespace from the start of a line
            line = line.remove(QRegExp("\\s*$")); // trim whitespace from the end of a line
            linesTmp.append(line);
        }
    }

    QString line;
    QRegularExpression reg("^[1-9][0-9]*.[ ]?([(KDVSJp]?[a-h]?[1-8]?[x]?[a-h][1-8][DVSJ]?[+#]?) [(KDVSJp]?[a-h]?[1-8]?[x]?[a-h][1-8][DVSJ]?[+#]?$"); //ugly, but for sure correct
    QRegularExpression lastTurn("^([0-9]*.[ ]?[(KDVSJp]?[a-h]?[1-8]?[x]?[a-h][1-8][DVSJ]?[+#]?)$");
    QRegularExpressionMatch match;
    QRegularExpressionMatch matchLast;
    bool hasMatch;
    QString instrIndex;
    QStringList tempList;
    bool isBlack = false;
    //checking whole document of instrucions
    for(int i = 0; i < linesTmp.count(); i++){
        line = linesTmp[i];
        match = reg.match(line);
        matchLast = lastTurn.match(line);

        hasMatch = match.hasMatch(); // should be true if there is valid line
        if(!hasMatch)   // if the line does not match
        {
            hasMatch = matchLast.hasMatch(); // it might me short version with white side winning the game
            if(!hasMatch) // if its not last instruction with white side winning...
            {
                QMessageBox::critical(nullptr, "Error", "Wrong instruction format in selected file: "+line);
                return;
            }
        }
        tempList = line.split(".");
        instrIndex = tempList[0];
        instrIndex = instrIndex.toInt();

        if(instrIndex != i+1)
        {
            QMessageBox::critical(nullptr, "Error", "Wrong line index in selected instruction file: "+line);
            return;
        }
        isBlack = !isBlack;
    }// for

    //REMOVING the 1., 2.,... the positional number after checking it, not necessary at all
    //Now split the linesTmp into black and white instructions into lines
    for(int i = 0, idx = 0; i < linesTmp.count(); ++i, idx += 2){ //increase idx by 2
        //remove the DOT 1. 2. 14. etc
        QStringList tmp = linesTmp[i].split(".");
        linesTmp[i] = tmp[1].trimmed();

        tmp = linesTmp[i].split(" ");
        if(tmp.count() == 2){ // 'e2e4 e7e5'
            lines.append(tmp[0]);
            lines.append(tmp[1]);
        }
        else if(tmp.count() == 1){ // only qwhite turn
            lines.append(tmp[0]);
        }

    }

    gameIdx = idx;
}

bool FileWork::increaseCounter()
{
    //we dont wanna scroll past that stuff do we?
    if(currentTranscriptIdx < lines.count() -1){
        currentTranscriptIdx++;
        return true;
    }
    else
        return false;
}

bool FileWork::decreaseCounter()
{
    if(currentTranscriptIdx > 0){
        currentTranscriptIdx--;
        return true;
    }
    else
        return false;
}



void FileWork::createTranscriptString(QString pieceName, QString turn, int retVal, int oldX, int oldY, int newX, int newY)
{
    Game *game = games[gameIdx];
    bool isblack = (turn == "BLACK");

    //remove the stuff after the currentIdx
    while(currentTranscriptIdx  <= lines.count() -2 ){
        lines.removeLast();
    }

    QString letter1;
    if(oldX == 0)
        letter1 = 'a';
    else if(oldX == 1)
        letter1 = 'b';
    else if(oldX == 2)
        letter1 = 'c';
    else if(oldX == 3)
        letter1 = 'd';
    else if(oldX == 4)
        letter1 = 'e';
    else if(oldX == 5)
        letter1 = 'f';
    else if(oldX == 6)
        letter1 = 'g';
    else if(oldX == 7)
        letter1 = 'h';

    QString letter2;
    if(newX == 0)
        letter2 = 'a';
    else if(newX == 1)
        letter2 = 'b';
    else if(newX == 2)
        letter2 = 'c';
    else if(newX == 3)
        letter2 = 'd';
    else if(newX == 4)
        letter2 = 'e';
    else if(newX == 5)
        letter2 = 'f';
    else if(newX == 6)
        letter2 = 'g';
    else if(newX == 7)
        letter2 = 'h';
    QString temporaryTranscriptLine;

    if(retVal == 0){
        temporaryTranscriptLine = pieceName +letter1+ QString::number(8 - oldY)+letter2 + QString::number(8 - newY);
    }
    else if(retVal == 1){
        temporaryTranscriptLine = pieceName +letter1 + QString::number(8 - oldY)+"x" +letter2 + QString::number(8 - newY);
    }
    //PAWN:we threw somebody out and exchanged it
    else if(retVal == 2){
        //piseme d8D, nazev nove figurky ke konci
        //piece name tady obsahuje jmeno nove figurky

        temporaryTranscriptLine = letter1 + QString::number(8 - oldY)+"x" +letter2 + QString::number(8 - newY) + pieceName;
    }
    //PAWN:we got to the enemy end and exchanged for soimething, no throwing out
    else if(retVal == 3){
        temporaryTranscriptLine = letter1+ QString::number(8 - oldY) + pieceName;
    }

    if(game->isCheck(!isblack, -1, -1) == 1)
        temporaryTranscriptLine += "+";

    lines.append(temporaryTranscriptLine);
    currentTranscriptIdx++;
    writeTranscriptToFile();
}

void FileWork::writeTranscriptToFile()
{
    QString fileInput;
    for(int i = 0, idx = 0; i < lines.count(); ++i){
        if(i % 2 == 0){
            idx++;
            fileInput += QString::number(idx) +". " + lines[i] + " ";
        }
        else
            fileInput += lines[i] + "\n";
    }

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      // We're going to streaming text to the file
      QTextStream stream(&file);

      stream << fileInput;

      file.close();
    }
    else{
        QMessageBox::information(nullptr, "error", file.errorString());
        return;
    }
}

