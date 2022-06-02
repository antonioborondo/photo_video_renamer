#pragma once

#include <cstdio>
#include <filesystem>
#include <fstream>

class Filesystem_wrapper
{
protected:
    std::filesystem::path m_path;

    Filesystem_wrapper() = default;

public:
    ~Filesystem_wrapper()
    {
        std::filesystem::remove_all(m_path);
    }

    const std::filesystem::path path() const
    {
        return m_path;
    }
};

class Directory_wrapper: public Filesystem_wrapper
{
public:
    Directory_wrapper()
    {
        m_path = std::tmpnam(nullptr);
        std::filesystem::create_directory(m_path);
    }
};

class File_wrapper: public Filesystem_wrapper
{
public:
    File_wrapper(const Directory_wrapper& parent_directory, const std::filesystem::path& filename)
    {
        m_path = parent_directory.path();
        m_path /= filename;
        std::ofstream file{m_path};
    }
};
