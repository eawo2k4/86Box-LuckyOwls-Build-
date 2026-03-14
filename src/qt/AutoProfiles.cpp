#include "AutoProfiles.hpp"

static const AutoProfile Win95Preset = {
    "Intel Pentium 133",
    32,
    "S3 Trio64V+",
    "Sound Blaster 16",
    "Intel 430FX",
    "IDE",
    "AMI BIOS"
};

std::optional<AutoProfile>
getAutoProfile(const QString &family, const QString &version)
{
    if (family == "Microsoft Windows" && version == "Windows 95")
        return Win95Preset;

    return std::nullopt;
}
if (famil == "Microsoft Windows" && version == "Windows 95")
        return Win95Preset;
    return std::nullopt;
}