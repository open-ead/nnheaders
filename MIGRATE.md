# Migration Guide for nnSdk/nnWare split

Timeline:
| What | When |
| ---- | ---- |
| Port current `nnsdk` files | ✅ Done |
| Port current `nnware` files | ✅ Done |
| Port all `nnheaders` PRs | ETA 2026-09-28 |
| `sead` compatibility update | Before 2026-10 |
| other repos compatibility update | Before 2027 |
| Downstream projects update | Before 2027 |
| Archive this repo | Before 2027 |

## What
We have decided to split this repo into 2 and detached it as a fork
by creating new repos: [`open-ead/nnsdk`](https://github.com/open-ead/nnsdk)
and [`open-ead/nnware`](https://github.com/open-ead/nnware). The new repos
will preserve all history, but:
- `nnsdk` will only contain `NintendoSDK` headers and sources that are not part of `NintendoWare`
- `nnware` will only contain `NintendoWare` headers and sources.

The headers remain user-created and reverse engineered from public sources.
The current guidelines for this repo still apply to the new repos.

## Why?

There are many reasons:
- This repo was created as a fork from `ultimate-research`. However, it's completed unrelated now.
- The different SDK modules are combined into one CMake target `NintendoSDK`, which includes all of:
  - Headers for the shared `nnSdk` library.
  - Headers and sources for the statically-linked, non-Ware SDK modules.
  - Headers for the statically-linked, `NintendoWare` libraries.
  This makes it tricky for projects that do not depend on all parts. With the new setup, each
  separate part will be its own CMake target, allowing more flexibility for projects to only
  pull in what they need.
- This also becomes more consistent on what the repos should include. This whole discussion
  was triggered from how we want to include `curl`, which is an external library, but technically 
  part of the SDK.

## What changed?

### CMake Target Split

The single `NintendoSDK` CMake target library is now split into multiple libraries:

| What | New Repo | CMake library target |
|-|-|-|
| NintendoSDK: nnSdk headers |`open-ead/nnsdk`|`nnSdk`|
| NintendoSDK: nn_gfx (statically linked) | `open-ead/nnsdk` | `nn_gfx` |
| NintendoSDK: nvn, nvnTools | `open-ead/nnsdk` | `nvn` |
| NintendoWare: atk | `open-ead/nnware`| `nn_atk` |
| NintendoWare: font | `open-ead/nnware` | `nn_font` |
| NintendoWare: g3d | `open-ead/nnware` | `nn_g3d` |
| NintendoWare: ui2d | `open-ead/nnware` | `nn_ui2d` |
| NintendoWare: vfx | `open-ead/nnware` | `nn_vfx` |

### Header Reorganization

The current headers don't follow consistent nameing convention and placement. With the migration,
we also want to take the opportunity to fix that.

Notable changes:
- Headers are now named like `nn/abc/abc_What.h` and `nn/abc/detail/abc_What.h`
  - `nn/abc.h` includes all `nn/abc/abc_*.h`, as well as stuff that doesn't really fit into any `abc_What.h` header.
  - Some parts will only have `nn/abc.h` and not the subdirectory, if there isn't enough work done to split them.
- Headers will be named more closely following the namespace. `nn::irsensor` will be `nn/irsensor.h`, not `nn/irs.h`.
- Top level `nn_What.h` headers for things directly in the `nn` namespace, most noteably `nn/nn_Result.h`
  - Currently, it still depends on the (modified) results headers from `libvapours`. This might change in the future.
- **`nn/types.h` will be removed**.
  - This is because there is little evidence that these aliases were used by the SDK.
  - You have a few options:
    - Include the types directly from `<cstddef>`, `<cstdint>` or `<nn/nn_Result.h>`, and use a script
      to replace `u*` and `s*` types with `uint*_t` and `int*_t`, `f32` and `f64` to `float` and `double`.
      `ulong` should be changed to `size_t` or `uint64_t`, depending on the context.
      You may try this python script
      ```python
      import pathlib
      import re

      # the source tree to migrate, e.g. "src" or "include"
      SOURCE = "..."
      EXTENSIONS = {".h", ".hpp", ".c", ".cpp", ".cc", ".cxx", ".inl"}

      TYPES = {
          "u8": "uint8_t", "u16": "uint16_t", "u32": "uint32_t", "u64": "uint64_t",
          "s8": "int8_t", "s16": "int16_t", "s32": "int32_t", "s64": "int64_t",
          "u128": "__uint128_t", "f32": "float", "f64": "double", "char16": "char16_t",
          "ulong": "size_t",
      }
      # (?!["']) avoids touching u8"..." string literal prefixes
      TYPE_RE = re.compile(r"\b(" + "|".join(TYPES) + r")\b(?![\"'])")
      INCLUDE_RE = re.compile(r'^([ \t]*)#include\s*[<"]nn/types\.h[>"]', re.MULTILINE)

      for path in pathlib.Path(SOURCE).rglob("*"):
          if not path.is_file() or path.suffix not in EXTENSIONS:
              continue
          old = path.read_text(encoding="utf-8")
          new = TYPE_RE.sub(lambda m: TYPES[m.group(1)], old)
          new = INCLUDE_RE.sub(
              lambda m: "\n".join(f"{m.group(1)}#include <{h}>"
                                  for h in ("cstddef", "cstdint", "nn/nn_Result.h")),
              new,
          )
          if new != old:
              path.write_text(new, encoding="utf-8")
              print(f"updated: {path}")
      ```
      Review the diff afterwards, since the regex also matches names in comments and strings,
      and drop the `nn/nn_Result.h` include from files that don't use `nn::Result`.
    - Use the same aliases from another library, for example `<prim/seadTypes.h>` from `sead`
    - Create your own types header.

### Other Improvements

We are also making other improvements that don't have visible effects to downstream projects

- Enable clang-format, clang-tidy, and CMake header verification

## How to migrate?

### CMake Projects

For CMake projects, it's assumed that you will include libraries as submodules.
If you are not familiar with git-submodule, consider using [magoo](https://github.com/Pistonite/magoo)
which is a git-submodule helper.

First, include either or both of the new repos (depends on what your project needs),
assuming you are putting the submodules in `lib/`

```bash
# with git
git submodule add https://github.com/open-ead/nnsdk lib/nnsdk
git submodule add https://github.com/open-ead/nnware lib/nnware

# with magoo
magoo install https://github.com/open-ead/nnsdk lib/nnsdk --name nnsdk --branch main
magoo install https://github.com/open-ead/nnware lib/nnware --name nnware --branch main

# commit the new submodule
git commit -m "add new nnsdk and nnware"
```

Then, change the `CMakeLists.txt` for your project and their dependencies:
```cmake
# old:
add_subdirectory(lib/NintendoSDK)
target_link_libraries(my_project PRIVATE NintendoSDK)

# new:
add_subdirectory(lib/nnsdk)

# you only need to link what you need, the example shows all 3 libraries
target_link_libraries(my_project PRIVATE nnSdk nn_gfx nvn)
```

Re-run CMake and clean-build your project to make sure everything builds fine, then, remove the old
repo (assuming it's at `lib/NintendoSDK`):
```
# with git
git submodule deinit -f lib/NintendoSDK
git rm -f lib/NintendoSDK
rm -rf .git/modules/lib/NintendoSDK

# with magoo
magoo remove lib/NintendoSDK

# commit the removal
git add .gitsubmodule
git commit -m "removed old NintendoSDK"
```

### Manually (non-CMake projects)

If your project does not use CMake, you might need to map the old include and source paths manually:

The structure for the new repos are:
```
open-ead/nnsdk/
├── include/nn/              # nnSdk headers
├── lib/
│   ├── gfx/
│   │   ├── include/nn/gfx/  # nn_gfx headers
│   │   ├── src/gfx/         # nn_gfx sources
│   │   └── CMakeLists.txt
│   └── nvn/
│       ├── include/
│       │   ├── nvn/         # nvn headers
│       │   ├── nvnTool/     # nvnTool headers
│       │   └── nv.h
│       ├── src/nvn/         # nvn sources (src/ is also on the include paths)
│       └── CMakeLists.txt
└── CMakeLists.txt

open-ead/nnware/
├── lib/
│   ├── atk/
│   │   ├── include/nn/atk/
│   │   ├── src/atk/
│   │   └── CMakeLists.txt
│   └── ...                  # same thing for each target
└── CMakeLists.txt
```

## Compatibility Patches to other `open-ead` repos

Before the migration is done, repos that depend on `nnheaders`, such as `sead`
will get a compatibility patch to work with the new repo, which includes:
- A CMake variable like `SEAD_USE_OLD_NNHEADERS_REPO`, which assumes
  you use the old `NintendoSDK` target. It will export the `ifdef` macros
  to use the old paths in the code.
- An ifdef macro of the same name `SEAD_USE_OLD_NNHEADERS_REPO` to
  conditionally choose the new or old includes.

You can receive updates from these repo while stilling using the `nnheaders`
repo by defining the `XXX_USE_OLD_NNHEADERS_REPO` variable or the macro
with the same name, example:
```bash
cmake -B build -S . -DSEAD_USE_OLD_NNHEADERS_REPO
```
Or directly in CMakeLists:
```cmake
option(SEAD_USE_OLD_NNHEADERS_REPO ON CACHE BOOL)
```

Note this is only a temporary escape hatch during the migration. These
options will be removed in the future, at which point you have to migrate
to the new repos to use newer revisions of those libraries.

## Current Open PRs

All Current PRs, including the WIP ones, will be ported and merged to the new repos.
During the initial phase of the migration, please do not create new PRs. 
You can create the PRs to the new repos once they are stood up, which should
only take a few days. Check the timeline at the top.

Each PR will retain the current commits by the PR author at the time of porting,
a port commit will be authored on top, followed by a merge commit into the `main` branch.
