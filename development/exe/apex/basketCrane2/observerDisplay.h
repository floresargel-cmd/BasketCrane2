#ifndef BASKET_OBSERVER_DISPLAY_H
#define BASKET_OBSERVER_DISPLAY_H
// Same EPICS fields and layer ordering as the current live display. The first
// five columns retain the existing position model's layout.
inline QString observerContentsSql(int basket = 0) {
    return QString("SELECT r.RackId,s.DieNum,r.Pc,s.OrdLen*1000,s.Temper,r.CurrentFinCode,s.StdPract,s.CustShip,r.NextDeptNum "
        "FROM RackDetail r INNER JOIN SOItem s ON r.SONum=s.SONum AND r.SOItemNum=s.SOItemNum "
        "LEFT JOIN (SELECT SONum,SOItemNum,MAX(StopTime) AS StopTime FROM ShiftProd "
        "WHERE StopTime>DATEADD(dd,-7,CAST(FLOOR(CAST(GETDATE() AS FLOAT)) AS SMALLDATETIME)) "
        "AND DeptNum='EXTRUDE' GROUP BY SONum,SOItemNum) latest "
        "ON r.SONum=latest.SONum AND r.SOItemNum=latest.SOItemNum "
        "WHERE r.Pc<>0 AND r.RackId BETWEEN 1 AND 130 %1 ORDER BY latest.StopTime DESC")
        .arg(basket ? QString("AND r.RackId=%1").arg(basket) : QString());
}
inline QString observerDestination(const QVector<QVariant>& row) {
    if (row.size() < 9) return "None";
    if (!row[8].toString().contains("DESTACK", Qt::CaseInsensitive)) return "None";
    const QString practice = row[6].toString();
    if (practice.contains("ANODIZING", Qt::CaseInsensitive) || practice.contains("PAINT", Qt::CaseInsensitive)
        || row[5].toString().trimmed() != "000") return "HCB";
    foreach (const QString& customer, QStringList() << "STARLINE" << "STAARC" << "MILTAC2")
        if (row[7].toString().contains(customer, Qt::CaseInsensitive)) return "HCB";
    foreach (const QString& rule, QStringList() << "CUTBACK" << "STOCK" << "DEPOT" << "O TEMPER @ AGGRESSIVE"
        << "CRIMP" << "PUNCH" << "AZOBRADE" << "AZO" << "ANODIZE" << "ANO" << "PRETREAT" << "COMPONENT MILL")
        if (practice.contains(rule, Qt::CaseInsensitive)) return "HCB";
    return "HCA";
}
inline QColor observerDestinationColor(const QString& destination) {
    if (destination == "HCB") return QColor(150,150,100);
    if (destination == "HCA") return QColor(100,150,150);
    return QColor(100,100,100);
}
#endif
