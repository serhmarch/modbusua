#ifndef CNSTD_FILESYSTEM_H
#define CNSTD_FILESYSTEM_H

#include <CnString.h>

#include <filesystem>

namespace CnStd {

namespace filesystem = std::filesystem;

inline CnString toString(const filesystem::path &path)
{
#ifdef CN_USE_WSTRING
    return CnString(path.wstring().data());
#else
    return CnString(path.string().data());
#endif
}

} // namespace CnStd

#endif // CNSTD_FILESYSTEM_H