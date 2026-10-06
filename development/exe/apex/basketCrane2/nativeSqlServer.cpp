#include "nativeSqlServer.h"
#include <Windows.h>
#include <comdef.h>
#include <QDateTime>
#import "msado15.dll" rename("EOF","adoEOF") rename_namespace("BasketADO")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")
#ifdef _DEBUG
#pragma comment(lib,"comsuppwd.lib")
#else
#pragma comment(lib,"comsuppw.lib")
#endif

namespace basket {
static QString adoText(const _bstr_t& value) { return QString::fromWCharArray(static_cast<const wchar_t*>(value),value.length()); }
static QVariant adoValue(const _variant_t& value) {
    switch (value.vt) {
    case VT_EMPTY: case VT_NULL: return QVariant();
    case VT_BOOL: return value.boolVal!=VARIANT_FALSE;
    case VT_I1: return int(value.cVal);
    case VT_UI1: return uint(value.bVal);
    case VT_I2: return int(value.iVal);
    case VT_UI2: return uint(value.uiVal);
    case VT_I4: case VT_INT: return int(value.lVal);
    case VT_UI4: case VT_UINT: return uint(value.ulVal);
    case VT_I8: return QVariant::fromValue<qlonglong>(value.llVal);
    case VT_UI8: return QVariant::fromValue<qulonglong>(value.ullVal);
    case VT_R4: return double(value.fltVal);
    case VT_R8: return value.dblVal;
    case VT_DATE: {
        SYSTEMTIME time={}; if (!VariantTimeToSystemTime(value.date,&time)) return QVariant();
        return QDateTime(QDate(time.wYear,time.wMonth,time.wDay),QTime(time.wHour,time.wMinute,time.wSecond,time.wMilliseconds));
    }
    case VT_BSTR: return QString::fromWCharArray(value.bstrVal,SysStringLen(value.bstrVal));
    case VT_ARRAY|VT_UI1: {
        LONG low=0,high=-1; SafeArrayGetLBound(value.parray,1,&low); SafeArrayGetUBound(value.parray,1,&high);
        void* bytes=nullptr; const HRESULT status=SafeArrayAccessData(value.parray,&bytes);
        if (FAILED(status)) _com_issue_error(status);
        const QByteArray result(static_cast<const char*>(bytes),high-low+1); SafeArrayUnaccessData(value.parray); return result;
    }
    default: { _variant_t text; HRESULT status=VariantChangeTypeEx(&text,const_cast<VARIANT*>(static_cast<const VARIANT*>(&value)),LOCALE_INVARIANT,0,VT_BSTR); if (FAILED(status)) _com_issue_error(status); return QString::fromWCharArray(text.bstrVal,SysStringLen(text.bstrVal)); }
    }
}
static QVariant::Type adoType(BasketADO::DataTypeEnum type) {
    switch (type) {
    case BasketADO::adBoolean: return QVariant::Bool;
    case BasketADO::adTinyInt: case BasketADO::adSmallInt: case BasketADO::adInteger: return QVariant::Int;
    case BasketADO::adUnsignedTinyInt: case BasketADO::adUnsignedSmallInt: case BasketADO::adUnsignedInt: return QVariant::UInt;
    case BasketADO::adBigInt: return QVariant::LongLong;
    case BasketADO::adUnsignedBigInt: return QVariant::ULongLong;
    case BasketADO::adSingle: case BasketADO::adDouble: return QVariant::Double;
    case BasketADO::adDate: case BasketADO::adDBDate: case BasketADO::adDBTime: case BasketADO::adDBTimeStamp: return QVariant::DateTime;
    case BasketADO::adBinary: case BasketADO::adVarBinary: case BasketADO::adLongVarBinary: return QVariant::ByteArray;
    default: return QVariant::String; // Includes exact decimal/money values.
    }
}
bool nativeSqlServerValueTests() {
    if (!adoValue(_variant_t()).isNull() || adoValue(_variant_t(true)).toBool()!=true
        || adoValue(_variant_t(42L)).toInt()!=42 || adoValue(_variant_t(12.5)).toDouble()!=12.5
        || adoValue(_variant_t(L"Unicode \x03a9 O'Brien")).toString()!=QString::fromWCharArray(L"Unicode \x03a9 O'Brien")) return false;
    _variant_t large; large.vt=VT_I8; large.llVal=9223372036854775806LL;
    if (adoValue(large).toLongLong()!=large.llVal) return false;
    _variant_t decimal; HRESULT status=VarDecFromStr(const_cast<wchar_t*>(L"12345.6789"),LOCALE_INVARIANT,0,&decimal.decVal); decimal.vt=VT_DECIMAL;
    if (FAILED(status) || adoValue(decimal).toString()!="12345.6789") return false;
    _variant_t binary; binary.vt=VT_ARRAY|VT_UI1; binary.parray=SafeArrayCreateVector(VT_UI1,0,3);
    if (!binary.parray) return false;
    for (LONG i=0;i<3;++i) { BYTE byte=BYTE(i*127); if (FAILED(SafeArrayPutElement(binary.parray,&i,&byte))) return false; }
    if (adoValue(binary).toByteArray()!=QByteArray::fromHex("007ffe")) return false;
    _variant_t date; date.vt=VT_DATE; date.date=2.5;
    return adoValue(date).toDateTime()==QDateTime(QDate(1900,1,1),QTime(12,0));
}
struct NativeSqlServerSession::Impl {
    HRESULT initialized=CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    BasketADO::_ConnectionPtr connection;
    QString password;
    ~Impl() { connection=nullptr; if (SUCCEEDED(initialized)) CoUninitialize(); }
    QSqlError error(const _com_error& failure,QSqlError::ErrorType type) {
        QStringList messages,codes;
        if (connection) {
            try { for (long i=0;i<connection->Errors->Count;++i) {
                const BasketADO::ErrorPtr item=connection->Errors->GetItem(i);
                messages<<adoText(item->Description); codes<<QString::number(item->NativeError);
            } } catch (const _com_error&) {}
        }
        if (messages.isEmpty()) messages<<adoText(failure.Description());
        QString message=messages.join("; ");
        if (message.isEmpty()) message="SQL Server client error.";
        if (!password.isEmpty()) message.replace(password,"[redacted]");
        return QSqlError("Direct SQL Server (MSOLEDBSQL19)",message,type,codes.join(';'));
    }
};
NativeSqlServerSession::NativeSqlServerSession():impl(new Impl) {}
NativeSqlServerSession::~NativeSqlServerSession() { Close(); }
bool NativeSqlServerSession::Open(const QString& connectionString,const QString& password,QSqlError& error) {
    Close(); error=QSqlError();
    impl->password=password;
    if (FAILED(impl->initialized) && impl->initialized!=RPC_E_CHANGED_MODE) {
        error=QSqlError("Direct SQL Server","COM initialization failed",QSqlError::ConnectionError); return false;
    }
    try {
        HRESULT created=impl->connection.CreateInstance(__uuidof(BasketADO::Connection));
        if (FAILED(created)) _com_issue_error(created);
        impl->connection->ConnectionTimeout=15; impl->connection->CommandTimeout=30;
        // The catalog, server, credentials and encryption are configured directly.
        impl->connection->Open(_bstr_t(connectionString.toStdWString().c_str()),L"",L"",BasketADO::adConnectUnspecified);
        return true;
    } catch (const _com_error& failure) { error=impl->error(failure,QSqlError::ConnectionError); Close(); return false; }
}
void NativeSqlServerSession::Close() {
    if (impl->connection) {
        try { if (impl->connection->State!=BasketADO::adStateClosed) impl->connection->Close(); } catch (const _com_error&) {}
        impl->connection=nullptr;
    }
}
bool NativeSqlServerSession::Execute(const QString& sql,QVector<SqlServerResultSet>& sets,QSqlError& error) {
    sets.clear(); error=QSqlError();
    if (!impl->connection) { error=QSqlError("Direct SQL Server","Connection is closed",QSqlError::ConnectionError); return false; }
    try {
        impl->connection->Errors->Clear();
        _variant_t affected;
        BasketADO::_RecordsetPtr result=impl->connection->Execute(_bstr_t(sql.toStdWString().c_str()),&affected,BasketADO::adCmdText);
        do {
            SqlServerResultSet set;
            if (affected.vt!=VT_EMPTY && affected.vt!=VT_NULL) set.affected=adoValue(affected).toInt();
            if (result && (result->State & BasketADO::adStateOpen)) {
                for (long c=0;c<result->Fields->Count;++c) {
                    const BasketADO::FieldPtr field=result->Fields->GetItem(c);
                    QSqlField column(adoText(field->Name),adoType(field->Type));
                    column.setLength(field->DefinedSize); column.setPrecision(field->Precision);
                    set.record.append(column);
                }
                while (!result->adoEOF) {
                    QVector<QVariant> row;
                    for (long c=0;c<result->Fields->Count;++c) row.append(adoValue(result->Fields->GetItem(c)->Value));
                    set.rows.append(row); result->MoveNext();
                }
            }
            sets.append(set);
            if (!result) break;
            affected.Clear(); result=result->NextRecordset(&affected);
        } while (result);
        return true;
    } catch (const _com_error& failure) { sets.clear(); error=impl->error(failure,QSqlError::StatementError); return false; }
}
}
