SET filesLocation=%cd%
echo %filesLocation%
for %%f in (%filesLocation%) do set projectName=%%~nxf
cd..
for %%f in (%cd%) do set customerName=%%~nxf
cd..
cd..
cd #moc
mkdir %customerName%
cd %customerName%
mkdir %projectName%
cd %projectName%
SET targetLocation=%cd%
mkdir mocFiles.4.8.1
mkdir mocFiles.5.1.1
cd..
cd..
cd..
cd libs
cd #qt
cd moc
callMoc.5.7.0.exe %filesLocation% %targetLocation%
cd %filesLocation%
cd..