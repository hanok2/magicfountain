#include "script.h"

Script::Script(QString script)
{
    QStringList lines = script.split("\n");
    QString text;

    QString content = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
    content += "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
    content += "p, li { white-space: pre-wrap; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; }";
    content += "</style></head><body>";

    int blockcount = lines.size();
    int i = 0;
    bool wasAnAction = false;

    while(i < blockcount){
        text = lines.at(i).trimmed();

        if(text.left(1) == "!"){ //Forced action
            content.append("<p>" + checkBoldItalicUnderline(text.mid(1)) + "</p>");
            wasAnAction = true;
        }
        else if(text.left(6) == "Title:"){ //Title
            content.append("<br/><br/><br/><br/><br/><br/><br/><br/><br/><p style=\"text-align: center;\">" + checkBoldItalicUnderline(text.mid(6).trimmed()));
            title.append(text.mid(6).trimmed());
            i++;
            text = lines.at(i);

            while(text.left(1) == "\t" && i < blockcount){
                content.append("<br/>" + checkBoldItalicUnderline(text.trimmed()));
                title.append(text.trimmed());

                i++;
                text = lines.at(i);
            }
            i--;
            content.append("<br/></p>");
        }
        else if(text.left(7) == "Credit:"){ //Credit
            content.append("<p style=\"text-align: center;\">" + checkBoldItalicUnderline(text.mid(7).trimmed()) + "</p>");
        }
        else if(text.left(7) == "Author:"){ //Author
            content.append("<p style=\"text-align: center;\">" + checkBoldItalicUnderline(text.mid(7).trimmed()) + "<br/></p>");
        }
        else if(text.left(7) == "Source:"){ //Source
            content.append("<p style=\"text-align: center;\">" + checkBoldItalicUnderline(text.mid(7).trimmed()) + "<br/></p>");
        }
        else if(text.left(11) == "Draft date:"){ //Draft date
            content.append("<br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><br/><p style=\"text-align: left;\">" + checkBoldItalicUnderline(text.mid(11).trimmed()) + "</p>");
        }
        else if(text.left(8) == "Contact:"){ //Contact
            content.append("<p style=\"text-align: left;\">" + checkBoldItalicUnderline(text.mid(8).trimmed()));
            contact.append(text.mid(8).trimmed());

            i++;
            text = lines.at(i);

            while(text.left(1) == "\t" && i < blockcount){
                content.append("<br/>" + checkBoldItalicUnderline(text.trimmed()));

                i++;
                text = lines.at(i);
            }
            i--;
            content.append("<br/></p>");
        }
        else if(text.left(2) == "# "){ //Act
            //Not used yet
        }
        else if(text.left(3) == "## "){ //Sequence
            //Not used yet
        }
        else if(text.left(4) == "### "){ //Scene
            //Not used yet
        }
        else if(text.left(3).toLower() == "int" || text.left(3).toLower() == "ext" || text.left(8).toLower() == "int./ext" || text.left(7).toLower() == "int/ext" || text.left(3).toLower() == "i/e"){ //Scene heading
            content.append("<p><br/>" + checkBoldItalicUnderline(text) + "</p>");
            wasAnAction = false;
        }
        else if(text.left(1) == ">"){
            if(text.right(1) == "<"){ //Centered
                content.append("<p style=\"text-align:center;\">" + checkBoldItalicUnderline(text.mid(1, text.size()-2)) + "</p>");
            }
            else{ //Transition
                content.append("<p style=\"text-align:right;\"><br/>" + checkBoldItalicUnderline(text.mid(1)) + "</p>");
            }
            wasAnAction = false;
        }
        else if(text.right(3) == "TO:"){ //Transition
            content.append("<p style=\"text-align: right;\"><br/>" + checkBoldItalicUnderline(text) + "</p>");

            wasAnAction = false;
        }
        else if(text.left(1) == "." && text.mid(1, 1) != "."){ //Forced scene heading
            content.append("<p>" + checkBoldItalicUnderline(text.mid(1)) + "</p>");

            wasAnAction = false;
        }
        else if(text.toUpper() == text && !text.isEmpty()){ //Dialogue
            content.append("<p><br/>                     " + checkBoldItalicUnderline(text) + "</p>"); //Character name
            i++;
            text = lines.at(i).trimmed();

            while(!text.isEmpty() && i < blockcount){
                text = lines.at(i).trimmed();

                if(text.left(1) == "(" && text.right(1) == ")"){ //Parenthetical
                    content.append("<p>               " + checkBoldItalicUnderline(text) + "</p>");
                }
                else if(!text.isEmpty()){ //Text
                    content.append("<p>           " + checkBoldItalicUnderline(text) + "</p>");
                }

                i++;
            }
            i--;

            wasAnAction = false;
        }
        else if(!text.isEmpty()){ //Default action
            content.append("<p>");
            if(!wasAnAction){
                content.append("<br/>");
            }

            text = lines.at(i);
            content.append(checkBoldItalicUnderline(text) + "</p>");

            wasAnAction = true;
        }

        i++;
    }

    content += "</body></html>";

    html = content;
}

QString Script::checkBoldItalicUnderline(QString text){
    QString result = text;

    //Check Bold with Italic, ex: ***something***
    while(result.contains("***") && result.mid(result.indexOf("***") + 3).contains("***")){
        result.replace(result.indexOf("***"), 3, "<b><i>");
        result.replace(result.indexOf("***"), 3, "</i></b>");
    }

    //Check Bold, ex: **something**
    while(result.contains("**") && result.mid(result.indexOf("**") + 2).contains("**")){
        result.replace(result.indexOf("**"), 2, "<b>");
        result.replace(result.indexOf("**"), 2, "</b>");
    }

    //Check Italic, ex: *something*
    while(result.contains("*") && result.mid(result.indexOf("*") + 1).contains("*")){
        result.replace(result.indexOf("*"), 1, "<i>");
        result.replace(result.indexOf("*"), 1, "</i>");
    }

    //Check Underline, ex: _something_
    while(result.contains("_") && result.mid(result.indexOf("_") + 1).contains("_")){
        result.replace(result.indexOf("_"), 1, "<u>");
        result.replace(result.indexOf("_"), 1, "</u>");
    }

    return result;
}

QString Script::toHtml(){
    return html;
}
