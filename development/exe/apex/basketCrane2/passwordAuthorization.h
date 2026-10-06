#pragma once

#include <QString>
#include <functional>
#include <utility>

namespace basket {

class PasswordAuthorization final
{
public:
    using CredentialVerifier = std::function<bool(const QString&)>;

    PasswordAuthorization(QString strongPassword, CredentialVerifier verifier)
        : strongPassword_(std::move(strongPassword)), verifier_(std::move(verifier))
    {
    }

    bool Authorize(const QString& suppliedPassword, const QString& requiredPassword = QString()) const
    {
        if (suppliedPassword.isEmpty()) return false;
        if (verifier_ && verifier_(suppliedPassword)) return true;
        const QString& expected = requiredPassword.isEmpty() ? strongPassword_ : requiredPassword;
        return !expected.isEmpty() && suppliedPassword == expected;
    }

private:
    QString strongPassword_;
    CredentialVerifier verifier_;
};

} // namespace basket