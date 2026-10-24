# Unarrow

A tiny C++ utility for Windows that removes or restores the arrows on shortcut icons by editing the registry.

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![License](https://img.shields.io/badge/license-GPL-green)

## Disclaimer

This tool modifies the Windows registry. Use at your own risk.

## Requirements

- Windows 10 or 11 (works on 7/8 too).
- **Administrator rights** (required to edit the registry).

## Usage

1. Run `Unarrow.exe` as administrator.
2. Pick an option:

```
1. Restore default (show arrows)
2. Remove arrows from shortcuts
0. Exit
```

The screen will flicker briefly while Explorer restarts — that's normal.

## How it works

The tool writes (or deletes) a single registry value:

```
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Icons
  └── 29 = %systemroot%\system32\imageres.dll,197
```

Pointing value `29` to a transparent icon hides the arrow overlay. Deleting the value brings it back.

## License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

You are free to use, study, share, and modify this software. Any distributed derivative work must also be released under the GPL-3.0. See the full text in the [`LICENSE`](LICENSE) file.
