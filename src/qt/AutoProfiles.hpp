#pragma once
#include <QString>
#include <optional>

struct AutoProfile {
    QString cpu;
    int     ramMB;
    QString videoCard;
    QString soundCard;
    QString chipset;
    QString storageController;
    QString bios;
};

std::optional<AutoProfile> getAutoProfile(const QString &family, const QString &version);