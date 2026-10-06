#pragma once
#include <cfloat>
#include <cmath>
#include "emailAlerts.h"
// Scope this adaptation to the Crane 2 project. Load the legacy fatal helper
// under a private name before SQL headers define their inline error paths.
#define uExit basketLegacyFatalExit
#include <Gu.h>
#undef uExit
static void uExit(QString title,QString message) {
    EmailAlerts::Notify(title,message);
    EmailAlerts::StopActive();
    basketLegacyFatalExit(title,message);
}
