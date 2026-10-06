#pragma once

#include <QFont>
#include <QPointer>
#include <QSplashScreen>

namespace basket {

// Owns the startup widget; legacy constructors report progress through a
// non-owning, automatically cleared observer while it is alive.
class StartupProgress final {
public:
    StartupProgress() : splash_(QPixmap(":/splash.png"))
    {
        Active() = &splash_;
        splash_.setFont(QFont("Arial", 12));
        splash_.show();
    }

    ~StartupProgress() { Active().clear(); }

    StartupProgress(const StartupProgress&) = delete;
    StartupProgress& operator=(const StartupProgress&) = delete;

    void Finish(QWidget& window) { splash_.finish(&window); }

    static void ShowMessage(const QString& message)
    {
        if (Active())
            Active()->showMessage(message, Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
    }

private:
    static QPointer<QSplashScreen>& Active()
    {
        static QPointer<QSplashScreen> active;
        return active;
    }

    QSplashScreen splash_;
};

} // namespace basket
