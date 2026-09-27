# Migration Guide for nnSdk/nnWare split

**Please do not create new PRs here during the migration.**
The new repos are already in good shape, so please direct future contributions there.

Timeline: Please see the end of the document for a detailed description
for each phase.

| What | When |
| ---- | ---- |
| Phase 1: Port current `nnsdk` files | ✅ Done |
| Phase 1: Port current `nnware` files | ✅ Done |
| Phase 1.5 Port `nnheaders` PRs | 2026-10 |
| Phase 2: `sead` compatibility gate (default on) | 2026-11 |
| Phase 2: `NintendoSDK-NEX` compatibility gate (default on) | 2026-11 |
| Phase 3: BOTW update | Before 2027 |
| Phase 3: SMO update | Before 2027 |
| Phase 3: `sead` gate default off | Before 2027 |
| Phase 3: `NintendoSDK-NEX` gate default off | Before 2027 |
| Phase 4: Migration complete | Before 2027 |

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

Additionally, with the exception of `nnSdk` which is an INTERFACE target (header-only),
the other targets will be changed from OBJECT to STATIC. The difference is that
an OBJECT target produces a bunch of `.o` objects, while a STATIC target zips
those objects into an `.a` archive (known as a static library, or staticlib).

We made this decision because:
- Linking a STATIC library automatically links objects of its dependencies. So, say your
  project depends on a library `A`, which depends on objects from another library `B`.
  If `A` is OBJECT, you also have to manually link `B` in your project, but if `A` is STATIC,
  CMake automatically adds objects from `B`.
- Sources suggest that this is what Nintendo did.

**This might require a slightly more complex change to downstream matching decompilation projects. See "How to migrate?" below.**

### Header Reorganization

The current headers don't follow consistent naming convention and placement. With the migration,
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
    - Use the same aliases from another library, for example `<basis/seadTypes.h>` from `sead`
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
git add .
git commit -m "add new nnsdk and nnware"
```

Then, change the `CMakeLists.txt` for your project and their dependencies:
```cmake
# old:
add_subdirectory(lib/NintendoSDK)
target_link_libraries(my_project PRIVATE NintendoSDK)

# new:
add_subdirectory(lib/nnsdk)
add_subdirectory(lib/nnware)

# you only need to link what you need, the example shows everything
target_link_libraries(my_project PRIVATE nnSdk nn_gfx nvn) # nnsdk targets
target_link_libraries(my_project PRIVATE nn_atk nn_font nn_g3d nn_ui2d nn_vfx) # nnware targets
```

Now you can re-run CMake and clean-build your project to make sure everything builds fine.

> [!NOTE]
>
> For matching decompilation projects, it's possible that after doing the changes
> above, you will find some functions are now missing from the final binary. This is because
> when linking a static library (`.a`), only objects in the library that are referenced
> by the main program are linked, whereas `.o` files are always linked.
>
> To fix this, we need to tell the linker to always link everything in the archive,
> using the `--whole-archive` ld flag. In CMake, specify the `WHOLE_ARCHIVE` feature
> in the link step:
> ```cmake
> add_subdirectory(lib/nnsdk)
> # target_link_libraries(uking PUBLIC nn_gfx) # <- old, does not link whole archive
> target_link_libraries(uking PUBLIC $<LINK_LIBRARY:WHOLE_ARCHIVE,nn_gfx>)
> ```
>
> If CMake now gives an error saying `WHOLE_ARCHIVE` is not supported,
> it's because it does not know how to interpret WHOLE_ARCHIVE for your `CMAKE_SYSTEM_NAME`.
>
> You have 2 options:
> 1. Change `CMAKE_SYSTEM_NAME` to `Linux`
> 2. Specify a custom feature, note it has to be lowercase because uppercase is reserved.
>    Put the block below in your CMakeLists.txt, or any file it includes (such as a Toolchain file).
>    This requires CMake >=3.30
> ```cmake
> # CMake only provides the WHOLE_ARCHIVE link feature for known platforms, not Generic.
> # Usage: target_link_libraries(target PRIVATE "$<LINK_LIBRARY:whole_archive,the_lib>")
> #    or: set_property(TARGET target PROPERTY LINK_LIBRARY_OVERRIDE_the_lib whole_archive)
> set(CMAKE_CXX_LINK_LIBRARY_USING_whole_archive
>     "LINKER:--whole-archive" "<LINK_ITEM>" "LINKER:--no-whole-archive")
> set(CMAKE_CXX_LINK_LIBRARY_USING_whole_archive_SUPPORTED TRUE)
> # Allow mixing with links of the same library without a feature (requires CMake 3.30)
> # This is important because some dependencies in the middle can link a library
> # without whole_archive, for example your_project -links-> nn_g3d -links-> nn_gfx
> set(CMAKE_LINK_LIBRARY_whole_archive_ATTRIBUTES
>    LIBRARY_TYPE=STATIC DEDUPLICATION=YES OVERRIDE=DEFAULT)
> ```
>
> and now this should work: (note the lowercase `whole_archive`)
>
> ```cmake
> add_subdirectory(lib/nnsdk)
> # target_link_libraries(uking PUBLIC nn_gfx) # <- old, does not link whole archive
> target_link_libraries(uking PUBLIC $<LINK_LIBRARY:whole_archive,nn_gfx>)
> ```
>
> This change might be temporary if the objects are missing because you haven't
> decompiled anything that references them. You can try removing this in the future
> as the project progresses.
>

Finally, remove the old repo (assuming it's at `lib/NintendoSDK`):
```bash
# with git
git submodule deinit -f lib/NintendoSDK
git rm -f lib/NintendoSDK
rm -rf .git/modules/lib/NintendoSDK

