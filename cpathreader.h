#ifndef CPATHREADER_H
#define CPATHREADER_H

#include <windows.h>
#include <string>
#include <vector>
#include <QString>
#include <QStringList>
#include <QRegExp>

typedef std::vector<std::wstring> StringListT;

class CPathReader
{
private:

    HKEY hKey_;
    LPCTSTR lpszKeyName_, lpszValueName_;


public:
    CPathReader();
    CPathReader( HKEY hKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName);
    ~CPathReader();


    bool Read( QStringList& strList );
    bool Write( const QStringList& strList);

};

#endif // CPATHREADER_H
