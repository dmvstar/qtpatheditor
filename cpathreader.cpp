#include "cpathreader.h"
#include <memory>
#include <QDebug>

/*
#include <sstream>
#include <regex>
*/
QStringList splitPathStr( QString path, QString separator)
{
    QRegExp rx("[^" + separator + "]+");
    QStringList list;
    int pos = 0;

    //qDebug() << "splitPathStr path [" << path << "]";

    while ((pos = rx.indexIn(path, pos)) != -1) {
        //qDebug() << "splitPathStr path [" << pos << "]["+rx.cap(0)+"]";
        list << rx.cap(0);
        pos += rx.matchedLength();
    }

    return list;
}

QString joinPathList( QStringList pathList, QString separator){
    return pathList.join( separator );
}

CPathReader::CPathReader()
{
}

CPathReader::CPathReader( HKEY hKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName) : hKey_( hKey), lpszKeyName_( lpszKeyName), lpszValueName_( lpszValueName)
{
}

CPathReader::~CPathReader()
{
}

bool CPathReader::Read( QStringList& strList )
{
    HKEY hPathKey = 0;
    if( RegOpenKeyEx( hKey_, lpszKeyName_, 0, KEY_QUERY_VALUE, &hPathKey) != ERROR_SUCCESS)
        return false;

    ULONG nChars = 0;
    if( RegQueryValueEx( hPathKey, lpszValueName_, 0, 0, 0, &nChars) != ERROR_SUCCESS)
        return false;

    // empty path is not failure
    if(nChars == 0)
        return true;

    std::vector<BYTE> vBuffer(nChars + sizeof(wchar_t), 0);
    LPBYTE lpBuffer = vBuffer.data();
    if( RegQueryValueEx( hPathKey, lpszValueName_, 0, 0, lpBuffer, &nChars) != ERROR_SUCCESS)

    qDebug() << "CPathReader::Read [" << "" << "]";

    strList = splitPathStr( QString::fromUtf16((ushort*)lpBuffer), ";" );

    return true;
}

bool CPathReader::Write( const QStringList& strList )
{
    HKEY hPathKey = 0;
    if( RegOpenKeyEx( hKey_, lpszKeyName_, 0, KEY_SET_VALUE, &hPathKey) != ERROR_SUCCESS)
        return false;

    QString strValue = joinPathList( strList, ";" );

    const BYTE* lpcBuffer = reinterpret_cast<const BYTE*>( strValue.toStdWString().c_str() );
    DWORD cbData = static_cast<DWORD>( strValue.size() * sizeof(wchar_t));
    LSTATUS lStatus = RegSetValueEx( hPathKey, lpszValueName_, 0, REG_EXPAND_SZ, lpcBuffer, cbData);
    return lStatus == ERROR_SUCCESS;
}


/*

bool _Read( StringListT& strList )
{
    HKEY hPathKey = 0;
    if( RegOpenKeyEx( hKey_, lpszKeyName_, 0, KEY_QUERY_VALUE, &hPathKey) != ERROR_SUCCESS)
        return false;

  //  std::shared_ptr<void> afPathKey( hPathKey, RegCloseKey);

    ULONG nChars = 0;
    if( RegQueryValueEx( hPathKey, lpszValueName_, 0, 0, 0, &nChars) != ERROR_SUCCESS)
        return false;

    // empty path is not failure
    if(nChars == 0)
        return true;

    std::vector<BYTE> vBuffer(nChars + sizeof(wchar_t), 0);
    LPBYTE lpBuffer = vBuffer.data();
    if( RegQueryValueEx( hPathKey, lpszValueName_, 0, 0, lpBuffer, &nChars) != ERROR_SUCCESS)
        return false;

    std::wstring sBuffer(reinterpret_cast<wchar_t*>(lpBuffer));
    strList = split_string_by( sBuffer, L';');
}


bool CPathReader::Write( const StringListT& strList)
{
    HKEY hPathKey = 0;
    if( RegOpenKeyEx( hKey_, lpszKeyName_, 0, KEY_SET_VALUE, &hPathKey) != ERROR_SUCCESS)
        return false;

    std::shared_ptr<void> afPathKey( hPathKey, RegCloseKey);

    std::wstring strValue = join_vector_by( strList, L';');
    const BYTE* lpcBuffer = reinterpret_cast<const BYTE*>( strValue.c_str());
    DWORD cbData = static_cast<DWORD>( strValue.size() * sizeof(wchar_t));
    LSTATUS lStatus = RegSetValueEx( hPathKey, lpszValueName_, 0, REG_EXPAND_SZ, lpcBuffer, cbData);
    return lStatus == ERROR_SUCCESS;
}
*/