# with magoo
magoo remove lib/NintendoSDK

# commit the removal
git add .
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

## Timeline and Phases

We aim to make the migration process as streamlined as possible
and give downstream projects plenty of time to migrate, with the goal
of archiving and discontinuing this repository.

### Phase 1: New repository creation (Done)

New repositories are created with the new architecture described above:
- [`open-ead/nnsdk`](https://github.com/open-ead/nnsdk)
- [`open-ead/nnware`](https://github.com/open-ead/nnware)

No action is required for downstream projects during this time,
`nnheaders`, `sead`, and `NintendoSDK-NEX` remain the same as-is.

After this phase, the new repos will start accepting PRs.

### Phase 1.5: Port of open PRs (In progress)

Existing PRs on this repo will be analyzed and ported on individual basis.
PRs that are not going to be ported are tagged `port:not-planned`.

Each ported PR will retain the current commits by the PR author at the time of porting,
a port commit will be authored on top, followed by a merge commit into the `main` branch.

**Please do not create new PRs here during the migration.**
The new repos are already in good shape, so please direct future contributions there.

### Phase 2: Compatibility gate of `open-ead` libraries: `sead` and `NintendoSDK-NEX`

`sead` and `NintendoSDK-NEX` are the only 2 repos that currently have a dependency
on the SDK. If your project uses either of these libraries, it's likely that you
also already have a dependency on the SDK.

During this phase, these libraries will introduce compatibility gates: `SEAD_USE_OLD_NNHEADERS_REPO`
and `NEX_USE_OLD_NNHEADERS_REPO`. These are CMake options and in-source macros
that gate changes required in the headers and sources to work with the new repos.

These options will be default to `ON` at this phase, so projects can continue to update to newer
versions of these libraries without any change to their setup. However it is recommended
that you try enabling these settings and start migrating to the new repos at this time,
so you can report any issues to us.

To enable the option, put something like this in your `CMakeLists.txt`:
```cmake
set(SEAD_USE_OLD_NNHEADERS_REPO OFF CACHE BOOL "Enable new nnsdk repo")
set(NEX_USE_OLD_NNHEADERS_REPO OFF CACHE BOOL "Enable new nnsdk repo")
```
Or define them in `cmake` command line invocation:
```bash
cmake -B build -DSEAD_USE_OLD_NNHEADERS_REPO=OFF -DNEX_USE_OLD_NNHEADERS_REPO=OFF ...
```

### Phase 3: Turning the compatibility off by default

Once *Breath of the Wild* and *Super Mario Odyssey* enables these settings and confirm the new repos
are working, the compatibility gates will be changed to `OFF` by default.

If you upgrade `sead` or `NintendoSDK-NEX` without migrating to the new repo, you will get a loud error
in CMake, like:
```
Could not find NintendoSDK! Make sure you add open-ead/nnsdk to your project
and put `add_subdirectory(path/to/nnsdk)` somewhere in your CMakeLists.txt.
Alternatively, set SEAD_USE_OLD_NNHEADERS_REPO=ON in your project to continue
using the old nnheaders repo as a temporary unblock until you migrate.
```

At this time, you should migrate your projects to the new repos because the old repo will soon
be discontinued. But `-DSEAD_USE_OLD_NNHEADERS_REPO=ON` is the escape hatch if you still have
to use the old repo.

### Phase 4: Discontinue and archive `nnheaders`

At this phase, this repo will be archived and discontinued.

The compatibility options from `sead` and `NintendoSDK-NEX` will also be removed.
You must migrate your project at this point to continue using newer revisions of `sead` and `NintendoSDK-NEX`.

After archival, CMake will throw an error by default for this repo unless you set
`USE_OLD_NNHEADERS_REPO=ON`. Treat this as an explicit request for existing and new projects
to use the new repos. You can set this option as an escape hatch if for some reason you must
continue to use this repo.





