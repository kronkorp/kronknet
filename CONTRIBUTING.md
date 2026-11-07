# Contributing to KronkNet

Thanks for wanting to help! Bug reports, fixes, features, tests and docs are all welcome.

## Reporting a bug or asking for a feature

Open an [issue](https://github.com/kronkorp/kronknet/issues/new/choose) and pick the matching template (bug report, feature request or question). For a bug, give the platform, how to reproduce it, and what you expected.

## Setting up

```sh
git clone https://github.com/kronkorp/kronknet.git
cd kronknet/
./tools.sh -d        # Debug build (use -r for Release)
```

Run the tests with CMake and CTest:

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

On Windows only the smoke test runs, because the unit tests need `fork()`.

## Workflow

1. Fork the repo and branch off `dev` (not `main`), named after what you do: `feat/<name>`, `fix/<name>`, `docs/<name>`.
2. Make your change. Keep it focused: one topic per pull request.
3. Add or update tests when you change behavior, and make sure the existing ones pass.
4. Open a pull request **against `dev`** and fill in the template. `dev` is merged into `main` for releases.

## Commit messages

Start with a tag, then a short sentence describing what the change does:

| Tag     | Use for                                  |
|---------|------------------------------------------|
| `[ADD]` | A new feature, file or test              |
| `[FIX]` | A bug fix                                |
| `[UP]`  | An improvement or refactor               |

```
[FIX] Sending a UDP datagram no longer arms EPOLLOUT
```

## Code guidelines

- The library is C. Stick to ISO C and keep the public headers (`include/kronknet/`) compilable on both Linux and Windows. Platform-specific code goes in `src/platform/`.
- Prefix public symbols with `kn` / `KN` and export them with `KN_API`.
- Document public functions with Doxygen comments, like the existing headers.
- Follow the style of the surrounding code.
- A callback that fails returns one of the `KNEVT*` codes from `kronknet/macros/errdef.h`.

By contributing, you agree that your work is distributed under the project's [MIT license](LICENSE).
