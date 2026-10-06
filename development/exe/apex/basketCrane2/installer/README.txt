Apex Basket Crane 2 installer and release versioning

Application releases start at 2.5.8. The middle and final version fields range
from 0 to 99. Incrementing 2.5.99 yields 2.6.0; 2.99.99 yields 3.0.0.
The Windows MSI major field is limited to 255; overflow fails explicitly.
Qt 5.7.0 library/project filenames identify the toolchain, not the app version.

version.h is the shared application, Windows resource and installer version.
The EXE name is stable: apexBasketCrane2.exe (apexBasketCrane2D.exe in Debug).
The application exposes --version and displays its release in the window title
and the Basket Crane 2 header. The 3D overview shows source/destination arrows
for active missions, including empty positions, in LiveObserver as well as Live.

Prepare the build machine: Visual Studio C++ desktop toolset, Windows SDK and
the repository's existing Qt/vendor libraries, .NET SDK, and WiX CLI 4.0.6:
  dotnet tool install wix --version 4.0.6 --tool-path C:\build-tools\wix4
  C:\build-tools\wix4\wix.exe extension add -g WixToolset.UI.wixext/4.0.6

Every Release application build automatically increments version.h before
compilation, including an otherwise up-to-date Build and Rebuild. Debug
application builds leave the version unchanged. A failed Release build may
consume a version number; it is not rolled back or reused.
Release solution builds increment once; the installer skips an additional
increment for its isolated packaging build. A standalone installer build
builds a Release application and increments once, including when requested
from a Debug solution. SkipApplicationBuild packages an existing release
without incrementing. SkipVersionIncrement can explicitly reuse a prepared
version when invoking Build-Installer.ps1 directly.

Build a new release:
  powershell -File installer\Build-Installer.ps1 -Wix C:\build-tools\wix4\wix.exe
Output is installer\out\ApexBasketCrane2-<version>-x86.msi.
BuildDirectory and MSBuild parameters allow isolated build output/tool paths.
The solution contains a buildable Installer utility project. Build Solution
with Debug or Release and Win32 or Mixed Platforms also builds the MSI.
The installer always packages a Release application, even in Debug solutions.
To build only the MSI, right-click Installer and choose Build. Output is in
installer\out. Win64/x64 solution configurations remain excluded from builds.
The packaging script builds the C++ project directly to avoid recursion.
The standalone MSBuild entry point is also available:
  msbuild installer\ApexBasketCrane2.Installer.proj /p:WixPath=C:\build-tools\wix4\wix.exe
WixPath is optional when wix is on PATH, installed as a global dotnet tool,
or installed at C:\build-tools\wix4. The Visual Studio/MSBuild entry point
prefers C:\build-tools\wix4 when present and passes its path explicitly.
The Installer project bypasses Visual Studio's fast up-to-date check so that
Build always invokes packaging and reports packaging failures.
The script also supports the auto-detected
per-user location %LOCALAPPDATA%\Apex\build-tools\wix4:
  dotnet tool install wix --version 4.0.6 --tool-path "%LOCALAPPDATA%\Apex\build-tools\wix4"
  "%LOCALAPPDATA%\Apex\build-tools\wix4\wix.exe" extension add -g WixToolset.UI.wixext/4.0.6
SkipApplicationBuild packages BuildDirectory\release\apexBasketCrane2.exe
only after checking its embedded version against version.h.

To explicitly reserve a version without building:
  powershell -File installer\Set-Version.ps1 -Next
Or choose a higher release version (the next Release build increments again):
  powershell -File installer\Set-Version.ps1 -Version 2.6.0
Verify rollover behavior without changing the real version:
  powershell -File tests\versioning.ps1
Read MSI metadata and deployment policies without installing it:
  powershell -File tests\installer.ps1 -Msi <built-msi-path>
The packaging script runs both checks and the offline application safety suite.

Install the MSI as an administrator. It installs the x86 application, embedded
Qt application, VLC DLLs/plugins, configuration guide and Start Menu shortcuts.
No service or automatic application launch is configured. A stable UpgradeCode
replaces older MSI releases and blocks downgrades. Upgrade removal participates
in the installer transaction so a failed upgrade can restore the old product.
Same-version builds are not new releases; increment before distributing updates.

Site configuration is C:\ProgramData\Apex\BasketCrane2\basketCrane2.ini.
The Start Menu shortcut passes this path explicitly. The placeholder template
selects LiveObserver and contains no production passwords/endpoints. An
administrator must replace placeholders or copy the site's existing validated
INI here. Installation, repair and upgrades never overwrite an existing INI;
uninstallation intentionally retains it. Existing portable INIs are not moved.
Verify settings offline using:
  apexBasketCrane2.exe --check-config --config "C:\ProgramData\Apex\BasketCrane2\basketCrane2.ini"
  apexBasketCrane2.exe --check-safety
Check-config checks syntax; it does not establish that placeholders are usable.

Database connections use Microsoft OLE DB Driver 19 for SQL Server directly.
No ODBC driver or DSN is required. The x86 MSOLEDBSQL19 client must be installed;
the MSI checks for it before installation. Microsoft's download page is:
https://learn.microsoft.com/sql/connect/oledb/download-oledb-driver-for-sql-server
Set Server, Catalog, User, Password, Encrypt and TrustServerCertificate in each
database section. The template uses Mandatory encryption and certificate validation.
For upgrades from a DSN configuration, run the packaged Migrate-Configuration.ps1
on the old configuration before starting the new app. It makes a backup and
resolves the registered server once; no runtime DSN lookups remain.
The MSI does not provision SQL databases, change PLC settings or install database
credentials. It preserves the site's INI, so existing settings require migration.
Use --check-databases for SELECT-only connection checks without the HMI or PLCs.
Keep configuration permissions
appropriate for the site's shared accounts. Third-party VLC notices accompany
the exact existing runtime; this packaging change does not upgrade VLC or Qt.

Remove the application through Windows Installed apps. Configuration is retained
for reinstall. Installer signing requires the organization's signing certificate;
the local build produces an unsigned MSI.
