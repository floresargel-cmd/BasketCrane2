# Basket Crane 2

Open `development/exe/apex/basketCrane2/sln/apexBasketCrane2.sln` in Visual Studio with the C++ desktop toolset and Windows SDK 10.0.26100.0. Use the Win32 platform.

The restored application includes direct SQL Server connections, environment controls, observer displays, installer scripts, and basket numbers in mission panels and new controller events.

## Local dependencies and configuration

Qt 5.7.0 static x86 libraries are external build dependencies and are not included in Git. Place the existing matching Qt distribution in `development/libs/#qt/qt.5.7.0.static.x32`. The debug linker expects `qtbase/lib/Qt5Cored.lib`; if your distribution names the file `Qt5Cored_.lib`, copy it to `Qt5Cored.lib`.

Copy `development/exe/apex/basketCrane2/installer/basketCrane2.template.ini` to `basketCrane2.ini` beside the executable, or supply its path with `--config`. Replace all `CHANGE_ME` values with local settings. Credentials, local configuration, Visual Studio caches, and recovery backups are excluded from Git.

See the application's `CONFIGURATION.txt`, `ARCHITECTURE.txt`, and `installer/README.txt` for environment policy, architecture, and packaging details.

## Validation

The Debug Win32 application build and the offline observer UI regression checks passed after restoration and the mission basket-number changes. The UI fixture runs with `--config <ini-path> --preview-ui <png-path>` and does not connect to plant databases or PLCs.