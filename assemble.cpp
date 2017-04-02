#include "assemble.h"

Assemble::Assemble():code("")
{

}

void Assemble::Read(const QString &content)
{
    QString temp = content.trimmed();
    code = temp.split("\n");
    for(auto i = code.begin(); i != code.end(); ++i)
    {
        *i = i->trimmed();
        if(!(i->isEmpty()))
        {
            i->replace(QRegExp("//[^\n]*|#[^\n]*"), "");
            if(i->contains(QRegExp("\\b[A-Za-z0-9_]+:")) == true)
            {
                QString lb = i->left(i->indexOf(":"));
                map.insert(lb, i - code.begin());
            }
        }
        else
            code.removeAt(i - code.begin());
    }
}
