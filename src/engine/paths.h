#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

// Where the game finds its assets and where it keeps the high score,
// the same way on Windows and Linux.

#include <string>
#include <cstdlib>
#include <system_error>
#include <filesystem>
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

inline fs::path exeDir()
{
#ifdef _WIN32
	wchar_t buf[MAX_PATH];
	DWORD n = GetModuleFileNameW(NULL, buf, MAX_PATH);
	if (n > 0 && n < MAX_PATH)
		return fs::path(std::wstring(buf, n)).parent_path();
#else
	char buf[4096];
	ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (n > 0)
		return fs::path(std::string(buf, n)).parent_path();
#endif
	return fs::current_path();
}

// The "assets" folder: next to the program (the build copies it there), or in the
// current folder.
inline const fs::path &assetRoot()
{
	static const fs::path root = []() {
		const fs::path candidates[] = {exeDir() / "assets", fs::current_path() / "assets"};
		std::error_code ec;
		for (const fs::path &c : candidates)
			if (fs::is_directory(c, ec))
				return c.lexically_normal();
		return candidates[0];
	}();
	return root;
}

inline std::string assetPath(const std::string &relative)
{
	return (assetRoot() / relative).string();
}

// A folder the player can write to: %APPDATA%\AllMenMustDie on Windows,
// ~/.local/share/all-men-must-die on Linux.
inline std::string dataPath(const std::string &file)
{
	fs::path dir;
#ifdef _WIN32
	const char *base = std::getenv("APPDATA");
	if (base)
		dir = fs::path(base) / "AllMenMustDie";
#else
	const char *xdg = std::getenv("XDG_DATA_HOME");
	const char *home = std::getenv("HOME");
	if (xdg && *xdg)
		dir = fs::path(xdg) / "all-men-must-die";
	else if (home)
		dir = fs::path(home) / ".local" / "share" / "all-men-must-die";
#endif
	std::error_code ec;
	if (dir.empty() || (fs::create_directories(dir, ec), !fs::is_directory(dir, ec)))
		dir = exeDir();
	return (dir / file).string();
}

#endif
